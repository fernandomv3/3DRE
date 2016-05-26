#include "Scene.h"
#include "MathUtils.h"
#include <algorithm>

Scene::Scene(){
  uuid = generateUUID();
  ambientLight = Light(Vec4(0.2,0.2,0.2,1.0));
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
  int numLights = lights.size();
  std::vector<float> c;
  c.reserve(numLights*4);
  std::vector<float> p;
  p.reserve(numLights*4);
  std::vector<float> a;
  a.reserve(numLights);
  std::vector<float> i;
  i.reserve(numLights);
  for(auto light : lights){
    c.insert(std::end(c), std::begin(light->getColor().getElements()), std::end(light->getColor().getElements()));
    p.insert(std::end(p), std::begin(light->getPosition().getElements()), std::end(light->getPosition().getElements()));
    a.push_back(light->getAttenuation());
    i.push_back(light->getIntensity());
  }
  color.swap(c);
  position.swap(p);
  attenuation.swap(a);
  intensity.swap(i);
  if(!color.empty()) res.push_back(std::make_tuple("light.color","4fv",numLights,color.data()));
  if(!position.empty())res.push_back(std::make_tuple("light.position","4fv",numLights,position.data()));
  if(!attenuation.empty())res.push_back(std::make_tuple("light.attenuation","1f",numLights,attenuation.data()));
  if(!intensity.empty())res.push_back(std::make_tuple("light.intensity","1f",numLights,intensity.data()));
  res.push_back(std::make_tuple("ambient","4f",1,static_cast<void*>(ambientLight.getColor().getElements().data())));
  return res;
}