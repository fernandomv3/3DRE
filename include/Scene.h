#ifndef SCENE_H
#define SCENE_H

#include "Light.h"
#include <memory>
#include <vector>
#include <string>

class Scene{
private:
  std::string uuid;
  std::vector< std::shared_ptr<Object3D> > objects;
  std::vector< std::shared_ptr<Light> > lights;
  Light ambientLight;
  std::vector<float> color;
  std::vector<float> position;
  std::vector<float> attenuation;
  std::vector<float> intensity;
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
  std::vector< std::tuple<std::string,std::string,int,void*> > getUniforms(std::string passName);
};

#endif