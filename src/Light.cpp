#include "Light.h"

Light::Light(const Vec4 color, float intensity, float attenuation):Mesh(std::shared_ptr<Geometry>(),std::shared_ptr<Material>()){
  this->color = color;
  this->intensity = intensity;
  this->attenuation = attenuation;
}
Vec4& Light::getColor() { return color; }
float Light::getIntensity()const { return intensity; }
float Light::getAttenuation()const { return attenuation; }