#include "raylib.h"
#include "include/enemy.h"
#include <vector>
#include <cmath>

// lanes
static float lanes[] = {100, 170, 240, 310, 380};

// ---------------- SPAWN ----------------

void SpawnWave1Enemies(std::vector<Enemy>& enemies, float startX) {
    float spacing = 120;

    for (int i = 0; i < 50; i++) {
        float x = startX + i * spacing;
        float y = lanes[GetRandomValue(0, 4)];

        enemies.push_back({{x, y}, 12, 10, RED, true, 10, ETYPE1, {0,0}});
    }
}

// ---------------- SPAWN BOSS ----------------

void SpawnBoss(std::vector<Enemy>& enemies, float camX) {
    enemies.push_back({
        {camX, 120},
        25,
        200,   // 10x hp
        PURPLE,
        true,
        50,
        BOSS,
        {0,0}
    });
}

// ---------------- SPAWN ETYPE2 ----------------

void SpawnEType2(std::vector<Enemy>& enemies, float camX, const Vector2& playerPos) {
    float y = lanes[GetRandomValue(0,1)];

    Enemy e;
    e.pos = {camX + GetRandomValue(-200, 200), y};
    e.radius = 12;
    e.hp = 10;
    e.color = ORANGE;
    e.active = true;
    e.energyValue = 15;
    e.type = ETYPE2;
    e.lockTimer = 0.4f;  // 0.4 sec delay before attack
    e.locked = false;


    // Direction toward player
                    Vector2 dir = {
                        playerPos.x - e.pos.x,
                        playerPos.y - e.pos.y
                    };



    // Normalize
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.1f) {
        dir.x /= len;
        dir.y /= len;
    }

    // --- FORCE DIAGONAL LOOK ---
    // Ensure downward motion
    if (dir.y < 0) dir.y = -dir.y;

    // Clamp angle so it looks diagonal (not vertical/horizontal)
    if (fabs(dir.x) < 0.3f) dir.x = (dir.x < 0 ? -0.3f : 0.3f);

    // Normalize again
    float len2 = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len2 > 0.1f) {
        dir.x /= len2;
        dir.y /= len2;
    }

    // slight randomness
    dir.x += GetRandomValue(-20, 20) * 0.01f;
    dir.y += GetRandomValue(-20, 20) * 0.01f;

    float speed = 250.0f;
    e.velocity = {dir.x * speed, dir.y * speed};

    // slight randomness
    e.velocity.x += GetRandomValue(-20,20);
    e.velocity.y += GetRandomValue(-20,20);

    enemies.push_back(e);
}

// ---------------- UPDATE ----------------

void UpdateEnemies(std::vector<Enemy>& enemies, Player& p, float camX, Sound damageSound) {



    float dt = GetFrameTime();
    static float collisionTimer = 0.0f;
    collisionTimer += dt;
    const float worldSpeed = 120.0f;

    for (auto &e : enemies) {
        if (!e.active) continue;

        if (e.type == ETYPE1) {
            e.pos.x -= worldSpeed * dt;
        }

        else if (e.type == ETYPE2) {
            if (!e.locked) {
                e.lockTimer -= dt;

                // stay in place while locking
                if (e.lockTimer <= 0) {

                    // 🔥 LOCK TARGET (ONCE)
                    Vector2 dir = {
                        p.pos.x - e.pos.x,
                        p.pos.y - e.pos.y
                    };


                    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
                    if (len > 0.1f) {
                        dir.x /= len;
                        dir.y /= len;
                    }

                    // force downward diagonal feel
                    if (dir.y < 0) dir.y = -dir.y;

                    // make it more aggressive (less horizontal drift)
                    dir.x *= 0.8f;
                    dir.y *= 1.2f;

                    float len2 = sqrtf(dir.x * dir.x + dir.y * dir.y);
                    dir.x /= len2;
                    dir.y /= len2;


                    float speed = 420.0f; // super fast dash!
                    e.velocity = {dir.x * speed, dir.y * speed};




                    e.locked = true;

                }

            }
            else {
                // move after lock
                e.pos.x += e.velocity.x * dt;
                e.pos.y += e.velocity.y * dt;

                if (e.pos.y > 400) e.active = false;
            }
        }


        else if (e.type == BOSS) {
            // stick to camera
            e.pos.x = camX;
        }

        // collision
        Vector2 playerPos = p.pos;
        Rectangle playerHitbox = {
            playerPos.x - 10,
            playerPos.y - 40,
            20,
            50
        };


        if (CheckCollisionCircleRec(e.pos, e.radius, playerHitbox) && collisionTimer > 0.5f) {
            p.hp -= (e.type == ETYPE2) ? 30 : (e.type == ETYPE1 ? 15 : 15);
            if (p.hp < 0) p.hp = 0;
            p.damageFlashTimer = 0.15f;
            SetSoundVolume(damageSound, 1.0f);
            PlaySound(damageSound);
            collisionTimer = 0.0f;


        }




        // cull offscreen left
        if (e.pos.x < camX - 500) {
            e.active = false;
        }
    }
}

// ---------------- DRAW ----------------

void DrawEnemies(const std::vector<Enemy>& enemies) {
    for (auto &e : enemies) {
        if (!e.active) continue;

        // EASY TO REPLACE WITH TEXTURE LATER
        DrawCircleV(e.pos, e.radius, e.color);
    }
}

// ---------------- CHECK ----------------

bool AreWave1EnemiesDead(const std::vector<Enemy>& enemies) {
    for (auto &e : enemies) {
        if (e.active && e.type == ETYPE1) return false;
    }
    return true;
}

bool IsBossAlive(const std::vector<Enemy>& enemies) {
    for (auto &e : enemies) {
        if (e.active && e.type == BOSS) return true;
    }
    return false;
}

