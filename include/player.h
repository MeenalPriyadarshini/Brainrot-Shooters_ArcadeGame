#pragma once
#include "raylib.h"

struct Player {
    Vector2 pos;
    float velocityY;
    bool onGround;
    int hp;
};

void UpdatePlayer(Player& p);
void DrawPlayer(const Player& p, Vector2 mouse);
