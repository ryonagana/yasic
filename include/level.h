#ifndef LEVEL_H
#define LEVEL_H


#define LEVEL_DIFFICULT_BEG 0
#define LEVEL_DIFFICULT_EASY 1
#define LEVEL_DIFFICULT_MEDIUM 2
#define LEVEL_DIFFICULT_HARD 3
#define LEVEL_DIFFICULT_NIGHTMARE 4



#include "enemy.h"

#define TILE 32


#define LEVEL_STATE_STOPPED    0
#define LEVEL_STATE_MARCH_WALK 1


typedef struct LEVEL {
    int shoot_counter;
    int enemies_shoot_count;
    struct ENEMY *enemies;
    int difficulty;
    int march_walk_time;
    int march_walk_delay;
    int state;

}LEVEL;

void LVL_Init(LEVEL *lvl,  ENEMY (*enemies_list)[ENEMY_COLS]);
void LVL_Start(ENEMY (*enemies)[ENEMY_COLS]);
void LVL_Update(LEVEL *level, ENEMY (*enemies)[ENEMY_COLS]);
void LVL_RenderGrid(void);
void LVL_End(void);

#endif // LEVEL_H
