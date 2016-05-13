#include "Material.h"
#include "MathUtils.h"

Material::Material(const Vec4& diffuse, const Vec4& specular, float shininess){
  this->uuid = generateUUID();
  this->diffuseColor = diffuse;
  this->specularColor = specular;
  this->shininess = shininess;
  this->colorMap = std::shared_ptr<Texture>();
  this->normalMap = std::shared_ptr<Texture>();
  this->specularMap = std::shared_ptr<Texture>();
}

Material& Material::setDiffuseColor(const Vec4& diffuseColor){
  this->diffuseColor = diffuseColor;
  return *this;
}
Material& Material::setSpecularColor(const Vec4& specularColor){
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

std::string Material::getUUID()const{ return uuid; }

Vec4 Material::getDiffuseColor()const { return diffuseColor; }
Vec4 Material::getSpecularColor()const { return specularColor; }
float Material::getShininess()const { return shininess; }
std::shared_ptr<Texture> Material::getColorMap()const { return colorMap; }
std::shared_ptr<Texture> Material::getNormalMap()const { return normalMap; }
std::shared_ptr<Texture> Material::getSpecularMap()const { return specularMap; }

std::vector< std::pair <std::string,std::string> > Material::getShaders() const{
  std::vector< std::pair <std::string,std::string> > vec;
  std::string v = 
  "#version 330\n"
  ""
  "uniform mat4 worldMatrix;"
  "uniform mat4 projectionMatrix;"
  "uniform mat4 modelMatrix;"
  "layout(location = 0) in vec3 vPosition;"
  ""
  "void main(){"
  "gl_Position =  projectionMatrix * worldMatrix * modelMatrix *vec4(vPosition,1.0);"
  "}";
  vec.push_back(std::make_pair("vertex",v));

  std::string f =
  "#version 330\n"
  ""
  "layout(location = 0) out vec4 color;"
  "uniform float gamma;"
  ""
  "void main(){"
  "  float x = gl_FragCoord.x/800.0;"
  "  float y = gl_FragCoord.y/600.0;"
  "  color = pow(vec4(1-x,1-x,1-x,1.0),vec4(gamma,gamma,gamma,1.0));"
  "}";
  vec.push_back(std::make_pair("fragment",f));

  return vec;
}