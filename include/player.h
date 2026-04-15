#pragma once
#include "raylib.h"

struct Player {
    Vector2 pos;
    Vector2 velocity;
float speed = 5.0f;
    float velocityY;
    bool onGround;
    int hp;
};

void UpdatePlayer(Player& p);
void DrawPlayer(const Player& p, Vector2 mouse);
