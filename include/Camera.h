#ifndef CAMERA_H
#define CAMERA_H
#include "Object3D.h"
#include <memory>

class Camera : public Object3D{
private:
  Mat4 projectionMatrix;
  Mat4 worldMatrix;
  std::shared_ptr<Vec4> target;
  float gamma;
public:
  Camera();
  Mat4 getProjectionMatrix()const;
  Mat4 getWorldMatrix()const;
  Camera& updateWorldMatrix();
  std::shared_ptr<Vec4> getTarget()const;
  Camera& setTarget(std::shared_ptr<Vec4> target);
  float getGamma()const;
  Camera& setGamma(float gamma);
  Camera& perspectiveCamera(float fov, float aspectRatio, float near, float far);
  Camera& orthographicCamera(float dist);
};


#endif