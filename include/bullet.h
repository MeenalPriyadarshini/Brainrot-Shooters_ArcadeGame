#pragma once
#include "raylib.h"
#include <vector>

struct Bullet {
    Vector2 pos;
    Vector2 vel;
    bool active;
};

void UpdateBullets(std::vector<Bullet>& bullets);
void DrawBullets(const std::vector<Bullet>& bullets);
