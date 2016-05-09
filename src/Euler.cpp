#include "Euler.h"
#include "Quaternion.h"
#include "MathUtils.h"
#include <cmath>

Euler::Euler(float x, float y, float z, const std::string order){
  e[0] = x;
  e[1] = y;
  e[2] = z;
  this->order = order;
  this->quaternion = nullptr;
}

Euler& Euler::setQuaternion(Quaternion* quaternion){
  this->quaternion = quaternion;
  return *this;
}

std::string Euler::getOrder()const{ return order; }
float Euler::operator[](int i) const { return e[i]; }
float& Euler::operator[](int i) { return e[i]; }
Euler& Euler::setFromQuaternion(Quaternion& q, const std::string order, bool update){
  float x2 = q[0] * q[0], 
        y2 = q[1] * q[1], 
        z2 = q[2] * q[2], 
        w2 = q[3] * q[3];

  if (order == std::string("XYZ")){
    e[0] = atan2( 2 * ( q[0] * q[3] - q[1] * q[2] ), ( w2 - x2 - y2 + z2 ) );
    e[1] = asin(  clamp( 2 * ( q[0] * q[2] + q[1] * q[3] ) ) );
    e[2] = atan2( 2 * ( q[2] * q[3] - q[0] * q[1] ), ( w2 + x2 - y2 - z2 ) );

  }
  else if (order == std::string("YXZ")){
    e[0] = asin(  clamp( 2 * ( q[0] * q[3] - q[1] * q[2] ) ) );
    e[1] = atan2( 2 * ( q[0] * q[2] + q[1] * q[3] ), ( w2 - x2 - y2 + z2 ) );
    e[2] = atan2( 2 * ( q[0] * q[1] + q[2] * q[3] ), ( w2 - x2 + y2 - z2 ) );
  }
  else if (order == std::string("ZXY")){
    e[0] = asin(  clamp( 2 * ( q[0] * q[3] + q[1] * q[2] ) ) );
    e[1] = atan2( 2 * ( q[1] * q[3] - q[2] * q[0] ), ( w2 - x2 - y2 + z2 ) );
    e[2] = atan2( 2 * ( q[2] * q[3] - q[0] * q[1] ), ( w2 - x2 + y2 - z2 ) );
  }
  else if (order == std::string("ZYX")){
    e[0] = atan2( 2 * ( q[0] * q[3] + q[2] * q[1] ), ( w2 - x2 - y2 + z2 ) );
    e[1] = asin(  clamp( 2 * ( q[1] * q[3] - q[0] * q[2] ) ) );
    e[2] = atan2( 2 * ( q[0] * q[1] + q[2] * q[3] ), ( w2 + x2 - y2 - z2 ) );
  }
  else if (order == std::string("YZX")){
    e[0] = atan2( 2 * ( q[0] * q[3] - q[2] * q[1] ), ( w2 - x2 + y2 - z2 ) );
    e[1] = atan2( 2 * ( q[1] * q[3] - q[0] * q[2] ), ( w2 + x2 - y2 - z2 ) );
    e[2] = asin(  clamp( 2 * ( q[0] * q[1] + q[2] * q[3] ) ) );
  }
  else if (order == std::string("XZY")){
    e[0] = atan2( 2 * ( q[0] * q[3] + q[1] * q[2] ), ( w2 - x2 + y2 - z2 ) );
    e[1] = atan2( 2 * ( q[0] * q[2] + q[1] * q[3] ), ( w2 + x2 - y2 - z2 ) );
    e[2] = asin(  clamp( 2 * ( q[2] * q[3] - q[0] * q[1] ) ) );
  }


  this->order = order;
  if(update){
    this->updateQuaternion();
  }
  return *this;
}
Euler& Euler::updateQuaternion(){
  quaternion->setFromEuler(*this , false);
  return *this;
}
