#ifndef MESH_H
#define MESH_H

#include "Object3D.h"
#include "Geometry.h"
#include "Material.h"
#include <memory>

class Mesh : public Object3D{
private:
  std::shared_ptr<Geometry> geometry;
  std::shared_ptr<Material> material;
  bool castShadows;
public:
  Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);
  std::shared_ptr<Geometry> getGeometry()const;
  std::shared_ptr<Material> getMaterial()const;
  Mesh& setGeometry(std::shared_ptr<Geometry> geometry);
  Mesh& setMaterial(std::shared_ptr<Material> material);
  bool getCastShadows()const;
  Mesh& setCastShadows(bool castShadows);
};

#endif