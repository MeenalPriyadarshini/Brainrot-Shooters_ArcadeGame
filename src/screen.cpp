#include "include/screen.h"
#include "raylib.h"

void DrawMenuScreen() {
    DrawText("ARCADE GAME", 300,150,40,GREEN);
    DrawText("PRESS ENTER", 300,220,25,WHITE);
}

void DrawPauseScreen() {
    DrawText("PAUSED", 350,200,40,YELLOW);
}

void DrawGameOverScreen(int score) {
    DrawText("GAME OVER", 300,150,40,RED);
    DrawText(TextFormat("Score: %d",score),320,220,30,YELLOW);
    DrawText("R TO RESTART",300,280,20,WHITE);
}
