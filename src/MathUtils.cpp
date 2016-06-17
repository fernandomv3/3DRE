#include "MathUtils.h"
#include <cstdlib>
#include <cmath>

std::string generateUUID(){
  static const std::string CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  std::string uuid = std::string(36,' ');
  int rnd = 0;
  int r = 0;
  
  uuid[8] = '-';
  uuid[13] = '-';
  uuid[18] = '-';
  uuid[23] = '-';

  uuid[14] = '4';

  for(int i=0;i<36;i++){
    if (i != 8 && i != 13 && i != 18 && i != 14 && i != 23) {
      if (rnd <= 0x02) {
          rnd = (0x2000000 + static_cast <int> (random(0.0,1.0) * 0x1000000)) | 0;
      }
      rnd >>= 4;
      r = rnd & 0xf;
      uuid[i] = CHARS[(i == 19) ? (r & 0x3) | 0x8 : r];
    }
  }
  return uuid;
}

float clamp(float value){
  return fmin(fmax(value,-1.0f),1.0f);
}

float lerp(float a, float b, float w){
  return a + w*(b-a);
}

float random(float min, float max){
  return min + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(max-min)));
}