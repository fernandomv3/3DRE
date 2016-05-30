#include "Framebuffer.h"
#include "MathUtils.h"

Framebuffer::Framebuffer(int width, int height){
  uuid = generateUUID();
  this->width = width;
  this->height = height;
  this->target = "rw";
}
Framebuffer& Framebuffer::addRenderTarget(std::string name, std::shared_ptr<Texture> texture){
  return *this;
}
Framebuffer& Framebuffer::removeTarget(std::string name){
  return *this;
}

int Framebuffer::init(){

  return 0;
}

std::unordered_map< std::string,std::shared_ptr<Texture> >& Framebuffer::getRenderTargets(std::string passName){ return renderTargets; }