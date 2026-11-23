#ifndef RAY_H
#define RAY_H

#include "globals.h"
#include "math_utils.h"

bool outOfBounds(vec2 coordinates);

vec2 ray(vec2 coordinates, vec2 direction, int maze[]);
void rotate(char direction, float* angle);
void move(char action, vec2* player, vec2 direction, int maze[]);
void wallCollision(vec2* player, vec2 direction, int maze[]);
void drawWall(char icon, int startHeight, int endHeight, int width, int cameraW, char buffer[]);
bool cornerCheck(vec2 hitPoint, int maze[], float hitX, float hitY, vec2 direction);

#endif