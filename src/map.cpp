#include "include/map.h"

void DrawMap(Camera2D cam) {

    float left = cam.target.x - 3000;
    float right = cam.target.x + 3000;

    DrawLine(left,400,right,400,DARKGRAY);
    DrawLine(left,100,right,100,DARKGRAY);
}
