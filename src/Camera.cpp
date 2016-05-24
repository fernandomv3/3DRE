#include "Camera.h"

Camera::Camera():Object3D(){
  this->projectionMatrix = Mat4::identity();
  this->worldMatrix = Mat4::identity();
  this->gamma = 2.2;
}
Mat4 Camera::getProjectionMatrix()const{ return projectionMatrix; }
Mat4 Camera::getWorldMatrix()const{ return worldMatrix; }
Camera& Camera::updateWorldMatrix(){
  Vec4 pos = this->getPosition();
  Vec4 scale = this->getScale();
  Mat4 t = Mat4::translation(pos[0] * -1,pos[1] * -1,pos[2] * -1);
  Mat4 r;
  if(!this->target){
    Quaternion q = inverse(this->getQuaternion());
    r = Mat4::rotationFromQuaternion(q);
  }
  else{
    r = Mat4::lookAt(pos, *target,Vec4(0.0,1.0,0.0,0.0));
  }
  Mat4 s = Mat4::scale(1 / scale[0], 1 / scale[1], 1 / scale[2]);
  Mat4 res = Mat4::identity();
  this->worldMatrix = cross(cross(cross(res,s),r),t);
  return *this;
}
std::shared_ptr<Vec4> Camera::getTarget()const{ return target; }
Camera& Camera::setTarget(std::shared_ptr<Vec4> target){
  this->target = target;
  return *this;
}
float Camera::getGamma()const{ return gamma; }
Camera& Camera::setGamma(float gamma){
  this->gamma = gamma;
  return *this;
}
Camera& Camera::perspectiveCamera(float fov, float aspectRatio, float near, float far){
  projectionMatrix = Mat4::perspective(fov, aspectRatio, near, far);
  return *this;
}

Camera& Camera::orthographicCamera(float dist){
  projectionMatrix = Mat4::orthographic(-dist,dist,dist,-dist,dist,-dist);
  return *this;
}

std::vector< std::tuple<std::string,std::string,int,void*> > Camera::getUniforms(){
  std::vector< std::tuple<std::string,std::string,int,void*> > res;
  res.push_back(std::make_tuple("projectionMatrix","m4fv",1,projectionMatrix.getElements().data()));
  res.push_back(std::make_tuple("worldMatrix","m4fv",1,worldMatrix.getElements().data()));
  return res;
}