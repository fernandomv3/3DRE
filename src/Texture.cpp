#include "Texture.h"
#include "MathUtils.h"

Texture::Texture(const std::string& sourceFile){
  this->uuid = generateUUID();
  this->gamma = true;
  this->image= std::unique_ptr<char[]>();
  this->height= 0;
  this->width= 0;
  this->sourceFile = sourceFile;
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
  /*SDL_Surface* image = IMG_Load(this->sourceFile.c_str());
  this->width = image->w;
  this->height = image->h;

  if(image->format->BytesPerPixel == 3) {this->format = GL_RGB;}
  else if(image->format->BytesPerPixel == 4) {this->format = GL_RGBA;}
  char* data = new char[image->w * image->h * image->format->BytesPerPixel];
  memcpy(data,image->pixels,image->w * image->h * image->format->BytesPerPixel);
  this->image = std::unique_ptr<char[]>(data);
  
  if(image){
    SDL_FreeSurface(image);
  }*/
  
  return *this;
}