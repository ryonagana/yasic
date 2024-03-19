#include "g_bullets.h"



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

BULLET *create_shot(BULLET *bullets, const float x, const float y, const float vx, const float vy){

    BULLET *bullet = getFreeBullet(bullets);

    bullet->x = x;
    bullet->y = y;
    bullet->alive = TRUE;
    bullet->ttl = game_rand_range(80,255);
    bullet->vx = vx;
    bullet->vy = vy;


    return bullet;

}

