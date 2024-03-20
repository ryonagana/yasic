#ifndef GAME_ENEMIES_HEADER
#define GAME_ENEMIES_HEADER

#define ENEMY_ROW_X 10
#define ENEMY_ROW_Y 5
#include "g_bullets.h"

#define MAX_BULLETS 50

#define WALK_TIME_DELAY_PHASE1 1000
#define WALK_TIME_DELAY_PHASE2 900
#define WALK_TIME_DELAY_PHASE3 600
#define WALK_TIME_DELAY_PHASE4 500
#define WALK_TIME_DELAY_PHASE5 300


enum  ENEMY_TYPE {
    ENEMY_TYPE_BLUE,
    ENEMY_TYPE_GREEN,
    ENEMY_TYPE_SHIP,

    ENEMY_TYPE_COUNT
};


typedef struct ENEMY {
    float x,y;
    int type;
    int life;
    int alive;
    BULLET bullets[MAX_BULLETS];
    int shoot_time;

} ENEMY;

typedef struct PLAYER PLAYER;
typedef struct ITEM ITEM;



ENEMY* getFreeEnemy(ENEMY (*enemies)[ENEMY_ROW_X]);
void enemies_init(ENEMY (*enemies)[ENEMY_ROW_X]);
int enemies_bullet_collision(ENEMY *e, BULLET *b);
void enemies_update_bullet(PLAYER *player, ENEMY (*enemies)[ENEMY_ROW_X]);
void enemies_draw_bullets(ENEMY enemies[ENEMY_ROW_Y][ENEMY_ROW_X]);
void enemies_set_down(ENEMY (*enemies)[ENEMY_ROW_X]);
int enemies_count(ENEMY (*enemies)[ENEMY_ROW_X]);
void enemies_killall(ENEMY *enemies[]);
void enemies_reset(ENEMY (*enemies)[ENEMY_ROW_X]);
void enemies_update(PLAYER *player,  ITEM *item_list, ENEMY (*enemies)[ENEMY_ROW_X]);
void draw_enemies(ENEMY enemy_list[ENEMY_ROW_Y][ENEMY_ROW_X], float offset_x, float offset_y);
int  enemies_bullet_collision(ENEMY *enemy, BULLET *bullet);
#endif // GAME_ENEMIES_HEADER
