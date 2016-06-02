#include "Material.h"
#include "MathUtils.h"

Material::Material(const Vec4 diffuse, const Vec4 specular, float shininess){
  this->uuid = generateUUID();
  this->diffuseColor = diffuse;
  this->specularColor = specular;
  this->shininess = shininess;
  this->colorMap = std::shared_ptr<Texture>();
  this->normalMap = std::shared_ptr<Texture>();
  this->specularMap = std::shared_ptr<Texture>();
}

Material& Material::setDiffuseColor(const Vec4 diffuseColor){
  this->diffuseColor = diffuseColor;
  return *this;
}
Material& Material::setSpecularColor(const Vec4 specularColor){
  this->specularColor = specularColor;
  return *this;
}
Material& Material::setShininess(float shininess){
  this->shininess = shininess;
  return *this;
}
Material& Material::setColorMap(const std::shared_ptr<Texture> colorMap){
  this->colorMap = colorMap;
  return *this;
}
Material& Material::setNormalMap(const std::shared_ptr<Texture> normalMap){
  this->normalMap = normalMap;
  return *this;
}
Material& Material::setSpecularMap(const std::shared_ptr<Texture> specularMap){
  this->specularMap = specularMap;
  return *this;
}
Material& Material::setShaderFiles(const std::string vf, const std::string ff){
  this->vf = vf;
  this->ff = ff;
  return *this;
}

std::string Material::getUUID()const{ return uuid; }

Vec4& Material::getDiffuseColor() { return diffuseColor; }
Vec4& Material::getSpecularColor() { return specularColor; }
float Material::getShininess()const { return shininess; }
std::shared_ptr<Texture> Material::getColorMap()const { return colorMap; }
std::shared_ptr<Texture> Material::getNormalMap()const { return normalMap; }
std::shared_ptr<Texture> Material::getSpecularMap()const { return specularMap; }

std::vector< std::pair <std::string,std::string> > Material::getShaders() const{
  std::vector< std::pair <std::string,std::string> > vec;
  vec.push_back(std::make_pair("vertexFile",vf));
  vec.push_back(std::make_pair("fragmentFile",ff));
  return vec;
}

std::unordered_map< std::string,std::shared_ptr<Texture> > Material::getTextures(std::string passName) const{
  auto result = std::unordered_map< std::string,std::shared_ptr<Texture> >();
  if(colorMap != nullptr){
    colorMap->loadFile();
    result["colorMap"] = colorMap;
  }
  if(normalMap != nullptr){
    normalMap->loadFile();
    result["normalMap"] = normalMap;
  }
  if(specularMap != nullptr){
    specularMap->loadFile();
    result["specularMap"] = specularMap;
  }
  return result;
}

std::vector< std::tuple<std::string,std::string,int,void*> > Material::getUniforms(std::string passName){
  std::vector< std::tuple<std::string,std::string,int,void*> > res;
  res.push_back(std::make_tuple("material.diffuse","4fv",1,diffuseColor.getElements().data()));
  res.push_back(std::make_tuple("material.specular","4fv",1,specularColor.getElements().data()));
  res.push_back(std::make_tuple("material.shininess","1f",1,&shininess));
  return res;
}