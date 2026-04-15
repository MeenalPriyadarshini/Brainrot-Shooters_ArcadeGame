#pragma once
#include "raylib.h"

enum GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAMEOVER
};

void DrawMenuScreen();
void DrawPauseScreen();
void DrawGameOverScreen(int score);
