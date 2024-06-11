#include "player.h"
#include "display.h"


static float s_speed_modifier[4] = {
    4.5f,
    5.0f,
    6.2f,
    6.9f
};

void Player_Init(PLAYER *p){
    p->x = 0;
    p->y = 0;
    p->shoot_timer = 50;
    p->vx = 1;
    p->vy = 0;
    p->shoot_count = 1;
    p->direction = 1;
    p->accel_timer = 70;
    p->accel_index = 0;
}
void Player_SpawnPos(PLAYER *p, int x, int y){
    p->x = x;
    p->y = y;
}

void Player_SetShootTime(PLAYER *p, int timer){
    p->shoot_timer = timer;
}


void Player_SetShootCount(PLAYER *p, int count){
    p->shoot_timer = count;
}

void Player_MoveLeft(PLAYER *p){
    p->direction = -1;
}
void Player_MoveRight(PLAYER *p){
    p->direction = 1;
}

void Player_Update(PLAYER *p){

    p->vx = p->direction * s_speed_modifier[0];
    //p->vy = p->direction * s_speed_modifier[0];

    p->x += p->vx;
    p->y += p->vy;
    p->direction = 0;
    return;
}

void Player_Render(PLAYER *p){
    al_draw_filled_rectangle(p->x, p->y, p->x + 32, p->y + 32, al_map_rgb(0,0,255));
}
