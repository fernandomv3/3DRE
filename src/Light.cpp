#include "Light.h"

Light::Light(const Vec4 color, float intensity, float attenuation):Mesh(std::shared_ptr<Geometry>(),std::shared_ptr<Material>()){
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