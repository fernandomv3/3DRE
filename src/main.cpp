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
#include "MathUtils.h"

bool handleEvents(Camera& cam, Scene& scene, Light& light){
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    switch(event.type){
      case SDL_QUIT:
        return true;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym){
          case SDLK_w:
            cam.getPosition()[1] += 0.1;
            break;
          case SDLK_s:
            cam.getPosition()[1] -=0.1;
            break;
          case SDLK_a:
            cam.getPosition()[0] += 0.1;
            break;
          case SDLK_d:
            cam.getPosition()[0] -= 0.1;
            break;
          case SDLK_l:
            light.getPosition()[0] += 0.1;
            break;
          case SDLK_j:
            light.getPosition()[0] -= 0.1;
            break;
          case SDLK_i:
            light.getPosition()[1] += 0.1;
            break;
          case SDLK_k:
            light.getPosition()[1] -= 0.1;
            break;
          case SDLK_u:
            light.getPosition()[2] -= 0.1;
            break;
          case SDLK_o:
            light.getPosition()[2] += 0.1;
            break;
      }
    }
  }
  return false;
}

std::vector<Vec4> generateSsaoKernel(int kernelSize){
  std::vector<Vec4> kernel;
  for(int i= 0; i < kernelSize;i++){
    Vec4 v = Vec4(random(-1.0,1.0),random(-1.0,1.0),random(0.0,1.0),0.0).normalize();
    float scale = float(i)/float(kernelSize);
    scale = lerp(0.1,1.0,scale*scale);
    v *= scale;
    kernel.push_back(v);
  }
  return kernel;
}

std::shared_ptr<Texture> generateSsaoNoise(int noiseSize){
  auto tex = std::make_shared<Texture>(noiseSize,noiseSize);
  tex->setType(std::type_index(typeid(float)));
  tex->setInnerFormat("RGBA16F");
  tex->setWrapping(std::make_tuple("repeat","repeat",""));
  std::vector<float> noise;
  for(int i = 0,size=noiseSize*noiseSize;i< size;i++){
    Vec4 v = Vec4(random(-1.0,1.0),random(-1.0,1.0),0.0,0.0).normalize();
    noise.push_back(v[0]);
    noise.push_back(v[1]);
    noise.push_back(v[2]);
    noise.push_back(v[3]);
  }
  tex->setType(std::type_index(typeid(float)));
  tex->loadData(noise.data(),noiseSize,noiseSize);
  return tex;
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
  depthMap->setInnerFormat("depth");
  shadowFb->addRenderTarget("depthMap",depthMap);
  auto depthMaterial = std::make_shared<Material>(Material());
  depthMaterial->setShaderFiles("/home/fernando/Projects/engine/shaders/vertex-transform.glsl","/home/fernando/Projects/engine/shaders/fragment-shadow.glsl");
  
  //create gBuffer
  auto gBuffer = std::make_shared<Framebuffer>(Framebuffer(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto positionTarget = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  positionTarget->setType(std::type_index(typeid(float)));
  positionTarget->setInnerFormat("RGBA16F");
  auto normalTarget = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  normalTarget->setType(std::type_index(typeid(float)));
  normalTarget->setInnerFormat("RGBA16F");
  auto diffuseTarget = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto specularTarget = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  auto depthTarget = std::make_shared<Texture>(Texture(SCREEN_WIDTH,SCREEN_HEIGHT));
  depthTarget->setFormat("depth");
  depthTarget->setInnerFormat("depth");
  gBuffer->addRenderTarget("position", positionTarget);
  gBuffer->addRenderTarget("normal", normalTarget);
  gBuffer->addRenderTarget("diffuse",diffuseTarget);
  gBuffer->addRenderTarget("specular",specularTarget);
  gBuffer->addRenderTarget("depth",depthTarget);

  //init cameras
  cam.perspectiveCamera(30.0,float(SCREEN_WIDTH)/float(SCREEN_HEIGHT),0.1,100.0);
  cam.getPosition()[2] = 10;
  cam.updateWorldMatrix();
  cam.setTarget(std::make_shared<Vec4>(Vec4(0.0,0.0,0.0,1.0)));

  lightCam.perspectiveCamera(30.0,float(SCREEN_WIDTH)/float(SCREEN_HEIGHT),0.1,100.0);
  lightCam.setTarget(std::make_shared<Vec4>(Vec4(0.0,0.0,0.0,1.0)));

  auto quad = std::make_shared<Geometry>(quadGeometry());//shared geometry

  auto quadMat = std::make_shared<Material>(Material(Vec4(0.5,0.5,0.5,0.0)));
  quadMat->setShaderFiles("/home/fernando/Projects/engine/shaders/vertex2.glsl","/home/fernando/Projects/engine/shaders/deferredLightning.glsl");

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

  //add spheres
  auto geometry = std::make_shared<Geometry>(loadDataFromFile("/home/fernando/Projects/engine/models/icosphere.dae"));
  auto phongMat = std::make_shared<Material>(Material(Vec4(0.5,0.5,1.0,0.0),Vec4(0.0,0.0,0.0,0.0),20));
  phongMat->setShaderFiles("/home/fernando/Projects/engine/shaders/vertex_deferred.glsl","/home/fernando/Projects/engine/shaders/fragment_sphere_deferred.glsl");
  std::vector<std::shared_ptr<Mesh>> sphereArray;
  for(int i = 0; i < 5;i++){
    for(int j = 0; j < 5 ; j++){
      auto mesh = std::make_shared<Mesh>(Mesh(geometry,phongMat));
      mesh->setPosition(Vec4(i-2,-1.8,j,1.0));
      mesh->setScale(Vec4(0.5,0.5,0.5,1.0));
      sphereArray.push_back(mesh);
    }
  }
  for(auto m : sphereArray){
    scene.add(m);
  }
  //add directional light
  auto dirlight = std::make_shared<Light>(Light(Vec4(0.8,0.8,0.8,1.0)));
  dirlight->setMaterial(quadMat);
  dirlight->setGeometry(quad);
  dirlight->setPosition(Vec4(6.0,7.0,7.5,0.0));
  quadMat->setDepthMap(depthMap);
  scene2.add(dirlight);

  scene2.setSsaoKernel(generateSsaoKernel(8));
  scene2.setSsaoNoise(generateSsaoNoise(4));
  scene2.setSsaoRadius(1);

  bool quit = false;
  float currTime = SDL_GetTicks();
  glClearColor(0.0,0.0,0.0,0.0);

  while(!quit){
    float newTime = SDL_GetTicks();
    float dt = (newTime - currTime)*0.001;
    currTime = newTime;

    quit = handleEvents(cam,scene,*dirlight);
    float t = currTime*0.001;
    renderer.setTime(t);

    lightCam.setPosition(dirlight->getPosition());
    for(auto m : sphereArray) m->setMaterial(depthMaterial);
    floorQuad->setMaterial(depthMaterial);
    renderer.setReadFramebuffer(nullptr);
    renderer.setWriteFramebuffer(shadowFb);
    glCullFace(GL_FRONT);
    renderer.render(scene,lightCam,"shadow");
    glCullFace(GL_BACK);
    dirlight->setLightMatrix(cross(lightCam.getProjectionMatrix(),lightCam.getWorldMatrix()));

    for(auto m : sphereArray) m->setMaterial(phongMat);
    floorQuad->setMaterial(floorMat);

    renderer.setReadFramebuffer(nullptr);
    renderer.setWriteFramebuffer(gBuffer);
    renderer.render(scene,cam,"geometry");

    renderer.setReadFramebuffer(gBuffer);
    renderer.setWriteFramebuffer(nullptr);
    renderer.render(scene2,cam,"deferred");
    SDL_GL_SwapWindow(window);
  }
  cleanUp(window,context);
  return 0;
}