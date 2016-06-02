#include "Texture.h"
#include "MathUtils.h"

#include <SDL2/SDL_image.h>

Texture::Texture(const std::string& sourceFile){
  this->uuid = generateUUID();
  this->sourceFile = sourceFile;
  this->image = std::unique_ptr<char[]>();
  this->height = 0;
  this->width = 0;
  this->nMipmaps = 3;
  this->wrapping = std::make_tuple("edge","edge","");
  this->filtering =std::make_pair("linear_mipmap_linear","linear");
  this->target = "2D";
  this->format = "RGB";
  this->innerFormat = "RGB8";
  this->gamma = true;
  this->loaded = false;
  this->type = std::type_index(typeid(unsigned char));
}
Texture::Texture(int w, int h){
  this->uuid = generateUUID();
  this->sourceFile = "";
  this->image = std::unique_ptr<char[]>();
  this->width = w;
  this->height = h;
  this->nMipmaps = 0;
  this->wrapping = std::make_tuple("edge","edge","");
  this->filtering =std::make_pair("nearest","nearest");
  this->target = "2D";
  this->format = "RGBA";
  this->innerFormat = "RGBA";
  this->gamma = false;
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
Texture& Texture::loadFile(){
  if(this->loaded)return *this;
  SDL_Surface* image = IMG_Load(this->sourceFile.c_str());
  this->width = image->w;
  this->height = image->h;

  if(image->format->BytesPerPixel == 3) {this->format = "RGB";}
  else if(image->format->BytesPerPixel == 4) {this->format = "RGBA";}
  char* data = new char[image->w * image->h * image->format->BytesPerPixel];
  memcpy(data,image->pixels,image->w * image->h * image->format->BytesPerPixel);
  this->image = std::unique_ptr<char[]>(data);
  
  if(image){
    SDL_FreeSurface(image);
  }
  this->loaded = true;
  return *this;
}
int Texture::getNMipmaps()const { return nMipmaps; }
std::tuple<std::string,std::string,std::string> Texture::getWrapping()const { return wrapping; }
std::pair<std::string,std::string> Texture::getFiltering()const { return filtering; }
std::string Texture::getTarget()const { return target; }
bool Texture::getLoaded()const { return loaded; }
std::string Texture::getFormat()const { return format; }
std::type_index Texture::getType()const { return type; }

Texture& Texture::setNMipmaps(int nMipmaps){
  this->nMipmaps = nMipmaps;
  return *this;
}
Texture& Texture::setWrapping(std::tuple<std::string,std::string,std::string> wrapping){
  this->wrapping = wrapping;
  return *this;
}
Texture& Texture::setFiltering(std::pair<std::string,std::string> filtering){
  this->filtering = filtering;
  return *this;
}
Texture& Texture::setTarget(std::string target){
  this->target = target;
  return *this;
}
Texture& Texture::setLoaded(bool loaded){
  this->loaded = loaded;
  return *this;
}
Texture& Texture::setFormat(std::string format){
  this->format = format;
  return *this;
}