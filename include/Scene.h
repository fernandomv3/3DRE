#ifndef SCENE_H
#define SCENE_H

#include "Light.h"
#include "Texture.h"
#include "Vec4.h"
#include <memory>
#include <vector>
#include <string>

class Scene{
private:
  std::string uuid;
  std::vector< std::shared_ptr<Object3D> > objects;
  std::vector< std::shared_ptr<Light> > lights;
  std::vector<Vec4> ssaoKernel;
  std::shared_ptr<Texture> ssaoNoise;
  Light ambientLight;

  std::vector<float> color;
  std::vector<float> position;
  std::vector<float> attenuation;
  std::vector<float> intensity;
  std::vector<float> matrices;
  std::vector<float> flatSsaoKernel;
public:
  Scene();
  std::string getUUID()const;
  const std::vector< std::shared_ptr<Object3D> >& getObjects()const;
  const std::vector< std::shared_ptr<Light> >& getLights()const;
  Scene& add(std::shared_ptr<Light> light);
  Scene& remove(std::shared_ptr<Light> light);
  Scene& add(std::shared_ptr<Object3D> object);
  Scene& remove(std::shared_ptr<Object3D> object);
  Light& getAmbientLight();

  Scene& setSsaoNoise(std::shared_ptr<Texture> ssaoNoise);
  Scene& setSsaoKernel(std::vector<Vec4> ssaoKernel);

  virtual std::vector< std::tuple<std::string,std::string,int,void*> > getUniforms(std::string passName);
  virtual std::unordered_map< std::string,std::shared_ptr<Texture> > getTextures(std::string passName) const;
};

#endif