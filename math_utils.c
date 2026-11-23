#include "math_utils.h"

float distance(vec2 player, vec2 wall){
    float dx = player.x - wall.x;
    float dy = player.y - wall.y;
    return sqrt(dx * dx + dy * dy);
}

vec2 radToVec2(float rad){
    return (vec2) {(float) cos(rad), (float) sin(rad)};   
} 

float degreesToRadians(float degrees) {
    return degrees * (M_PI / 180.0f);
}


float stepFunction(float slope, float position){
    if(slope > 0){
        return (1 - position) / slope;
    }
    if(slope < 0){
        return - position / slope;
    }
    return INFINITY;
}

float decimal(float number){
    return number - (int) number;
}
