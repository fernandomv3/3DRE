#include <GLContext.h>
#include <iostream>
#include <memory>
#include "Camera.h"
#include "Scene.h"
#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h"

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
  const int SCREEN_WIDTH = 800;
  const int SCREEN_HEIGHT = 600;

  SDL_Window* window = NULL;
  SDL_GLContext context = NULL;

  initializeContext(window,context,SCREEN_WIDTH,SCREEN_HEIGHT);

  Camera cam;
  Scene scene, scene2;
  Renderer renderer(SCREEN_WIDTH,SCREEN_HEIGHT);

  auto writeFb = std::make_shared<Framebuffer>(Framebuffer(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto out1 = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto depth = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  depth->setFormat("depth");
  writeFb->addRenderTarget("depth",depth);
  writeFb->addRenderTarget("color",out1);

  cam.perspectiveCamera(30.0,float(SCREEN_WIDTH)/float(SCREEN_HEIGHT),0.1,100.0);
  cam.getPosition()[2] = 15;

  auto geom = std::make_shared<Geometry>(icosahedronGeometry());
  auto mat = std::make_shared<Material>(Material(Vec4(1.0,0.0,0.0,0.0)));
  mat->setShaderFiles("/home/fernando/Projects/engine/vertex.glsl","/home/fernando/Projects/engine/fragment.glsl");

  auto obj = std::make_shared<Mesh>(Mesh(geom,mat));
  auto obj2 = std::make_shared<Mesh>(Mesh(geom,mat));
  auto parentObj = std::make_shared<Object3D>(Object3D());
  parentObj->add(obj2);

  scene.add(obj);
  scene.add(obj2);

  auto dirlight = std::make_shared<Light>(Light(Vec4(0.8,0.8,0.8,1.0)));
  dirlight->setPosition(Vec4(2.0,2.0,2.0,0.0));
  scene.add(dirlight);

  auto quad = std::make_shared<Geometry>(quadGeometry());
  auto quadMat = std::make_shared<Material>(Material(Vec4(1.0,0.0,0.0,0.0)));
  quadMat->setShaderFiles("/home/fernando/Projects/engine/vertex2.glsl","/home/fernando/Projects/engine/fragment2.glsl");
  auto tex = std::make_shared<Texture>(Texture("checkers.png"));
  //quadMat->setColorMap(tex);

  auto quadObj = std::make_shared<Mesh>(Mesh(quad,quadMat));
  scene2.add(quadObj);

  bool quit = false;
  parentObj->getPosition()[0]=3;
  parentObj->getPosition()[1]=3;
  float currTime = SDL_GetTicks();
  glClearColor(1.0,1.0,1.0,1.0);
  while(!quit){
    glClear(GL_COLOR_BUFFER_BIT);
    float newTime = SDL_GetTicks();
    float dt = (newTime - currTime)*0.001;
    currTime = newTime;

    quit = handleEvents();
    float t = currTime*0.001;
    renderer.setTime(t);
    float s = 0.5 * sin(t) + 1 ;
    parentObj->getRotation()[2] += 75*dt;
    obj2->getRotation()[2] += 150*dt;
    obj->getRotation()[2] -= 150*dt;
    obj->setScale(Vec4(s,s,s,0.0));
    renderer.setWriteFramebuffer(writeFb);
    renderer.setReadFramebuffer(nullptr);
    renderer.render(scene,cam,"forward");
    renderer.setWriteFramebuffer(nullptr);
    renderer.setReadFramebuffer(writeFb);
    renderer.render(scene2,cam,"second");
    SDL_GL_SwapWindow(window);
  }
  cleanUp(window,context);
  return 0;
}