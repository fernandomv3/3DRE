#include "Framebuffer.h"
#include "MathUtils.h"
#include "GL/glew.h"

Framebuffer::Framebuffer(int width, int height){
  uuid = generateUUID();
  this->width = width;
  this->height = height;
  this->target = "rw";
  this->fbo = 0;
}
Framebuffer& Framebuffer::addRenderTarget(std::string name, std::shared_ptr<Texture> texture){
  return *this;
}
Framebuffer& Framebuffer::removeTarget(std::string name){
  return *this;
}

uint Framebuffer::init(){
  if (fbo == 0){
    glGenFramebuffers(1,&fbo);
  }
  return fbo;
}

std::unordered_map< std::string,std::shared_ptr<Texture> >& Framebuffer::getRenderTargets(std::string passName){ return renderTargets; }