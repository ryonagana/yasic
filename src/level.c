#include "level.h"
#include "enemy.h"
#include <string.h>
static int s_shoot_timer_list[LEVEL_DIFFICULT_NIGHTMARE+1] = {
    150,
    120,
    90,
    80,
    50
};

static int s_shoot_counter_list[LEVEL_DIFFICULT_NIGHTMARE+1] = {
    1,
    2,
    3,
    5,
    7
};



void LVL_Init(LEVEL *lvl, ENEMY (*enemies_list)[ENEMY_COLS]){
    lvl->enemies = *enemies_list;
    lvl->difficulty  = LEVEL_DIFFICULT_EASY;
    lvl->enemies_shoot_count = s_shoot_counter_list[lvl->difficulty % (LEVEL_DIFFICULT_NIGHTMARE+1)];
    lvl->shoot_counter = s_shoot_timer_list[lvl->difficulty % (LEVEL_DIFFICULT_NIGHTMARE+1)];

    memset(lvl->enemies, 0, sizeof(lvl->enemies) * ENEMY_ROWS * ENEMY_COLS);
}


void LVL_Start(ENEMY (*enemies)[ENEMY_COLS]){
    for(int y = 0; y < ENEMY_COLS;y++){
        for(int x = 0;x < ENEMY_ROWS;x++){
            ENEMY *e = &enemies[y][x];
            e->x = x * TILE * 1.5;
            e->y = y * TILE * 1.5;
            e->alive = 1;
            e->flags = 0;
            e->vx = 1;
            e->vy = 1;
            e->direction = 1;
        }
    }
}


void LVL_End(void){

}
