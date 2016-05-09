#include "Scene.h"
#include "MathUtils.h"
#include <algorithm>

Scene::Scene(){
  uuid = generateUUID();
}
std::string Scene::getUUID()const { return uuid; }
const std::vector< std::shared_ptr<Object3D> >& Scene::getObjects()const { return objects; }
const std::vector< std::shared_ptr<Light> >& Scene::getLights()const { return lights; }
Scene& Scene::add(std::shared_ptr<Light> light){
  lights.push_back(light);
  return *this;
}
Scene& Scene::remove(std::shared_ptr<Light> light){
  auto it = std::find(lights.begin(),lights.end(), light);
  if(it != lights.end()){
    lights.erase(it);
  }
  return *this;
}
Scene& Scene::add(std::shared_ptr<Object3D> object){
  objects.push_back(object);
  return *this;
}
Scene& Scene::remove(std::shared_ptr<Object3D> object){
  auto it = std::find(objects.begin(),objects.end(), object);
  if(it != objects.end()){
    objects.erase(it);
  }
  return *this;
}