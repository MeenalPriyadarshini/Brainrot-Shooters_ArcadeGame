#pragma once
#include "raylib.h"
#include <vector>

struct Enemy {
    Vector2 pos;
    int hp;
    float radius;
    Color color;
    bool active;
    int energyValue;
};

void SpawnEnemies(std::vector<Enemy>& enemies, int small, int big, float startX);
void UpdateEnemies(std::vector<Enemy>& enemies, const Vector2& playerPos, int& hp);
void DrawEnemies(const std::vector<Enemy>& enemies);
