#include "include/bullet.h"
#include <algorithm>

void UpdateBullets(std::vector<Bullet>& bullets) {
    for (auto &b : bullets) {
        if (b.active) {
            b.pos.x += b.vel.x;
            b.pos.y += b.vel.y;
        }
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b){
        return !b.active;
    }), bullets.end());
}

void DrawBullets(const std::vector<Bullet>& bullets) {
    for (auto &b : bullets)
        if (b.active) DrawCircleV(b.pos,3,WHITE);
}
