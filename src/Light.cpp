#include "Light.h"

Light::Light(const Vec4 color, float intensity, float attenuation):Mesh(nullptr,nullptr){
  this->color = color;
  this->intensity = intensity;
  this->attenuation = attenuation;
  this->lightMatrix = Mat4::identity();
}
Vec4& Light::getColor() { return color; }
float Light::getIntensity()const { return intensity; }
float Light::getAttenuation()const { return attenuation; }
Mat4 Light::getLightMatrix()const { return lightMatrix; }
Light& Light::setLightMatrix(const Mat4 matrix){
	this->lightMatrix = matrix;
	return *this;
}
std::vector< std::tuple<std::string,std::string,int,void*> > Light::getUniforms(std::string passName){
  std::vector< std::tuple<std::string,std::string,int,void*> > res;
  res.push_back(std::make_tuple("light.color","4fv",1,this->color.getElements().data()));
  res.push_back(std::make_tuple("light.position","4fv",1,this->getPosition().getElements().data()));
  res.push_back(std::make_tuple("light.attenuation","1f",1,&this->attenuation));
  res.push_back(std::make_tuple("light.intensity","1f",1,&this->intensity));
  res.push_back(std::make_tuple("light.depthMatrix","m4fv",1,this->lightMatrix.getElements().data()));
  return res;
}