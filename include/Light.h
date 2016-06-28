#ifndef LIGHT_H
#define LIGHT_H
#include "Mesh.h"
#include "Texture.h"
#include <memory>

class Light : public Mesh {
private:
  Vec4 color;
  float intensity;
  float attenuation;
  Mat4 lightMatrix;
public:
  Light(const Vec4 color = Vec4(), float intensity = 1.0, float attenuation = 0.0);
  Vec4& getColor();
  float getIntensity()const;
  float getAttenuation()const;
  Mat4 getLightMatrix()const;
  Light& setLightMatrix(const Mat4 matrix);
  virtual std::vector< std::tuple<std::string,std::string,int,void*> >getUniforms(std::string passName);
};

#endif