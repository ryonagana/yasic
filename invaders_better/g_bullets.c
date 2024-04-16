#include "g_bullets.h"
#include "main.h"
#include "g_render.h"
#include <math.h>

int getCountBulletsAlive(BULLET *bullets){

    int c = 0;
    while(++c < MAX_BULLETS && bullets[c].alive);
    return c;

}

BULLET* getFreeBullet(BULLET bullets[MAX_BULLETS]){
   int c = 0;

   for(c = 0; c < MAX_BULLETS; c++){
    if(!bullets[c].alive){
            break;
        }
    }

   return &bullets[c];
}

BULLET *create_shot_angle(BULLET *bullets, const float x, const float y, const float vx, const float vy, float angle_deg, int id){
    BULLET *bullet = getFreeBullet(bullets);

    bullet->alive = TRUE;
    bullet->vx = vx;
    bullet->vy = vy;
    bullet->bullet_id  = id;
    bullet->x = x;
    bullet->y = y;
    bullet->angle = angle_deg;
    bullet->ttl = game_rand_range(100, 255);
    return bullet;
}

BULLET *create_shot(BULLET *bullets, const float x, const float y, const float vx, const float vy, int id){

    BULLET *bullet = getFreeBullet(bullets);

    bullet->x = x;
    bullet->y = y;
    bullet->alive = TRUE;
    bullet->ttl = game_rand_range(100,255);
    bullet->vx = vx;
    bullet->vy = vy;
    bullet->angle = 90;
    bullet->bullet_id = id;


    return bullet;

}

void bullet_update(BULLET *bullet_list){

    for(int i = 0; i < MAX_BULLETS;i++){
        if(!bullet_list[i].alive) continue;

        if(bullet_list[i].x < -8){
            bullet_list[i].alive = FALSE;
        }

        if(bullet_list[i].x > al_get_display_width(display)-1){
            bullet_list[i].alive = FALSE;
        }


        if(bullet_list[i].y < -8){
            bullet_list[i].alive = FALSE;
        }

        if(bullet_list[i].y > al_get_display_height(display)-1){
            bullet_list[i].alive = FALSE;
        }


        bullet_list[i].x += bullet_list[i].vx;
        bullet_list[i].y += bullet_list[i].vy;

    }

}
