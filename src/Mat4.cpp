#include "Mat4.h"
#include "Vec4.h"
#include "Quaternion.h"
#include <algorithm>

Mat4::Mat4(float value){ e.fill(value); }
Mat4::Mat4(const std::array<float,16>& el){ e = el; }

const float& Mat4::operator[](int i) const{ return e[i]; }
float& Mat4::operator[](int i){ return e[i]; }

Mat4& Mat4::operator*=(float t){
  for(int i=0; i< 16; i++) e[i]*=t;
  return *this;
}
Mat4& Mat4::operator*=(const Mat4& m2){
  for(int i=0; i< 16; i++) e[i]*= m2.e[i];
  return *this;
}

bool Mat4::operator==(const Mat4& m2)const{
  bool equal = true;
  for(int i=0; i< 16; i++) equal = equal && (e[i] == m2.e[i]);
  return equal;
}

Mat4 Mat4::identity(){
  Mat4 res;
  res[0] = 1.0;
  res[5] = 1.0;
  res[10] = 1.0;
  res[15] = 1.0;
  return res;
}
Mat4 Mat4::translation(float x, float y , float z){
  Mat4 mat = Mat4::identity();
  mat[12] =x;
  mat[13] =y;
  mat[14] =z;
  return mat;
}
Mat4 Mat4::scale(float x, float y , float z){
  Mat4 mat = Mat4::identity();
  mat[0] =x;
  mat[5] =y;
  mat[10] =z;
  return mat;
}
Mat4 Mat4::rotation(float x , float y, float z){
  float rX = 3.14159 * x /180.0;
  float rY = 3.14159 * y /180.0;
  float rZ = 3.14159 * z /180.0;

  Mat4 rotX = Mat4::identity();
  rotX[5] = cos(rX);
  rotX[6] = -sin(rX);
  rotX[9] = sin(rX);
  rotX[10] = cos(rX);

  Mat4 rotY = Mat4::identity();
  rotY[0] = cos(rY);
  rotY[2] = -sin(rY);
  rotY[8] = sin(rY);
  rotY[10] = cos(rY);

  Mat4 rotZ = Mat4::identity();
  rotZ[0] = cos(rZ);
  rotZ[1] = sin(rZ);
  rotZ[4] = -sin(rZ);
  rotZ[5] = cos(rZ);
  return cross(cross(rotX,rotY),rotZ);
}
Mat4 Mat4::perspective(float fov, float aspectRatio, float zNear, float zFar){
  Mat4 mat;
  float radians = fov * (3.14159f / 180.0f);
  mat[0] = (1 / tan(radians));
  mat[5] = aspectRatio * mat[0];
  mat[10] = ((zFar +zNear)/(zNear -zFar));
  mat[11] = -1.0;
  mat[14] = ((2.0 * zNear * zFar)/(zNear -zFar));
  return mat;
}
Mat4 Mat4::orthographic(float left,float right,float top,float bottom,float near,float far){
  Mat4 mat = Mat4::identity();
  
  float w = right - left;
  float h = top - bottom;
  float p = far - near;

  float x = (right + left)/w;
  float y = (top + bottom)/h;
  float z = (far + near)/p;

  mat[0]= 2.0f/w;
  mat[5]= 2.0f/h;
  mat[10]= -2.0f/p;
  mat[12]= -x;
  mat[13]= -y;
  mat[14]= -z;

  return mat;
}
Mat4 Mat4::lookAt(const Vec4& eye, const Vec4& target,const Vec4& up){
  Vec4 z = unitVector(eye - target);
  if(!z.length()) z[2] = 1.0;
  Vec4 x = unitVector(cross(up,z));
  Vec4 y = cross(z,x);

  Mat4 res = Mat4::identity();
  res[0] = x[0];
  res[4] = x[1];
  res[8] = x[2];

  res[1] = y[0];
  res[5] = y[1];
  res[9] = y[2];

  res[2] = z[0];
  res[6] = z[1];
  res[10] = z[2];
  return res;
}

Mat4 Mat4::rotationFromQuaternion(const Quaternion& q){
  Mat4 res = Mat4::identity();
  res[0] = 1 - (2*q[1]*q[1]) -(2*q[2]*q[2]);
  res[1] = (2*q[0]*q[1]) + (2*q[3]*q[2]);
  res[2] = (2*q[0]*q[2]) - (2*q[3]*q[1]);

  res[4] = (2*q[0]*q[1]) - (2*q[3]*q[2]);
  res[5] = 1 - (2*q[0]*q[0]) -(2*q[2]*q[2]);
  res[6] = (2*q[1]*q[2]) + (2*q[3]*q[0]);

  res[8] = (2*q[0]*q[2]) + (2*q[3]*q[1]);
  res[9] = (2*q[1]*q[2]) - (2*q[3]*q[0]);
  res[10] = 1 - (2*q[0]*q[0]) -(2*q[1]*q[1]);

  return res;
}

std::array<float,16>& Mat4::getElements(){
  return this->e;
}

Mat4 operator*(const Mat4& m1, const Mat4& m2){
  Mat4 res(m1);
  return res*=m2;
}
Mat4 operator*(float t,const Mat4& m){
  Mat4 res(m);
  return res*=t;
}
Mat4 operator*(const Mat4& m,float t){
  Mat4 res(m);
  return res*=t;
}
Mat4 cross(const Mat4& m1, const Mat4& m2){
  Mat4 res;
  res[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
  res[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
  res[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
  res[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

  res[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
  res[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
  res[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
  res[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

  res[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
  res[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
  res[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
  res[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

  res[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
  res[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
  res[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
  res[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
  return res;
}
Mat4 transpose(const Mat4& m){
  Mat4 result;
  for(int i=0; i < 4; i++){
    for(int j=0; j < 4 ; j++){
      result[i*4+j] =m[j*4+i];
    }
  }
  return result;  
}

std::ostream& operator<<(std::ostream& os, const Mat4& m){
  for(int i = 0; i < 4; i++){
    os << m[0+(i*4)] << " " << m[1+(i*4)] << " " << m[2+(i*4)] << " " << m[3+(i*4)] << std::endl;
  }
  return os;
}