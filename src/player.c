#include "player.h"

void Player_Init(PLAYER *p){
    p->x = 0;
    p->y = 0;
    p->shoot_timer = 50;
    p->vx = 1;
    p->vy = 0;
    p->shoot_count = 1;
}
void Player_SpawnPos(PLAYER *p, int x, int y){
    p->x = 0;
    p->y = 0;
}

void Player_SetShootTime(PLAYER *p, int timer){
    p->shoot_timer = timer;
}


void Player_SetShootCount(PLAYER *p, int count){
    p->shoot_timer = count;
}

