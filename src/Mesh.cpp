#include "Mesh.h"

Mesh::Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material):Object3D(){
  this->geometry = geometry;
  this->material = material;
  this->castShadows = true;
}
std::shared_ptr<Geometry> Mesh::getGeometry()const { return geometry; }
std::shared_ptr<Material> Mesh::getMaterial()const { return material; }
Mesh& Mesh::setGeometry(std::shared_ptr<Geometry> geometry){
  this->geometry = geometry;
  return *this;
}
Mesh& Mesh::setMaterial(std::shared_ptr<Material> material){
  this->material = material;
  return *this;
}
bool Mesh::getCastShadows()const { return castShadows; }
Mesh& Mesh::setCastShadows(bool castShadows){
  this->castShadows = castShadows;
  return *this;
}