#include "Scene.h"
#include "MathUtils.h"
#include <algorithm>

Scene::Scene(){
  uuid = generateUUID();
  ambientLight = Light(Vec4(0.0,0.0,1.0,1.0));
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

Light& Scene::getAmbientLight(){ return ambientLight; }

std::vector< std::tuple<std::string,std::string,int,void*> > Scene::getUniforms(){
  std::vector< std::tuple<std::string,std::string,int,void*> > res;
  std::vector<float> color;
  std::vector<float> position;
  std::vector<float> attenuation;
  int numLights = lights.size();
  color.reserve(numLights*4);
  position.reserve(numLights*4);
  attenuation.reserve(numLights);
  for(auto light : lights){
    color.insert(std::end(color), std::begin(light->getColor().getElements()), std::end(light->getColor().getElements()));
    position.insert(std::end(position), std::begin(light->getPosition().getElements()), std::end(light->getPosition().getElements()));
    attenuation.push_back(light->getAttenuation());
  }
  if(!color.empty()) res.push_back(std::make_tuple("light.color","4fv",numLights,color.data()));
  if(!position.empty())res.push_back(std::make_tuple("light.position","4fv",numLights,position.data()));
  if(!attenuation.empty())res.push_back(std::make_tuple("light.attenuation","1f",numLights,attenuation.data()));
  res.push_back(std::make_tuple("ambient","4f",1,static_cast<void*>(ambientLight.getColor().getElements().data())));
  return res;
}