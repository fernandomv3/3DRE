#include <GLContext.h>
#include <iostream>
#include <sstream>
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

  Camera cam, lightCam;
  Scene scene, scene2;
  Renderer renderer(SCREEN_WIDTH,SCREEN_HEIGHT);
  //create shadowmap framebuffer
  auto shadowFb = std::make_shared<Framebuffer>(Framebuffer(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto depthMap = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  depthMap->setFormat("depth");
  shadowFb->addRenderTarget("depthMap",depthMap);
  auto depthMaterial = std::make_shared<Material>(Material());
  depthMaterial->setShaderFiles("/home/fernando/Projects/engine/shaders/vertex-transform.glsl","/home/fernando/Projects/engine/shaders/fragment-shadow.glsl");

  //create gBuffer

  auto gBuffer = std::make_shared<Framebuffer>(Framebuffer(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto positionTarget = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto normalTarget = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto diffuseTarget = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto specularTarget = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto depthTarget = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  depthTarget->setFormat("depth");
  gBuffer->addRenderTarget("position", positionTarget);
  gBuffer->addRenderTarget("normal", normalTarget);
  gBuffer->addRenderTarget("diffuse",diffuseTarget);
  gBuffer->addRenderTarget("specular",specularTarget);
  gBuffer->addRenderTarget("depth",depthTarget);

  //create post-process framebuffer
  /*auto writeFb = std::make_shared<Framebuffer>(Framebuffer(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto out1 = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto depth = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  depth->setFormat("depth");
  writeFb->addRenderTarget("depth",depth);
  writeFb->addRenderTarget("color",out1);*/

  //init cameras
  cam.perspectiveCamera(30.0,float(SCREEN_WIDTH)/float(SCREEN_HEIGHT),0.1,100.0);
  cam.getPosition()[2] = 10;
  lightCam.perspectiveCamera(30.0,float(SCREEN_WIDTH)/float(SCREEN_HEIGHT),0.1,100.0);
  lightCam.setTarget(std::make_shared<Vec4>(Vec4(0.0,0.0,0.0,1.0)));

  //add robot
  auto geometry = std::make_shared<Geometry>(loadDataFromFile("/home/fernando/Projects/engine/models/robot-low-normal.dae"));
  auto phongMat = std::make_shared<Material>(Material(Vec4(0.5,0.5,0.5,0.0),Vec4(1.0,1.0,1.0,0.0),20));
  phongMat->setShaderFiles("/home/fernando/Projects/engine/shaders/vertex_deferred.glsl","/home/fernando/Projects/engine/shaders/fragment_robot_deferred.glsl");
  auto robot = std::make_shared<Mesh>(Mesh(geometry,phongMat));

  auto texture = std::make_shared<Texture>(Texture("/home/fernando/Projects/engine/textures/normalMap.jpg"));
  texture->setGamma(false);
  texture->setNMipmaps(0);
  texture->setFiltering(std::make_pair("nearest","nearest"));
  phongMat->setNormalMap(texture);
  auto specTexture = std::make_shared<Texture>(Texture("/home/fernando/Projects/engine/textures/perlin.png"));
  specTexture->setGamma(false);
  specTexture->setNMipmaps(0);
  specTexture->setFiltering(std::make_pair("nearest","nearest"));
  phongMat->setSpecularMap(specTexture);
  scene.add(robot);
  robot->getRotation()[0] = 90;
  robot->getRotation()[2] = 180;

  auto quad = std::make_shared<Geometry>(quadGeometry());//shared geometry

  auto quadMat = std::make_shared<Material>(Material(Vec4(0.5,0.5,0.5,0.0)));
  quadMat->setShaderFiles("/home/fernando/Projects/engine/shaders/vertex2.glsl","/home/fernando/Projects/engine/shaders/deferredLightning.glsl");
  auto quadObj = std::make_shared<Mesh>(Mesh(quad,quadMat));
  scene2.add(quadObj);
  //add floor
  auto floorMat = std::make_shared<Material>(Material(Vec4(0.5,0.5,0.5,0.0)));
  floorMat->setShaderFiles("/home/fernando/Projects/engine/shaders/vertex_deferred.glsl","/home/fernando/Projects/engine/shaders/fragment_floor_deferred.glsl");

  auto floorQuad = std::make_shared<Mesh>(Mesh(quad,floorMat));
  auto tex = std::make_shared<Texture>(Texture("/home/fernando/Projects/engine/textures/sand.jpg"));
  auto floorNormal = std::make_shared<Texture>(Texture("/home/fernando/Projects/engine/textures/sand-normal.jpg"));
  floorNormal->setGamma(false);
  floorNormal->setNMipmaps(0);
  floorNormal->setFiltering(std::make_pair("nearest","nearest"));
  floorMat->setNormalMap(floorNormal);
  floorMat->setColorMap(tex);
  floorQuad->setScale(Vec4(10,10,10,1));
  
  floorQuad->getPosition()[1] = -2.3;
  floorQuad->getRotation()[0] = 90;
  floorQuad->getPosition()[2] = -5;
  scene.add(floorQuad);

  //add directional light
  auto dirlight = std::make_shared<Light>(Light(Vec4(0.8,0.8,0.8,1.0)));
  dirlight->setPosition(Vec4(6.0,7.0,7.5,0.0));
  scene.add(dirlight);
  
  bool quit = false;
  float currTime = SDL_GetTicks();
  glClearColor(1.0,1.0,1.0,1.0);

  while(!quit){
    float newTime = SDL_GetTicks();
    float dt = (newTime - currTime)*0.001;
    currTime = newTime;

    quit = handleEvents();
    float t = currTime*0.001;
    renderer.setTime(t);
    robot->getRotation()[2] -= 50*dt;

    lightCam.setPosition(dirlight->getPosition());

    robot->setMaterial(depthMaterial);
    floorQuad->setMaterial(depthMaterial);
    renderer.setWriteFramebuffer(shadowFb);
    renderer.setReadFramebuffer(nullptr);
    //glCullFace(GL_FRONT);
    renderer.render(scene,lightCam,"shadow");
    //glCullFace(GL_BACK);
    dirlight->setLightMatrix(cross(lightCam.getProjectionMatrix(),lightCam.getWorldMatrix()));

    robot->setMaterial(phongMat);
    floorQuad->setMaterial(floorMat);
    renderer.setWriteFramebuffer(nullptr);
    //renderer.setWriteFramebuffer(writeFb);
    renderer.setWriteFramebuffer(gBuffer);
    renderer.setReadFramebuffer(shadowFb);
    renderer.render(scene,cam,"forward");
    
    renderer.setWriteFramebuffer(nullptr);
    renderer.setReadFramebuffer(gBuffer);
    renderer.render(scene2,cam,"second");
    SDL_GL_SwapWindow(window);
  }
  cleanUp(window,context);
  return 0;
}