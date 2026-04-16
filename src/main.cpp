#include "raylib.h"
#include <vector>
#include <cmath>
#include "include/player.h"
#include "include/bullet.h"
#include "include/enemy.h"
#include "include/screen.h"
#include "include/map.h"

int main() {
    const int screenWidth = 900;
    const int screenHeight = 500;

    InitWindow(screenWidth, screenHeight, "Arcade Prototype");
    SetTargetFPS(60);
    HideCursor();

 Player player = { {100, 380}, {0, 0}, 5.0f, 0.0f, true, 100 };
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    Camera2D camera = {0};
    camera.offset = {screenWidth/2.0f, screenHeight/2.0f};
    camera.zoom = 1.0f;

    float shootCooldown = 0;
    float timePlaying = 0.0f;  // Fix plyerrightshiftbug - reset on restart
    int energy = 0;
    bool bossSpawned = false;
    float bossTimer = 0.0f;

    GameState state = MENU;

    while (!WindowShouldClose()) {

        // ================= STATE CONTROL =================
        if (state == MENU && IsKeyPressed(KEY_ENTER)) {
            enemies.clear();
            SpawnWave1Enemies(enemies, 600);
            bossSpawned = false;
            bossTimer = 0;
            state = PLAYING;
        }
        if (state == PLAYING && IsKeyPressed(KEY_ESCAPE)) state = PAUSED;
        if (state == PAUSED && IsKeyPressed(KEY_ESCAPE)) state = PLAYING;

        // ================= GAME UPDATE =================
        if (state == PLAYING) {

            UpdatePlayer(player);

            // Fixed camera scroll relative to player spawn
            timePlaying += GetFrameTime();
            camera.target.x = 100.0f + timePlaying * 120.0f;  // 2 pix/frame *60 = 120 pix/s
            camera.target.y = player.pos.y;

            // Clamp player relative to camera view (roughly centered, visible movement)
            const float halfBound = GetScreenWidth() / 2.0f - 100.0f;
            player.pos.x = fmaxf(camera.target.x - halfBound, fminf(player.pos.x, camera.target.x + halfBound));

            // Disabled recenter (prevent sliding)
            // if (fabsf(player.velocity.x) < 0.1f) {
            //     player.pos.x += (camera.target.x - player.pos.x) * 0.03f;
            // }

            // AIM
            Vector2 center = {player.pos.x, player.pos.y - 20};
            Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);

            Vector2 dir = {mouse.x - center.x, mouse.y - center.y};
            float len = sqrtf(dir.x*dir.x + dir.y*dir.y);
            if (len > 0.1f) {
                dir.x /= len;
                dir.y /= len;
            }

            // SHOOT
            shootCooldown += GetFrameTime();
            if (IsKeyDown(KEY_W) && shootCooldown > 0.15f) {
                bullets.push_back({center, {dir.x*10, dir.y*10}, true});
                shootCooldown = 0;
            }

            UpdateBullets(bullets);
            UpdateEnemies(enemies, player.pos, camera.target.x, player.hp);
            
            // COLLISION
            for (auto &b : bullets) {
                for (auto &e : enemies) {
                    if (!b.active || !e.active) continue;

                    if (CheckCollisionCircles(b.pos, 3, e.pos, e.radius)) {
                        e.hp -= 3;
                        b.active = false;

                        if (e.hp <= 0) {
                            e.active = false;
                            energy += e.energyValue;
                        }
                    }
                }
            }

            // Wave 1 Logic
            if (!bossSpawned && AreWave1EnemiesDead(enemies)) {
                SpawnBoss(enemies, camera.target.x);
                bossSpawned = true;
            }

            // Boss behavior
            if (bossSpawned && IsBossAlive(enemies)) {
                bossTimer += GetFrameTime();

                if (bossTimer > 2.0f) {
                    SpawnEType2(enemies, camera.target.x, player.pos);
                    bossTimer = 0;
                }
            }

            // End wave when boss dead
            if (bossSpawned && !IsBossAlive(enemies)) {
                state = GAMEOVER; // later → Wave 2
            }

            // GAME OVER CONDITIONS
            if (player.hp <= 0) {
                player.hp = 0;
                state = GAMEOVER;
            }

        }

        // RESTART - fix plyerrightshiftbug
        if (state == GAMEOVER && IsKeyPressed(KEY_R)) {
            timePlaying = 0.0f;
            camera.target = {100.0f, 380.0f};
            player = { {100, 380}, {0, 0}, 5.0f, 0.0f, true, 100 };
            bullets.clear();
            enemies.clear();
            energy = 0;
            bossSpawned = false;
            bossTimer = 0.0f;
            state = MENU;
        }

        // ================= DRAW =================
        BeginDrawing();
        ClearBackground(BLACK);

        if (state == MENU) {
            DrawMenuScreen();
        } else if (state == GAMEOVER) {
            DrawGameOverScreen(energy);
        } else {
            BeginMode2D(camera);

            DrawMap(camera);
            DrawPlayer(player, GetScreenToWorld2D(GetMousePosition(), camera));
            DrawEnemies(enemies);
            DrawBullets(bullets);

            EndMode2D();

            if (state == PAUSED) DrawPauseScreen();

            // UI
            DrawRectangle(10,10,200,20,DARKGRAY);
            DrawRectangle(10,10,player.hp*2,20,RED);
            DrawText(TextFormat("HP: %d", player.hp),10,40,20,WHITE);
            DrawText(TextFormat("Energy: %d", energy),10,70,20,GREEN);
        }

        if (state == PLAYING) {
            // CURSOR
            Vector2 m = GetMousePosition();
            DrawLine(m.x-10,m.y,m.x+10,m.y,GREEN);
            DrawLine(m.x,m.y-10,m.x,m.y+10,GREEN);
        }

        EndDrawing();
    }

    CloseWindow();
}
