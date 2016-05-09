#include <iostream>
#include <memory>
#include "Camera.h"
#include "Scene.h"
#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"

int render(Scene& scene, Camera& camera){
	std::cout << "Rendering " << scene.getObjects().size() << " objects" << std::endl;
  return 0;
}

int main(int argc, char** argv){
  Camera cam;
  Scene scene;
  
  cam.perspectiveCamera(60,4.0/3.0,0.001,100.0);
  cam.getPosition()[2] = 10.0;

  auto geom = std::make_shared<Geometry>(quadGeometry());
  auto mat = std::make_shared<Material>(Material());
  auto obj = std::make_shared<Mesh>(Mesh(geom,mat));

  scene.add(obj);

  render(scene,cam);
  return 0;
}