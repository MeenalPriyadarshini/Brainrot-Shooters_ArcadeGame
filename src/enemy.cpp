#include "raylib.h"
#include "include/enemy.h"
#include <cmath>



void SpawnEnemies(std::vector<Enemy>& enemies, int small, int big, float startX) {

    float spacing = 100;

    int smallCount = 0;

    for (int i=0;i<small;i++) {

        float x = startX + i*spacing;
        float y = GetRandomValue(120,380);

        enemies.push_back({{x,y},2,10,RED,true,10});
        smallCount++;

        if (smallCount >=5 && GetRandomValue(0,1)) {
            enemies.push_back({{x+50,(float)GetRandomValue(120,380)},60,20,PURPLE,true,30});
            smallCount=0;
        }
    }
}

void UpdateEnemies(std::vector<Enemy>& enemies, const Vector2& playerPos, int& hp) {
    const float worldSpeed = 120.0f;

    // Move world backward (enemies left)
    for (auto &e : enemies) {
        if (e.active) {
            e.pos.x -= worldSpeed * GetFrameTime();
            
            // Cull off-screen left
            if (e.pos.x < playerPos.x - 500) {
                e.active = false;
            }
        }
    }

    // Damage collision
    static float timer = 0;
    timer += GetFrameTime();

    for (auto &e : enemies) {
        if (!e.active) continue;

        float dx = playerPos.x - e.pos.x;
        float dy = playerPos.y - e.pos.y;
        float dist = std::sqrtf(dx*dx + dy*dy);

        if (dist < e.radius + 10 && timer > 0.5f) {
            hp -= 5;
            if (hp < 0) hp = 0;
            timer = 0;
        }
    }
}

void DrawEnemies(const std::vector<Enemy>& enemies) {
    for (auto &e: enemies)
        if (e.active) DrawCircleV(e.pos,e.radius,e.color);
}
