#include "Object3D.h"
#include "MathUtils.h"
#include <algorithm>

Object3D::Object3D(){
  this->uuid = generateUUID();
  this->position = Vec4(0.0,0.0,0.0,1.0);
  this->rotation = Euler();
  this->quaternion = Quaternion(0.0,0.0,0.0,1.0);
  this->scale = Vec4(1.0,1.0,1.0,1.0);
  this->modelMatrix = Mat4::identity();
  this->normalModelMatrix = Mat4::identity();
  this->rotation.setQuaternion(&this->quaternion);
  this->quaternion.setEuler(&this->rotation);
  this->parent = nullptr;
  this->children = std::vector< std::shared_ptr<Object3D> >();
  this->rotQuaternions = false;
}

std::string Object3D::getUUID()const{ return uuid; }
Vec4& Object3D::getPosition(){ return position; }
Euler& Object3D::getRotation(){ return rotation; }
Vec4& Object3D::getScale(){ return scale; }
Quaternion& Object3D::getQuaternion(){ return quaternion; }
Mat4 Object3D::getModelMatrix()const { return modelMatrix; }
Mat4 Object3D::getNormalModelMatrix()const { return normalModelMatrix; }

Object3D& Object3D::setPosition(const Vec4 position) {
  this->position = position;
  return *this;
}
Object3D& Object3D::setRotation(const Euler rotation) {
  this->rotation = rotation;
  return *this;
}
Object3D& Object3D::setScale(const Vec4 scale) {
  this->scale = scale;
  return *this;
}
Object3D& Object3D::setQuaternion(const Quaternion quaternion) {
  this->quaternion = quaternion;
  return *this;
}

Object3D& Object3D::updateModelMatrix(){
  Mat4 t = Mat4::translation(position[0],position[1],position[2]);
  Mat4 r;
  if (!rotQuaternions){
    r = Mat4::rotation(rotation[0],rotation[1],rotation[2]);
  }else{
    r = Mat4::rotationFromQuaternion(quaternion);
  }
  Mat4 s = Mat4::scale(scale[0],scale[1],scale[2]);
  Mat4 res;
  if(parent){
    parent->updateModelMatrix();
    res = parent->modelMatrix;
  }else{
    res = Mat4::identity();
  }

  this->modelMatrix = cross(cross(cross(t,r),s),res);
  return *this;
}
Object3D& Object3D::updateNormalModelMatrix(){
  Mat4 r;
  if (!rotQuaternions){
    r = Mat4::rotation(-rotation[0],-rotation[1],-rotation[2]);
  }else{
    r = Mat4::rotationFromQuaternion(inverse(quaternion));
  }
  Mat4 s = Mat4::scale(1.0/scale[0],1.0/scale[1],1.0/scale[2]);
  Mat4 res = Mat4::identity();
  res = cross(cross(r,s),res);
  res = transpose(res);
  if(parent){
    parent->updateNormalModelMatrix();
    res = cross(parent->normalModelMatrix,res);
  }
  this->normalModelMatrix = res;
  return *this;
}

bool Object3D::isVisible()const{ return visible; }
Object3D& Object3D::setVisible(bool visible){
  this->visible = visible;
  return *this;
}
Object3D& Object3D::setParent(const std::shared_ptr<Object3D> parent){
  this->parent = parent.get();
  return *this;
}
Object3D& Object3D::add(const std::shared_ptr<Object3D> child){
  this->children.push_back(child);
  child->parent = this;
  return *this;
}

Object3D& Object3D::remove(const std::shared_ptr<Object3D> child){
  auto it = find(this->children.begin(),this->children.end(), child);
  if(it != this->children.end()){
    this->children.erase(it);
    child->setParent(nullptr);
  }
  return *this;
}

const bool& Object3D::useQuaternions() const{ return rotQuaternions; }
bool& Object3D::useQuaternions(bool useQuaternions){ return rotQuaternions; }

std::vector< std::tuple<std::string,std::string,int,void*> > Object3D::getUniforms(std::string passName){
  std::vector< std::tuple<std::string,std::string,int,void*> > res;
  res.push_back(std::make_tuple("modelMatrix","m4fv",1,modelMatrix.getElements().data()));
  res.push_back(std::make_tuple("normalModelMatrix","m4fv",1,normalModelMatrix.getElements().data()));
  return res;
}