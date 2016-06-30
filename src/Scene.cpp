#include "Scene.h"
#include "MathUtils.h"
#include <algorithm>

Scene::Scene(){
  uuid = generateUUID();
  ambientLight = Light(Vec4(0.01,0.01,0.01,1.0));
  ssaoNoise = nullptr;
  ssaoRadius = 1.0;
}
std::string Scene::getUUID()const { return uuid; }
const std::vector< std::shared_ptr<Object3D> >& Scene::getObjects()const { return objects; }
const std::vector< std::shared_ptr<Light> >& Scene::getLights()const { return lights; }

Scene& Scene::setSsaoRadius(float radius){
  this->ssaoRadius = radius;
  return *this;
}
Scene& Scene::setSsaoNoise(std::shared_ptr<Texture> ssaoNoise){
  this->ssaoNoise = ssaoNoise;
  return *this;
}
Scene& Scene::setSsaoKernel(std::vector<Vec4> ssaoKernel){
  this->ssaoKernel = ssaoKernel;
  this->ssaoKernelSize = ssaoKernel.size();
  return *this;
}

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

std::vector< std::tuple<std::string,std::string,int,void*> > Scene::getUniforms(std::string passName){
  std::vector< std::tuple<std::string,std::string,int,void*> > res;
  if(passName == "forward"){
    int numLights = lights.size();
    std::vector<float> c;
    c.reserve(numLights*4);
    std::vector<float> p;
    p.reserve(numLights*4);
    std::vector<float> m;
    m.reserve(numLights*16);
    std::vector<float> a;
    a.reserve(numLights);
    std::vector<float> i;
    i.reserve(numLights);

    for(auto light : lights){
      auto lightMatrixElements = light->getLightMatrix().getElements();
      c.insert(std::end(c), light->getColor().getElements().begin(), light->getColor().getElements().end());
      p.insert(std::end(p), light->getPosition().getElements().begin(), light->getPosition().getElements().end());
      m.insert(std::end(m), lightMatrixElements.begin(),lightMatrixElements.end());
      a.push_back(light->getAttenuation());
      i.push_back(light->getIntensity());
    }
    color.swap(c);
    position.swap(p);
    attenuation.swap(a);
    intensity.swap(i);
    matrices.swap(m);
    if(!color.empty()) res.push_back(std::make_tuple("light.color","4fv",numLights,color.data()));
    if(!position.empty())res.push_back(std::make_tuple("light.position","4fv",numLights,position.data()));
    if(!attenuation.empty())res.push_back(std::make_tuple("light.attenuation","1f",numLights,attenuation.data()));
    if(!intensity.empty())res.push_back(std::make_tuple("light.intensity","1f",numLights,intensity.data()));
    if(!matrices.empty())res.push_back(std::make_tuple("light.depthMatrix","m4fv",numLights,matrices.data()));
  }
  res.push_back(std::make_tuple("ambient","4f",1,static_cast<void*>(ambientLight.getColor().getElements().data())));
  
  int kernelSize = ssaoKernel.size();
  std::vector<float> k;
  k.reserve(kernelSize *4);
  for(auto v : ssaoKernel){
    k.push_back(v[0]);
    k.push_back(v[1]);
    k.push_back(v[2]);
    k.push_back(v[3]);
  }
  flatSsaoKernel.swap(k);
  if(!flatSsaoKernel.empty()){
    res.push_back(std::make_tuple("ssaoKernel","4fv",kernelSize,flatSsaoKernel.data()));
    res.push_back(std::make_tuple("ssaoKernelSize","1i",1,&ssaoKernelSize));
    res.push_back(std::make_tuple("ssaoRadius","1f",1,&ssaoRadius));
  }
  
  return res;
}

std::unordered_map< std::string,std::shared_ptr<Texture> > Scene::getTextures(std::string passName) const{
  auto result = std::unordered_map< std::string,std::shared_ptr<Texture> >();
  if (passName == "shadow") return result; 
  if(ssaoNoise != nullptr){
    result["ssaoNoise"] = ssaoNoise;
  }
  return result;
}