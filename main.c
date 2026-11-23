#include "ray.h"
#include "maze.h"
#include "globals.h"

void clearInputBuffer(){
    while(kbhit()){
        getch();
    }
}

int main(){
    system("cls");
    printf("Controls: wasd + left/right arrow\nClose the game with ESC\nPress 1 for no map, 2 for map");
    
    bool map = false;

    if(getch() == '2'){
        map = true;
    }

    system("cls");
    printf("Press anything");

    int maze[mazeWidth * mazeHeight]; 
    memset(maze, 0, sizeof(maze));
    int StartTile = sizeof(maze)/sizeof(maze[0]) - 2;
    int EndTile = 1;
    generateMaze(maze, StartTile, EndTile, 1);

    vec2 player = {
        .x = mazeHeight - 1.5f,
        .y = mazeWidth - 1.5f
    };

    float angle = 9 *  M_PI / 6 ;

    int cameraW = 120;
    int cameraH = 55;
        
    vec2 direction = radToVec2(angle);
    float FOV = 70.0f * M_PI  / 180.0f;
    float focalLength =(cameraW / 2.0f) / tanf(FOV / 2.0f);  

    float planeLength = tan(FOV / 2.0f);
    vec2 cameraPlane = {
        -direction.y * planeLength,
        direction.x * planeLength
    };

    char framebuffer[cameraW * cameraH];
    char output[cameraW * cameraH + cameraH + 1];

    while(1){
        char action;
        action = getch();
        system("cls");

        //EVENT HANDLING

        if(action == ESC){
            int index = 1; 
            while(1){
                char curr[30] ;
                sprintf(curr, "Layout/maze%d.ppm", index);
                if(remove(curr) != 0){
                    if(errno == ENOENT)
                        break;
                    else if(errno == EBUSY || errno == EACCES){
                        printf("Please close the maze \n");  
                        printf("Press anything to close the game");
                        getch();
                        break;
                    }   
                }
                index++;
            }
            break;
        }


        if(action == '\0' || action == -32){
            action = getch();
            rotate(action, &angle);
        }else{
            move(action, &player, direction, maze);
        }

        if((int) player.y * mazeWidth + (int) player.x == EndTile){
            printf("Das Spiel ist vorbei.");
            break;
        }

        //WALL CALCULATION

        direction = radToVec2(angle);
        cameraPlane.x = -direction.y * planeLength;
        cameraPlane.y =  direction.x * planeLength;

        memset(framebuffer, ' ', cameraH * cameraW);
        for(int i = 0; i < cameraW; i++){
            float cameraFactor = 2.0f * (i + 0.5f) / cameraW - 1.0f;
            vec2 rayDirection = {
                direction.x + cameraPlane.x * cameraFactor,
                direction.y + cameraPlane.y * cameraFactor
            };

            vec2 wall = ray(player, rayDirection, maze);

            if(wall.x == -1.0f && wall.y == -1.0f) continue;

            float dx = wall.x - player.x;
            float dy = wall.y - player.y;
            float depth = dx * direction.x + dy * direction.y;
            float height = ((float) wallHeight * focalLength) / depth;

            int startHeight = (cameraH / 2) - (height / 2) > 0 ? (cameraH / 2) - (height / 2) : 0;
            int endHeight = (cameraH / 2) + (height / 2) < cameraH ? (cameraH / 2) + (height / 2) : cameraH;
            
            framebuffer[ (cameraH / 2 ) * cameraW + cameraW / 2] = 'A';

            float tileX = decimal(wall.x);
            float tileY = decimal(wall.y);
            if((tileX < 0.08f || tileX > 0.92f) && (tileY < 0.08f || tileY > 0.92f)){
                if(cornerCheck(wall, maze, tileX, tileY, rayDirection)){
                    drawWall('|', startHeight, endHeight, i, cameraW, framebuffer);
                }else{
                    drawWall('*', startHeight, endHeight, i, cameraW, framebuffer);
                }
            }else{
                drawWall('*', startHeight, endHeight, i, cameraW, framebuffer);
            }
        }

        //ACTUAL RENDERING OF OUTPUT BUFFER

        int position = 0;
        if(!map){
            for(int i = 0; i < 5; i++){
                output[position] = '\n';
                position++;
            }
        }
        
        for(int y = 0; y < cameraH; y++){
            for(int x = 0; x < cameraW; x++){
                if(x == 0 && y == 0){
                    output[position] = framebuffer[0];
                    position++;
                    continue;
                }
                if(framebuffer[y * cameraW + x] == '*'){ 
                    if(framebuffer[(y - 1) * cameraW + x] == ' '){
                        output[position] = 223;
                    }else if(framebuffer[(y + 1) * cameraW + x] == ' '){
                        output[position] = 220;
                    }
                    else if((framebuffer[y * cameraW + x - 1] == ' ' && framebuffer[y * cameraW + x + 1] == '*') ||  
                            (framebuffer[y * cameraW + x - 1] == '*' && framebuffer[y * cameraW + x + 1] == ' ') 
                        ){
                        output[position] = '|';
                    }
                    else{
                        output[position] = '*';
                    }
                }
                else{
                    output[position] = framebuffer[y * cameraW + x];
                }
                position++;
            }
            output[position] = '\n';
            position++;
        }
        output[position] = '\0';

        if(map){
            char buffer[mazeWidth * mazeWidth * 5];
            insertMaze(maze, mazeWidth, mazeHeight, buffer, (int) player.y * mazeWidth + (int) player.x);
            printSideToSides(output, buffer);
        }else{
            printf("%s", output); 
        }
        clearInputBuffer();
    }

    system("del /Q \"Layout\\*\"");
    return 0;
}

