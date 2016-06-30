#include "Framebuffer.h"
#include "MathUtils.h"
#include "GL/glew.h"

#include <algorithm>

Framebuffer::Framebuffer(int width, int height){
  uuid = generateUUID();
  this->width = width;
  this->height = height;
  this->target = "rw";
  this->fbo = 0;
  this->initialized = false;
}

bool Framebuffer::isInitialized() const{ return initialized; }
uint Framebuffer::getFbo()const{ return fbo; }
int Framebuffer::getWidth()const{ return width; }
int Framebuffer::getHeight()const{ return height; }

Framebuffer& Framebuffer::addRenderTarget(std::string name, std::shared_ptr<Texture> texture){
  renderTargets[name] = texture;
  insertOrder.push_back(name);
  return *this;
}

std::vector<std::string> Framebuffer::getTargetsOrder() const{
  return this->insertOrder;
}

Framebuffer& Framebuffer::removeTarget(std::string name){
  renderTargets.erase(name);
  insertOrder.erase(std::remove(insertOrder.begin(), insertOrder.end(), name), insertOrder.end());
  return *this;
}

uint Framebuffer::init(){
  if (fbo == 0){
    glGenFramebuffers(1,&fbo);
  }
  initialized = true;
  return fbo;
}

std::unordered_map< std::string,std::shared_ptr<Texture> >& Framebuffer::getRenderTargets(std::string passName){ return renderTargets; }