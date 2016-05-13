#include "Quaternion.h"
#include "Vec4.h"
#include "Euler.h"
#include <cmath>

Quaternion::Quaternion(float x, float y, float z, float w){
  e[0] = x; e[1] = y; e[2] = z; e[3] = w;
  this->euler = nullptr;
}
float Quaternion::operator[](int i) const { return e[i]; }
float& Quaternion::operator[](int i) { return e[i]; }
Quaternion& Quaternion::updateEuler(){
  if(euler != nullptr)
    euler->setFromQuaternion(*this,euler->getOrder(),false);
  return *this;
}
Quaternion& Quaternion::setFromEuler(const Euler& euler , bool update){
  float c1 = cos( euler[0] / 2 ),
        c2 = cos( euler[1] / 2 ),
        c3 = cos( euler[2] / 2 ),
        s1 = sin( euler[0] / 2 ),
        s2 = sin( euler[1] / 2 ),
        s3 = sin( euler[2] / 2 );

  if ( euler.getOrder() == std::string("XYZ") ) {
      e[0] = s1 * c2 * c3 + c1 * s2 * s3;
      e[1] = c1 * s2 * c3 - s1 * c2 * s3;
      e[2] = c1 * c2 * s3 + s1 * s2 * c3;
      e[3] = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.getOrder() == std::string("YXZ") ) {
      e[0] = s1 * c2 * c3 + c1 * s2 * s3;
      e[1] = c1 * s2 * c3 - s1 * c2 * s3;
      e[2] = c1 * c2 * s3 - s1 * s2 * c3;
      e[3] = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( euler.getOrder() == std::string("ZXY") ) {
      e[0] = s1 * c2 * c3 - c1 * s2 * s3;
      e[1] = c1 * s2 * c3 + s1 * c2 * s3;
      e[2] = c1 * c2 * s3 + s1 * s2 * c3;
      e[3] = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.getOrder() == std::string("ZYX") ) {
      e[0] = s1 * c2 * c3 - c1 * s2 * s3;
      e[1] = c1 * s2 * c3 + s1 * c2 * s3;
      e[2] = c1 * c2 * s3 - s1 * s2 * c3;
      e[3] = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( euler.getOrder() == std::string("YZX") ) {
      e[0] = s1 * c2 * c3 + c1 * s2 * s3;
      e[1] = c1 * s2 * c3 + s1 * c2 * s3;
      e[2] = c1 * c2 * s3 - s1 * s2 * c3;
      e[3] = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.getOrder() == std::string("XZY") ) {
      e[0] = s1 * c2 * c3 - c1 * s2 * s3;
      e[1] = c1 * s2 * c3 - s1 * c2 * s3;
      e[2] = c1 * c2 * s3 + s1 * s2 * c3;
      e[3] = c1 * c2 * c3 + s1 * s2 * s3;
  }

  if (update){
    this->updateEuler();
  }
  return *this;
}
Quaternion& Quaternion::conjugate(){
  e[0] *= -1; e[1] *= -1; e[2] *= -1;
  return *this;
}
Quaternion& Quaternion::inverse(){
  return this->conjugate().normalize();
}
Quaternion& Quaternion::normalize(){
  e[0] /= this->length(); e[1] /= this->length(); e[2] /= this->length(); e[3] /= this->length();
  return *this;
}
float Quaternion::length() const{
  return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2] + e[3]*e[3]);
}

Quaternion& Quaternion::setEuler(Euler* euler){
  this->euler= euler;
  return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& q){
  e[0] = e[0] * q.e[3] + e[3] * q.e[0] + e[1] * q.e[2] - e[2] * q.e[1];
  e[1] = e[1] * q.e[3] + e[3] * q.e[1] + e[2] * q.e[0] - e[0] * q.e[2];
  e[2] = e[2] * q.e[3] + e[3] * q.e[2] + e[0] * q.e[1] - e[1] * q.e[0];
  e[3] = e[3] * q.e[3] - e[0] * q.e[0] - e[1] * q.e[1] - e[2] * q.e[2];
  this->updateEuler();
  return *this;
}
bool Quaternion::operator==(const Quaternion& q) const{
  return (e[0] == q.e[0]) && (e[1] == q.e[1]) && (e[2] == q.e[2]) && (e[3] == q.e[3]);
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2){
  Quaternion res;
  res[0] = q1[0] * q2[3] + q1[3] * q2[0] + q1[1] * q2[2] - q1[2] * q2[1];
  res[1] = q1[1] * q2[3] + q1[3] * q2[1] + q1[2] * q2[0] - q1[0] * q2[2];
  res[2] = q1[2] * q2[3] + q1[3] * q2[2] + q1[0] * q2[1] - q1[1] * q2[0];
  res[3] = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];
  return res;
}
float dot(const Quaternion& q1, const Quaternion& q2){
  return q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3];
}
Quaternion rotationBetweenVectors(const Vec4& vec1,const Vec4& vec2){
  float e = dot(vec1,vec2);
  Vec4 vec = cross(vec1,vec2);
  float term = sqrt(2*(1+e));
  return Quaternion(vec[0] * 1.0f/term, vec[1] * 1.0f/term, vec[2] * 1.0f/term, term / 2.0f);
}
Quaternion conjugate(const Quaternion& q){
  Quaternion res(q);
  return res.conjugate();
}
Quaternion inverse(const Quaternion& q){
  Quaternion res(q);
  return res.inverse();
}
Quaternion normalize(const Quaternion& q){
  Quaternion res(q);
  return res.normalize();
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q){
  os << q[0] << " " << q[1] << " " << q[2] << " " << q[3];
  return os;
}