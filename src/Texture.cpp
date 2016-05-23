#include "Texture.h"
#include "MathUtils.h"

#include <SDL2/SDL_image.h>

Texture::Texture(const std::string& sourceFile){
  this->uuid = generateUUID();
  this->gamma = true;
  this->image= std::unique_ptr<char[]>();
  this->height= 0;
  this->width= 0;
  this->sourceFile = sourceFile;
  this->alpha = true;
  this->dimensions= 2;
  this->wrapping= "repeat";
  this->filtering= "mipmap_linear";
  this->loaded = false;
  this->type = std::type_index(typeid(unsigned char));
}
const std::string& Texture::getUUID()const{ return uuid; }
const std::string& Texture::getSourceFile()const{ return sourceFile; }
const char* Texture::getImage()const { return this->image.get(); }
int Texture::getHeight()const{ return height; }
int Texture::getWidth()const{ return width; }
bool Texture::getGamma()const{ return gamma; }
Texture& Texture::setGamma(bool gamma){
  this->gamma = gamma;
  return *this;
}
bool Texture::hasAlpha()const{
  return alpha;
}
Texture& Texture::loadFile(){
  if(this->loaded)return *this;
  SDL_Surface* image = IMG_Load(this->sourceFile.c_str());
  this->width = image->w;
  this->height = image->h;

  if(image->format->BytesPerPixel == 3) {this->alpha = false;}
  else if(image->format->BytesPerPixel == 4) {this->alpha = true;}
  char* data = new char[image->w * image->h * image->format->BytesPerPixel];
  memcpy(data,image->pixels,image->w * image->h * image->format->BytesPerPixel);
  this->image = std::unique_ptr<char[]>(data);
  
  if(image){
    SDL_FreeSurface(image);
  }
  this->loaded = true;
  return *this;
}

  int Texture::getDimensions()const{ return dimensions; }
  std::string Texture::getWrapping()const{ return wrapping; }
  std::string Texture::getFiltering()const{ return filtering; }
  std::string Texture::getTarget()const{ return target; }
  bool Texture::getLoaded()const{ return loaded; }
  bool Texture::getAlpha()const{ return alpha; };
  std::type_index Texture::getType()const{ return type; }
  Texture& Texture::setDimensions(int dimensions){
    this->dimensions = dimensions;
    return *this;
  };
  Texture& Texture::setWrapping(std::string wrapping){
    this->wrapping = wrapping;
    return *this;
  };
  Texture& Texture::setFiltering(std::string filtering){
    this->filtering = filtering;
    return *this;
  };
  Texture& Texture::setTarget(std::string target){
    this->target = target;
    return *this;
  };
  Texture& Texture::setLoaded(bool loaded){
    this->loaded = loaded;
    return *this;
  };