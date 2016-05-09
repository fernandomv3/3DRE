#ifndef VEC4H
#define VEC4H

#include <array>

class Vec4{
private:
  std::array<float,4> e;
public:
  Vec4(float x = 0.0, float y = 0.0, float z = 0.0, float w = 0.0);

  const Vec4& operator+() const;
  Vec4 operator-() const;

  float operator[](int i) const;
  float& operator[](int i);
  bool operator==(const Vec4& v2);

  Vec4& operator+=(const Vec4& v2);
  Vec4& operator-=(const Vec4& v2);
  Vec4& operator*=(const Vec4& v2);
  Vec4& operator/=(const Vec4& v2);
  Vec4& operator*=(const float t);
  Vec4& operator/=(const float t);

  float length() const;
  float squaredLength() const;
  Vec4& normalize();
};

Vec4 operator+(const Vec4& v1, const Vec4& v2);
Vec4 operator-(const Vec4& v1, const Vec4& v2);
Vec4 operator*(const Vec4& v1, const Vec4& v2);
Vec4 operator/(const Vec4& v1, const Vec4& v2);

Vec4 operator+(const Vec4& v1, const float t);
Vec4 operator-(const Vec4& v1, const float t);
Vec4 operator*(const Vec4& v1, const float t);
Vec4 operator/(const Vec4& v1, const float t);

Vec4 operator+(const float t, const Vec4& v1);
Vec4 operator-(const float t, const Vec4& v1);
Vec4 operator*(const float t, const Vec4& v1);
Vec4 operator/(const float t, const Vec4& v1);

float distance(const Vec4& v1, const Vec4& v2);
float dot(const Vec4& v1, const Vec4& v2);
Vec4 cross(const Vec4& v1, const Vec4& v2);
Vec4 unitVector(const Vec4& v);

#endif