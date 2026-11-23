#include "ray.h"

bool outOfBounds(vec2 coordinates){
    return coordinates.x < 0 || coordinates.x >= mazeWidth || coordinates.y < 0 || coordinates.y >= mazeHeight;
}

vec2 ray(vec2 coordinates, vec2 direction, int maze[]){
    vec2 tileIndex = {
        (int) coordinates.x,
        (int) coordinates.y
    };
    vec2 tileCoordinates = {
        coordinates.x - tileIndex.x,
        coordinates.y - tileIndex.y
    };
    while(!outOfBounds(tileIndex)){
        if(maze[(int) tileIndex.y * mazeWidth + (int) tileIndex.x] == 1){
            vec2 hitPoint = {
                tileCoordinates.x + tileIndex.x,
                tileCoordinates.y + tileIndex.y
            };
            return hitPoint;
        }
        float xstep, ystep;
        xstep = stepFunction(direction.x, tileCoordinates.x);
        ystep = stepFunction(direction.y, tileCoordinates.y);
        if(xstep < ystep){
            if(direction.x > 0){
                tileIndex.x++;
                tileCoordinates.x = 0.0f;
            }else{
                tileIndex.x--;
                tileCoordinates.x = 1.0f;
            }
            tileCoordinates.y += xstep * direction.y; 
        }else{
            if(direction.y > 0){
                tileIndex.y++;
                tileCoordinates.y = 0.0f;
            }else{
                tileIndex.y--;
                tileCoordinates.y = 1.0f;
            }
            tileCoordinates.x += ystep * direction.x; 
        }
     
    }
    vec2 dummy = {-1.0f, -1.0f};
    return dummy;
}

void rotate(char direction, float* angle){
    if(direction == ARROW_LEFT || direction == -32){
        *angle -= (M_PI / 4);
        if(*angle < 0) *angle = 7 * M_PI / 4;
    }else if(direction == ARROW_RIGHT){
        *angle += (M_PI / 4);
        if(*angle > 2 * M_PI ) *angle = M_PI / 4;
    }
}

void move(char action, vec2* player, vec2 direction, int maze[]){
    vec2 dummy = *player;
    vec2 effectiveDirection;
    if(action == 'w'){
        effectiveDirection.x = direction.x;
        effectiveDirection.y = direction.y;
    }
    if(action == 'a'){
        effectiveDirection.x = direction.y;
        effectiveDirection.y = -direction.x;
    }
    if(action == 's'){
        effectiveDirection.x = -direction.x;
        effectiveDirection.y = -direction.y;
    }   
    if(action == 'd'){
        effectiveDirection.x = -direction.y;
        effectiveDirection.y = direction.x;
    }
    dummy.x += (effectiveDirection.x * stepSize);
    dummy.y += (effectiveDirection.y * stepSize);
    if(maze[(int) dummy.y * mazeWidth + (int) dummy.x] == 1){
        return;
    }
    *player = dummy;
}

void drawWall(char icon, int startHeight, int endHeight, int width, int cameraW, char buffer[]){
    for(int j = startHeight; j < endHeight; j++){
        buffer[j * cameraW + width] = icon;
    } 
}

//check if given wall coordinate is actually corner, it is if it is in form of  "   " or "CWW"
//                                                                              "WC "    "W  "
//                                                                              "   "    "W  "
//in the first case we return for the outer parts of the corner, for the latter we only return for the inner part of the Walls
bool cornerCheck(vec2 hitPoint, int maze[], float hitX, float hitY, vec2 direction){
    if(hitX == 0.f){
        if(direction.x > 0){
            if(hitY < 0.08f){
                return maze[((int) hitPoint.y  - 1)* mazeWidth + (int) hitPoint.x] == 0 || 
                       maze[((int) hitPoint.y  - 1)* mazeWidth + (int) hitPoint.x - 1] == 1; 
            }else if(hitY > 0.92f){
                return maze[((int) hitPoint.y  + 1)* mazeWidth + (int) hitPoint.x] == 0 ||
                       maze[((int) hitPoint.y  + 1)* mazeWidth + (int) hitPoint.x - 1] == 1; 
            }    
        }else{
            if(hitY < 0.08f){
                return maze[((int) hitPoint.y  - 1)* mazeWidth + (int) hitPoint.x - 1] == 0 ||
                       maze[((int) hitPoint.y  - 1)* mazeWidth + (int) hitPoint.x + 1 - 1] == 1;    
            }else if(hitY > 0.92f){
                return maze[((int) hitPoint.y  + 1)* mazeWidth + (int) hitPoint.x - 1] == 0 ||
                       maze[((int) hitPoint.y  + 1)* mazeWidth + (int) hitPoint.x  + 1 - 1] == 1;    
            }             
        }
    }else if(hitY == 0.f){
        if(direction.y > 0){
            if(hitX < 0.08f){
                return maze[(int) hitPoint.y * mazeWidth + (int) hitPoint.x - 1] == 0 ||
                       maze[((int) hitPoint.y  - 1)* mazeWidth + (int) hitPoint.x - 1] == 1; 
            }else if(hitX > 0.92f){
                return maze[(int) hitPoint.y * mazeWidth + (int) hitPoint.x + 1] == 0 ||
                       maze[((int) hitPoint.y  - 1)* mazeWidth + (int) hitPoint.x + 1] == 1;  
            }    
        }else{
            if(hitX < 0.08f){
                return maze[((int) hitPoint.y - 1) * mazeWidth + (int) hitPoint.x - 1] == 0 ||
                       maze[((int) hitPoint.y  + 1 - 1)* mazeWidth + (int) hitPoint.x - 1] == 1;   
            }else if(hitX > 0.92f){
                return maze[((int) hitPoint.y - 1) * mazeWidth + (int) hitPoint.x + 1] == 0 ||
                       maze[((int) hitPoint.y  + 1 - 1)* mazeWidth + (int) hitPoint.x  + 1] == 1;    
            }
        }
    } 
    return false;
}



