#ifndef GAME_BULLETS_HEADER
#define GAME_BULLETS_HEADER

#include "g_utils.h"

#define MAX_BULLETS 50

typedef struct BULLET {
    float x,y;
    float vx,vy;
    int ttl;
    int alive;
    float angle;
    ALLEGRO_COLOR color;
} BULLET;

int getCountBulletsAlive(BULLET *bullets);
BULLET* getFreeBullet(BULLET bullets[MAX_BULLETS]);
BULLET *create_shot(BULLET *bullets, const float x, const float y, const float vx, const float vy);
#endif // GAME_BULLETS_HEADER
