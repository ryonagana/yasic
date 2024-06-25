#ifndef PLAYER_H
#define PLAYER_H
#include <stdio.h>
#include <stdint.h>
#include "bullet.h"



typedef struct PLAYER {
    int x;
    int y;
    int vx;
    int vy;
    int shoot_timer;
    int direction;
    uint32_t shoot_count;
    uint32_t weapons;
    uint32_t items;
    int accel_timer;
    int accel_index;
    int state;
    TBULLETS bullets;

}PLAYER;

void Player_Init(PLAYER *p);
void Player_SpawnPos(PLAYER *p, int x, int y);
void Player_SetShootTime(PLAYER *p, int timer);
void Player_SetShootCount(PLAYER *p, int count);

void Player_Update(PLAYER *p);
void Player_Render(PLAYER *p);

void Player_MoveLeft(PLAYER *p);
void Player_MoveRight(PLAYER *p);

void Player_Shoot(PLAYER *p);

#endif // PLAYER_H
