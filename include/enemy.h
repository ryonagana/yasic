#ifndef ENEMY_H
#define ENEMY_H

#include <stdio.h>
#include <stdint.h>

#define ENEMY_ROWS 7
#define ENEMY_COLS 7


#define ENEMY_DIR_RIGHT (1)
#define ENEMY_DIR_LEFT  (-1)

typedef struct ENEMY {
    int x;
    int y;
    int vx;
    int vy;
    int alive;
    uint32_t flags;
    int direction;
}ENEMY;

//dummy
typedef struct LEVEL LEVEL;

void Enemy_Render(ENEMY (*enemies)[ENEMY_COLS]);
void Enemy_Shutdown(void);
int Enemy_AliveCount(ENEMY (*enemies)[ENEMY_COLS]);
void Enemy_MoveDown(ENEMY (*enemies)[ENEMY_COLS], LEVEL *level);
void Enemy_ChangeDirection(ENEMY (*enemies)[ENEMY_COLS], int direction);
void Enemy_CorrectPosition(ENEMY (*enemies)[ENEMY_COLS]);
#endif // ENEMY_H
