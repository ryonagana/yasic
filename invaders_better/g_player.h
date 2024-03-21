#ifndef GAME_PLAYER_HEADER
#define GAME_PLAYER_HEADER

#include "g_item.h"
#include "g_bullets.h"


typedef struct PLAYER {
    float x,y;
    float vx,vy;
    float life;
    int lives;
    int32_t score;
    int alive;
    int direction;
    BULLET bullets[MAX_BULLETS];
    int shot_time;
    int shot_count;
    int shoot;
    int keypressed[4];
    ITEM items[ITEM_ID_COUNT];
    ITEM *item_use;
    int ammo;
} PLAYER;

void player_init(PLAYER *p);

void player_draw(float x, float y);
void player_update_shot(PLAYER *player);
void player_draw_shot(PLAYER *player);
void player_shoot(PLAYER *player);
void player_update(PLAYER *player);
#endif // GAME_PLAYER_HEADER
