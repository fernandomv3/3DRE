#ifndef MAT4_H
#define MAT4_H

#include <array>

class Vec4;
class Quaternion;
class Mat4{
private:
  std::array<float,16> e;
public:
  Mat4(float value = 0.0);
  Mat4(const std::array<float,16>& el);

  const float& operator[](int i) const;
  float& operator[](int i);
  
  static Mat4 identity();
  static Mat4 translation(float x, float y , float z);
  static Mat4 scale(float x, float y , float z);
  static Mat4 rotation(float x , float y, float z);
  static Mat4 perspective(float fov, float aspectRatio, float zNear, float zFar);
  static Mat4 orthographic(float left,float right,float top,float bottom,float near,float far);
  static Mat4 lookAt(const Vec4& eye, const Vec4& target,const Vec4& up);
  static Mat4 rotationFromQuaternion(const Quaternion& q);

  Mat4& operator*=(float t);
  Mat4& operator*=(const Mat4& m2);

  bool operator==(const Mat4& m2)const;
};

Mat4 operator*(float t,const Mat4& m);
Mat4 operator*(const Mat4& m,float t);
Mat4 operator*(const Mat4& m1, const Mat4& m2);
Mat4 cross(const Mat4& m1, const Mat4& m2);
Mat4 transpose(const Mat4& m);

#endif