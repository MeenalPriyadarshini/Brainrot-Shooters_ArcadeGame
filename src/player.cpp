#include "raylib.h"
#include "include/player.h"
#include <cmath>



void UpdatePlayer(Player& p) {
// Horizontal movement (fixed timestep 60fps)
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        p.velocity.x = -p.speed;
    } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        p.velocity.x = p.speed;
    } else {
        p.velocity.x = 0;
    }
    p.pos.x += p.velocity.x;

    // Jump and gravity (fixed timestep)
    if (IsKeyPressed(KEY_SPACE) && p.onGround) {
        p.velocityY = -10;
        p.onGround = false;
    }

    p.velocityY += 0.5f;
    p.pos.y += p.velocityY;

    if (p.pos.y >= 380) {
        p.pos.y = 380;
        p.velocityY = 0;
        p.onGround = true;
    }
}

void DrawPlayer(const Player& p, Vector2 mouse) {

    Vector2 center = {p.pos.x, p.pos.y - 20};

    DrawCircleV(center, 8, WHITE);
    DrawLine(center.x, center.y, center.x, center.y+30, WHITE);

    float t = GetTime();
float leg = std::sinf((float)t*10)*5;

    DrawLine(center.x,center.y+30,center.x-10,center.y+45+leg,WHITE);
    DrawLine(center.x,center.y+30,center.x+10,center.y+45-leg,WHITE);

    // gun
    Vector2 dir = {mouse.x-center.x, mouse.y-center.y};
float len = std::sqrtf(dir.x*dir.x + dir.y*dir.y);
    if (len > 0.1f) {
        dir.x/=len; dir.y/=len;
    }

    Vector2 gunEnd = {center.x+dir.x*20, center.y+dir.y*20};
    DrawLineEx(center, gunEnd, 4, GRAY);
}
