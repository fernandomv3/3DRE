#ifndef LIGHT_H
#define LIGHT_H
#include "Mesh.h"
#include "Vec4.h"

class Light : public Mesh {
private:
  Vec4 color;
  float intensity;
  float attenuation;
public:
  Light(const Vec4& color = Vec4(), float intensity = 1.0, float attenuation = 0.0);
  Vec4 getColor() const;
  float getIntensity()const;
  float getAttenuation()const;
};

#endif