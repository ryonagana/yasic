#ifndef LEVEL_H
#define LEVEL_H


#define LEVEL_DIFFICULT_BEG 0
#define LEVEL_DIFFICULT_EASY 1
#define LEVEL_DIFFICULT_MEDIUM 2
#define LEVEL_DIFFICULT_HARD 3
#define LEVEL_DIFFICULT_NIGHTMARE 4



#include "enemy.h"

#define TILE 32

typedef struct LEVEL {
    int shoot_counter;
    int enemies_shoot_count;
    struct ENEMY *enemies;
    int difficulty;

}LEVEL;

void LVL_Init(LEVEL *lvl,  ENEMY (*enemies_list)[ENEMY_COLS]);
void LVL_Start(ENEMY (*enemies)[ENEMY_COLS]);
void LVL_End(void);

#endif // LEVEL_H
