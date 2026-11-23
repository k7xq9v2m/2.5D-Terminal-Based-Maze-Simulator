#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include "globals.h"

typedef struct{
    float x;
    float y;
} vec2;

float distance(vec2 player, vec2 wall);
vec2 radToVec2(float rad);
float degreesToRadians(float degrees);
float stepFunction(float slope, float position);
float decimal(float number);

#endif



