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
    int bullet_id;
    ALLEGRO_COLOR color;
} BULLET;

int getCountBulletsAlive(BULLET *bullets);
BULLET* getFreeBullet(BULLET bullets[MAX_BULLETS]);
void bullet_update(BULLET *bullet_list);
BULLET *create_shot_angle(BULLET *bullets, const float x, const float y, const float vx, const float vy, float angle_deg, int id);
BULLET *create_shot(BULLET *bullets, const float x, const float y, const float vx, const float vy, int id);
#endif // GAME_BULLETS_HEADER
