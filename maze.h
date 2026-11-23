#ifndef MAZE_H
#define MAZE_H 

#include "globals.h"

#define HORIZONTAL true
#define VERTICAL false


void moveMaze(int maze[], int pattern[], int gridWidth, int gridHeight, int startTile, int endTile);
bool chooseOrientationHorizontal(int width, int height);
void divide(int grid[], bool openings[], int x, int y, int width, int height, bool orientation, int gridWidth);
void generateMaze(int maze[], int startTile, int endTile, int iteration);

void insertMaze(int maze[], int width, int height, char buffer[], int player);
void printMaze(int maze[], int width, int height, int iteration);
void printMazeOnScreen(int maze[], int width, int height);
void printSideToSides(char vision[], char layout[]);


#endif 