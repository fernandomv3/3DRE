#define NO_SDL_GLEXT
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <iomanip>
#include <memory>
#include "Camera.h"
#include "Scene.h"
#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_GLContext context = NULL;

bool handleEvents(){
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    switch(event.type){
      case SDL_QUIT:
        return true;
    }
  }
  return false;
}

void initializeContext(){
  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
  }
  else{
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    window = SDL_CreateWindow(
      "Engine",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL 
    );
    if(window == NULL){
      std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    }else{
      context = SDL_GL_CreateContext( window );
      if( context == NULL ){
        std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
      }
      else{
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
          std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        }

        if(!GLEW_VERSION_3_3){
          std::cerr << "OpenGL 4.5 not available" << std::endl;
          exit(-1);
        }
        const unsigned char* version = glGetString(GL_VERSION);
        if (version == NULL) {
          exit(-1);
        }
        SDL_GL_SetSwapInterval(0);
        std::cout << version << std::endl;
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_MULTISAMPLE);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glEnable( GL_PROGRAM_POINT_SIZE);
      }
    }
  }
}

void cleanUp(){
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char** argv){

  initializeContext();

  Camera cam;
  Scene scene;
  Renderer renderer(SCREEN_WIDTH,SCREEN_HEIGHT);
  
  cam.perspectiveCamera(30.0,float(SCREEN_WIDTH)/float(SCREEN_HEIGHT),0.1,100.0);
  cam.getPosition()[2] = 15;

  auto geom = std::make_shared<Geometry>(icosahedronGeometry());
  auto mat = std::make_shared<Material>(Material(Vec4(1.0,0.0,0.0,0.0)));
  //auto tex = std::make_shared<Texture>(Texture("checkers.png"));
  //mat->setColorMap(tex);
  auto obj = std::make_shared<Mesh>(Mesh(geom,mat));
  auto obj2 = std::make_shared<Mesh>(Mesh(geom,mat));
  auto parentObj = std::make_shared<Object3D>(Object3D());
  parentObj->add(obj2);

  scene.add(obj);
  scene.add(obj2);

  auto dirlight = std::make_shared<Light>(Light(Vec4(0.8,0.8,0.8,1.0)));
  dirlight->setPosition(Vec4(2.0,2.0,2.0,0.0));
  scene.add(dirlight);

  bool quit = false;
  parentObj->getPosition()[0]=3;
  parentObj->getPosition()[1]=3;
  while(!quit){
    float t = SDL_GetTicks()*0.001;
    renderer.setTime(t);
    float s = 0.5 * sin(t) + 1 ;
    parentObj->getRotation()[2] += 0.05;
    obj2->getRotation()[2] += 0.1;
    obj->getRotation()[2] -= 0.1;
    obj->setScale(Vec4(s,s,s,0.0));
    quit = handleEvents();
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer.render(scene,cam,"forward");
    SDL_GL_SwapWindow(window);
  }
  cleanUp();
  return 0;
}