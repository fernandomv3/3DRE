#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec4.h"
#include "Texture.h"
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <tuple>

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
  Material(const Vec4 diffuse = Vec4(), const Vec4 specular = Vec4(), float shininess = 0.0);

  Material& setDiffuseColor(const Vec4 diffuseColor);
  Material& setSpecularColor(const Vec4 specularColor);
  Material& setShininess(float shininess);
  Material& setColorMap(const std::shared_ptr<Texture> colorMap);
  Material& setNormalMap(const std::shared_ptr<Texture> normalMap);
  Material& setSpecularMap(const std::shared_ptr<Texture> specularMap);

  std::string getUUID()const;
  Vec4& getDiffuseColor();
  Vec4& getSpecularColor();
  float getShininess()const;
  std::shared_ptr<Texture> getColorMap()const;
  std::shared_ptr<Texture> getNormalMap()const;
  std::shared_ptr<Texture> getSpecularMap()const;
  virtual std::vector< std::pair<std::string,std::string> > getShaders() const;
  virtual std::unordered_map< std::string,std::shared_ptr<Texture> > getTextures(std::string passName) const;
  virtual std::vector< std::tuple<std::string,std::string,int,void*> >getUniforms(std::string passName);
};


#endif
