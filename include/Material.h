#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec4.h"
#include "Texture.h"
#include <memory>
#include <string>

class Material{
private:
  std::string uuid;
  Vec4 diffuseColor;
  Vec4 specularColor;
  float shininess;
  std::shared_ptr<Texture> colorMap;
  std::shared_ptr<Texture> normalMap;
  std::shared_ptr<Texture> specularMap;
public:
  Material(const Vec4& diffuse = Vec4(), const Vec4& specular = Vec4(), float shininess = 0.0);

  Material& setDiffuseColor(const Vec4& diffuseColor);
  Material& setSpecularColor(const Vec4& specularColor);
  Material& setShininess(float shininess);
  Material& setColorMap(const std::shared_ptr<Texture> colorMap);
  Material& setNormalMap(const std::shared_ptr<Texture> normalMap);
  Material& setSpecularMap(const std::shared_ptr<Texture> specularMap);

  std::string getUUID();
  Vec4 getDiffuseColor()const;
  Vec4 getSpecularColor()const;
  float getShininess()const;
  std::shared_ptr<Texture> getColorMap()const;
  std::shared_ptr<Texture> getNormalMap()const;
  std::shared_ptr<Texture> getSpecularMap()const;

};


#endif
