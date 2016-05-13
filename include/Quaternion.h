#ifndef QUATERNION_H
#define QUATERNION_H
#include <array>
#include <iostream>
class Vec4;
class Euler;
class Quaternion{
private:
  std::array<float,4> e;
  Euler* euler;
public:
  Quaternion(float x =0.0, float y=0.0, float z=0.0, float w=0.0);
  float operator[](int i) const;
  float& operator[](int i);
  Quaternion& updateEuler();
  Quaternion& setFromEuler(const Euler& euler , bool update = true);
  Quaternion& conjugate();
  Quaternion& inverse();
  Quaternion& normalize();
  float length() const;

  Quaternion& setEuler(Euler* euler);

  Quaternion& operator*=(const Quaternion& q);
  bool operator==(const Quaternion& q) const;
  
};

Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
float dot(const Quaternion& q1, const Quaternion& q2);
Quaternion rotationBetweenVectors(const Vec4& vec1,const Vec4& vec2);
Quaternion conjugate(const Quaternion& q);
Quaternion inverse(const Quaternion& q);
Quaternion normalize(const Quaternion& q);
std::ostream& operator<<(std::ostream& os, const Quaternion& q);
#endif