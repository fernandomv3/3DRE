#include <iostream>
#include <memory>
#include "Camera.h"
#include "Scene.h"
#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"
#include "GLContext.h"
#include "Renderer.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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

int main(int argc, char** argv){

  initializeContext(context,window,SCREEN_WIDTH,SCREEN_HEIGHT);

  Camera cam;
  Scene scene;
  Renderer renderer;
  
  cam.perspectiveCamera(60,4.0/3.0,0.001,100.0);
  cam.getPosition()[2] = 10.0;

  auto geom = std::make_shared<Geometry>(quadGeometry());
  auto mat = std::make_shared<Material>(Material());
  auto obj = std::make_shared<Mesh>(Mesh(geom,mat));

  scene.add(obj);

  bool quit = false;
  while(!quit){
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    quit = handleEvents();
    renderer.render(scene,cam);
    SDL_GL_SwapWindow(window);
    SDL_Delay( 2000 );
  }

  return 0;
}