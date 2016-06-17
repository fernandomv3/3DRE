#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <string>

//Adapted from https://gist.github.com/ne-sachirou/882192
std::string generateUUID();

//clamps from -1 to 1
float clamp(float value);

float lerp(float a, float b, float w);

float random(float min,float max);
#endif