#ifndef ENEMY_H
#define ENEMY_H

#include <stdio.h>
#include <stdint.h>

#define ENEMY_ROWS 8
#define ENEMY_COLS 5


typedef struct ENEMY {
    int x;
    int y;
    int vx;
    int vy;
    int alive;
    uint32_t flags;
    int direction;
}ENEMY;

void Enemy_Update(ENEMY (*enemies)[ENEMY_COLS]);
void Enemy_Render(ENEMY (*enemies)[ENEMY_COLS]);
void Enemy_Shutdown(void);
#endif // ENEMY_H
