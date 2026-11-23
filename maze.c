#include "maze.h"
//move pattern into maze array, while creating outer walls
void moveMaze(int maze[], int pattern[], int gridWidth, int gridHeight, int startTile, int endTile){
    int index = 0;
    for(int i = 0; i < mazeHeight; i++){
        for(int j = 0; j < mazeWidth; j++){
            if(i == 0 || i == (mazeHeight - 1) || j == 0 || j == (mazeWidth - 1)) maze[i * mazeWidth + j] = 1;
        }
    }
    maze[startTile] = 0;
    maze[endTile] = 0;
    for(int i = 1; i < mazeHeight - 1; i++){
        for(int j = 1; j < mazeWidth - 1 ; j++){
            maze[i * mazeWidth + j] = pattern[index];
            index++;
        }
    }
}

//for rendering maze layout on screen, spaces between each column for better ratio of width to height of terminal line break
void insertMaze(int maze[], int width, int height, char buffer[], int player){
    int index = 0;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if((i * width + j) == player) buffer[index] = 'P';
            else if(maze[i * width + j] == 1)buffer[index] = '*';
            else buffer[index] = ' ';
            buffer[index + 1] = ' ';
            index += 2;
        }
        buffer[index] = '\n';
        index++;
    }   
    buffer[index] = '\0';
}

//print maze in Layout folder
void printMaze(int maze[], int width, int height, int iteration){
    char iter[5];
    sprintf(iter,"%d",iteration);

    _mkdir("Layout");

    char name[20] = "Layout/maze";
    strcat(name, iter);
    strcat(name, ".ppm");
    
    int w = width * 10;
    int h = height * 10;

    FILE *f = fopen(name, "wb");
    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", w, h);
    fprintf(f, "255\n");
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            int index =(((y * w + x) / w) * height / h) * width+ (((y * w + x) % w) * width / w);
            if(maze[index] == 1){
                fputc(0x00, f);
                fputc(0x00, f);
                fputc(0x00, f);
            }else{
                fputc(0xFF, f);
                fputc(0xFF, f);
                fputc(0xFF, f);
            }
        }
    }
    fclose(f);
}

//------------------ recursive division algorithm
bool chooseOrientationHorizontal(int width, int height){
    if(width < height){
        return HORIZONTAL;
    }else if(width > height){
        return VERTICAL;
    }
    else return rand() % 2;
}

void divide(int grid[], bool openings[], int x, int y, int width, int height, bool orientation, int gridWidth){
    if(width < 3 || height <  3) return;

    bool horizontal = orientation;

    int length = horizontal ? width : height;

    int wx = x + (horizontal ? 0 : 1 + rand() % (width - 2));
    int wy = y + (horizontal ? 1 + rand() % (height - 2) : 0);
    int tries = 10;
    if(horizontal){
        while(openings[wy * gridWidth + wx - 1] || openings[wy * gridWidth + wx + length]){
            if(tries == 0) return;
            wy = y + 1 + rand() % (height - 2);
            tries--;                        
        } 
    }else{
        while(openings[(wy - 1) * gridWidth + wx] || openings[(wy +  length ) * gridWidth + wx]){
            if(tries == 0) return;
            wx = x +  1 + rand() % (width - 2);            
            tries--;
        }         
    }


    int px = wx + (horizontal ? rand() % width : 0);
    int py = wy + (horizontal ? 0 : rand() % height);
    openings[py * gridWidth + px] = true; 


    int dx = horizontal ? 1 : 0;
    int dy = horizontal ? 0 : 1;


    for(int i = 0; i < length; i++){
        if(wx != px || wy != py) grid[wy * gridWidth + wx] = 1;               
        wx += dx;
        wy += dy;
    }   

    int nx = x;
    int ny = y;
    int w = horizontal ? width : wx - x;
    int h = horizontal ? wy - y : height;
    divide(grid, openings, nx, ny, w, h, chooseOrientationHorizontal(w, h), gridWidth);

    nx = horizontal ? x : wx + 1;
    ny = horizontal ? wy + 1: y;
    w = horizontal ? width : width + x - wx - 1;
    h = horizontal ? y + height - wy - 1 : height;
    divide(grid, openings, nx, ny, w, h, chooseOrientationHorizontal(w, h), gridWidth);
}

void generateMaze(int maze[], int startTile, int endTile, int iteration){
    int gridWidth = mazeWidth - 2;
    int gridHeight = mazeHeight - 2;

    srand(time(NULL));

    int grid[gridHeight * gridWidth];
    memset(grid, 0 , gridHeight * gridWidth * sizeof(int));    

    bool openings[gridHeight * gridWidth];
    memset(openings, false, gridHeight * gridWidth * sizeof(bool));
   
    divide(grid, openings, 0, 0, gridWidth, gridHeight, true, gridWidth);

    moveMaze(maze, grid, gridWidth, gridHeight, startTile, endTile);
    printMaze(maze, mazeWidth, mazeHeight, iteration);
}

//render of view and maze layout side to side
void printSideToSides(char vision[], char layout[]){
    int vIndex = 0;
    int lIndex = 0;

    char buffer[mazeWidth * mazeHeight * 10];
    for(int i = 0; i < 5; i++)
        buffer[i] = '\n';
    int bIndex = 5;

    while(vision[vIndex] != '\0' || layout[lIndex] != '\0' ){
        while(vision[vIndex] != '\0' && vision[vIndex] != '\n'){
            buffer[bIndex] = vision[vIndex];
            vIndex++;
            bIndex++;
        }
        for(int i = 0; i < 3; i++)
            buffer[bIndex + i] = ' ';
        bIndex += 3;
        while(layout[lIndex] != '\0' && layout[lIndex] != '\n'){
            buffer[bIndex] = layout[lIndex];
            lIndex++;
            bIndex++;
        }

        if(vision[vIndex] == '\n'){
            vIndex++;
        }
        if(layout[lIndex] == '\n'){
            lIndex++;
        }
        buffer[bIndex] = '\n';
        bIndex++;        
    }
    buffer[bIndex] = '\0';
    printf("%s", buffer);
}


