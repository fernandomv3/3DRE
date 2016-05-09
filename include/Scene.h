#ifndef SCENE_H
#define SCENE_H

#include "Light.h"
#include "Object3D.h"
#include <memory>
#include <vector>
#include <string>

class Scene{
private:
  std::string uuid;
  std::vector< std::shared_ptr<Object3D> > objects;
  std::vector< std::shared_ptr<Light> > lights;
public:
  Scene();
  std::string getUUID()const;
  const std::vector< std::shared_ptr<Object3D> >& getObjects()const;
  const std::vector< std::shared_ptr<Light> >& getLights()const;
  Scene& add(std::shared_ptr<Light> light);
  Scene& remove(std::shared_ptr<Light> light);
  Scene& add(std::shared_ptr<Object3D> object);
  Scene& remove(std::shared_ptr<Object3D> object);
};

#endif