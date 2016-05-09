#ifndef EULER_H
#define EULER_H
#include <array>
#include <string>
class Quaternion;

class Euler{
private:
  std::array<float,3> e;
  std::string order;
  Quaternion* quaternion;

public:
  Euler(float x=0.0, float y=0.0, float z=0.0, const std::string order = std::string("XYZ"));
  Euler& setQuaternion(Quaternion* quaternion);
  std::string getOrder()const;
  float operator[](int i) const;
  float& operator[](int i);
  Euler& setFromQuaternion(Quaternion& q, const std::string order, bool update = true);
  Euler& updateQuaternion();
};

#endif