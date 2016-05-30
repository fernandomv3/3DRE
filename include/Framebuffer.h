#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Texture.h"
#include <memory>
#include <unordered_map>
#include <string>

class Framebuffer{
private:
  std::string uuid;
  int width;
  int height;
  uint fbo;
  std::string target;
  std::unordered_map< std::string,std::shared_ptr<Texture> > renderTargets;
public:
  Framebuffer(int width, int height);
  uint init();
  Framebuffer& addRenderTarget(std::string name, std::shared_ptr<Texture> texture);
  Framebuffer& removeTarget(std::string name);
  std::unordered_map< std::string,std::shared_ptr<Texture> >& getRenderTargets(std::string passName);
};


#endif