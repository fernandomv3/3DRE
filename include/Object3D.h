#ifndef OBJECT3D_H
#define OBJECT3D_H
#include "Vec4.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "Euler.h"
#include <memory>
#include <string>
#include <vector>

class Object3D{
private:
  std::string uuid;
  Vec4 position;
  Euler rotation;
  Quaternion quaternion;
  Vec4 scale;
  Mat4 modelMatrix;
  bool visible;
  Object3D* parent;
  std::vector< std::shared_ptr<Object3D> > children;
  bool rotQuaternions;
public:
  Object3D();
  std::string getUUID()const;
  Vec4& getPosition();
  Euler& getRotation();
  Vec4& getScale();
  Quaternion& getQuaternion();
  Object3D& setPosition(const Vec4 position);
  Object3D& setRotation(const Euler rotation);
  Object3D& setScale(const Vec4 scale);
  Object3D& setQuaternion(const Quaternion quaternion);
  Mat4 getModelMatrix()const;
  Object3D& updateModelMatrix();
  bool isVisible()const;
  Object3D& setVisible(bool visible);
  Object3D& setParent(const std::shared_ptr<Object3D> parent);
  Object3D& add(const std::shared_ptr<Object3D> child);
  Object3D& remove(const std::shared_ptr<Object3D> child);
  const bool& useQuaternions() const;
  bool& useQuaternions(bool quaternions);
};
#endif