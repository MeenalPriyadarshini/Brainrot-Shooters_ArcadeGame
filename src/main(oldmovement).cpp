#include "raylib.h"
#include <vector>
#include <cmath>
#include "include/player.h"
#include "include/bullet.h"
#include "include/enemy.h"
#include "include/screen.h"
#include "include/map.h"

bool AreAllEnemiesGone(const std::vector<Enemy>& enemies, float camLeft) {
    for (const auto& e : enemies) {
        if (e.active && e.pos.x > camLeft) {
            return false; // still active in screen/world
        }
    }
    return true;
}

int main() {
    const int screenWidth = 900;
    const int screenHeight = 500;

    InitWindow(screenWidth, screenHeight, "Arcade Prototype");
    SetTargetFPS(60);
    HideCursor();

    Player player{{100, 380}, 0, true, 100};
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    SpawnEnemies(enemies, 50, 3, 600);

    Camera2D camera = {0};
    camera.offset = {screenWidth/2.0f, screenHeight/2.0f};
    camera.zoom = 1.0f;

    float shootCooldown = 0;
    int energy = 0;

    GameState state = MENU;

    while (!WindowShouldClose()) {

        // ================= STATE CONTROL =================
        if (state == MENU && IsKeyPressed(KEY_ENTER)) state = PLAYING;
        if (state == PLAYING && IsKeyPressed(KEY_ESCAPE)) state = PAUSED;
        if (state == PAUSED && IsKeyPressed(KEY_ESCAPE)) state = PLAYING;

        // ================= GAME UPDATE =================
        if (state == PLAYING) {

            UpdatePlayer(player);
            camera.target = player.pos;

            // Wave clear check (new fix for off-screen enemies)
            float camLeft = camera.target.x - 500;
            if (AreAllEnemiesGone(enemies, camLeft)) {
                state = GAMEOVER;
            }

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
            UpdateEnemies(enemies, player.pos, player.hp);
            
            // COLLISION
            for (auto &b : bullets) {
                for (auto &e : enemies) {
                    if (!b.active || !e.active) continue;

                    if (CheckCollisionCircles(b.pos, 3, e.pos, e.radius)) {
                        e.hp -= 10;
                        b.active = false;

                        if (e.hp <= 0) {
                            e.active = false;
                            energy += e.energyValue;
                        }
                    }
                }
            }

            // GAME OVER CONDITIONS
            if (player.hp <= 0) {
                player.hp = 0;
                state = GAMEOVER;
            }
            // Wave clear ends game (old check - complements new)
            static float waveCheckTimer = 0;
            waveCheckTimer += GetFrameTime();
            if (waveCheckTimer > 0.1f) {
                bool allDead = true;
                for (auto &e : enemies) {
                    if (e.active) {
                        allDead = false;
                        break;
                    }
                }
                if (allDead) state = GAMEOVER;
                waveCheckTimer = 0;
            }
        }

        // RESTART
        if (state == GAMEOVER && IsKeyPressed(KEY_R)) {
            player = {{100,380},0,true,100};
            bullets.clear();
            enemies.clear();
            energy = 0;
            SpawnEnemies(enemies, 50, 3, 600);
            state = MENU;
        }

        // ================= DRAW =================
        BeginDrawing();
        ClearBackground(BLACK);

        if (state == MENU) {
            DrawMenuScreen();
        }
        else {
            BeginMode2D(camera);

            DrawMap(camera);
            DrawPlayer(player, GetScreenToWorld2D(GetMousePosition(), camera));
            DrawEnemies(enemies);
            DrawBullets(bullets);

            EndMode2D();

            if (state == PAUSED) DrawPauseScreen();
            if (state == GAMEOVER) DrawGameOverScreen(energy);

            // UI
            DrawRectangle(10,10,200,20,DARKGRAY);
            DrawRectangle(10,10,player.hp*2,20,RED);
            DrawText(TextFormat("HP: %d", player.hp),10,40,20,WHITE);
            DrawText(TextFormat("Energy: %d", energy),10,70,20,GREEN);
        }

        // CURSOR
        Vector2 m = GetMousePosition();
        DrawLine(m.x-10,m.y,m.x+10,m.y,GREEN);
        DrawLine(m.x,m.y-10,m.x,m.y+10,GREEN);

        EndDrawing();
    }

    CloseWindow();
}

