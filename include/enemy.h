#pragma once
#include "raylib.h"
#include <vector>
#include "player.h"


enum EnemyType {
    ETYPE1,
    ETYPE2,
    BOSS
};

struct Enemy {
    Vector2 pos;
    float radius;
    int hp;
    Color color;
    bool active;
    int energyValue;

    EnemyType type;

    // for EType2 dash
    Vector2 velocity;
    float lockTimer;
    bool locked;
};



void UpdateEnemies(std::vector<Enemy>& enemies, Player& player, float camX, Sound damageSound);

void DrawEnemies(const std::vector<Enemy>& enemies);

void SpawnWave1Enemies(std::vector<Enemy>& enemies, float startX);
void SpawnBoss(std::vector<Enemy>& enemies, float camX);
void SpawnEType2(std::vector<Enemy>& enemies, float camX, const Vector2& playerPos);

bool AreWave1EnemiesDead(const std::vector<Enemy>& enemies);
bool IsBossAlive(const std::vector<Enemy>& enemies);
