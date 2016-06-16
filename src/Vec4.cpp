#include "Vec4.h"
#include <cmath>

Vec4::Vec4(float x, float y, float z, float w){
  e[0] = x; e[1]=y; e[2]=z; e[3]=w;
}

std::array<float,4>& Vec4::getElements(){ return this->e; }

const Vec4& Vec4::operator+() const { return *this; }
Vec4 Vec4::operator-() const { return Vec4(-e[0], -e[1], -e[2], -e[3]); }
float Vec4::operator[](int i) const { return e[i]; }
float& Vec4::operator[](int i) { return e[i]; }
bool Vec4::operator==(const Vec4& v2){
  return (e[0]==(v2.e[0])) && (e[1]==(v2.e[1])) && (e[2]==(v2.e[2])) && (e[3]==(v2.e[3]));
}

Vec4& Vec4::operator+=(const Vec4& v2){
  e[0] += v2.e[0]; e[1] += v2.e[1]; e[2] += v2.e[2]; e[3] += v2.e[3]; 
  return *this;
}
Vec4& Vec4::operator-=(const Vec4& v2){
  e[0] -= v2.e[0]; e[1] -= v2.e[1]; e[2] -= v2.e[2]; e[3] -= v2.e[3]; 
  return *this;
}
Vec4& Vec4::operator*=(const Vec4& v2){
  e[0] *= v2.e[0]; e[1] *= v2.e[1]; e[2] *= v2.e[2]; e[3] *= v2.e[3]; 
  return *this;
}
Vec4& Vec4::operator/=(const Vec4& v2){
  e[0] /= v2.e[0]; e[1] /= v2.e[1]; e[2] /= v2.e[2]; e[3] /= v2.e[3]; 
  return *this;
}
Vec4& Vec4::operator*=(const float t){
  e[0] *= t; e[1] *= t; e[2] *= t; e[3] *= t; 
  return *this;
}
Vec4& Vec4::operator/=(const float t){
  e[0] /= t; e[1] /= t; e[2] /= t; e[3] /= t; 
  return *this;
}

float Vec4::length() const{
  return sqrt(this->squaredLength());
}
float Vec4::squaredLength() const{
  return e[0]*e[0] + e[1]*e[1] + e[2]*e[2] +e[3]*e[3];
}
Vec4& Vec4::normalize(){
  *this /= this->length(); 
  return *this;
}

Vec4 operator+(const Vec4& v1, const Vec4& v2){
  return Vec4(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2], v1[3] + v2[3]);
}
Vec4 operator-(const Vec4& v1, const Vec4& v2){
  return Vec4(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2], v1[3] - v2[3]);
}
Vec4 operator*(const Vec4& v1, const Vec4& v2){
  return Vec4(v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2], v1[3] * v2[3]);
}
Vec4 operator/(const Vec4& v1, const Vec4& v2){
  return Vec4(v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2], v1[3] / v2[3]);
}
Vec4 operator+(const Vec4& v1, const float t){
  return Vec4(v1[0] + t, v1[1] + t, v1[2] + t, v1[3] + t);
}
Vec4 operator-(const Vec4& v1, const float t){
  return Vec4(v1[0] - t, v1[1] - t, v1[2] - t, v1[3] - t);
}
Vec4 operator*(const Vec4& v1, const float t){
  return Vec4(v1[0] * t, v1[1] * t, v1[2] * t, v1[3] * t);
}
Vec4 operator/(const Vec4& v1, const float t){
  return Vec4(v1[0] / t, v1[1] / t, v1[2] / t, v1[3] / t);
}
Vec4 operator+(const float t, const Vec4& v1){
  return Vec4(v1[0] + t, v1[1] + t, v1[2] + t, v1[3] + t);
}
Vec4 operator-(const float t, const Vec4& v1){
  return Vec4(t - v1[0], t - v1[1], t - v1[2], t - v1[3]);
}
Vec4 operator*(const float t, const Vec4& v1){
  return Vec4(v1[0] * t, v1[1] * t, v1[2] * t, v1[3] * t);
}
Vec4 operator/(const float t, const Vec4& v1){
  return Vec4(t / v1[0], t / v1[1], t / v1[2], t / v1[3]);
}

float distance(const Vec4& v1, const Vec4& v2){
  return (v1 - v2).length();
}
float dot(const Vec4& v1, const Vec4& v2){
  return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] + v1[3]*v2[3];
}
Vec4 cross(const Vec4& v1, const Vec4& v2){
  return Vec4(v1[1] * v2[2] - v1[2] * v2[1],
              v1[2] * v2[0] - v1[0] * v2[2],
              v1[0] * v2[1] - v1[1] * v2[0]);
}
Vec4 unitVector(const Vec4& v){
  return v / v.length();
}

std::ostream& operator<<(std::ostream& os, const Vec4& v){
  os << v[0] << " " << v[1] << " " << v[2] << " " << v[3];
  return os;
}