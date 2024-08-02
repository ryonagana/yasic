#include "bullet.h"
#include "utils.h"
#include "video.h"
#include <math.h>

#define BULLET_LOOP(max, ptr) for(int i = 0; i < (max); i++){\
                                        struct bullet_t *b = &(ptr)[i];\

#define BULLET_LOOP_END()  }\

void bullet_init(struct bullet_t *bullet, int max, int flags){
        BULLET_LOOP(max, bullet)
        {
            b->alive = 0;
            b->flags = flags;
            b->alive = FALSE;
            b->angle = 0;
            b->gravity = -0.008f;
        }
        BULLET_LOOP_END()

}
void bullet_update(struct bullet_t *bullet, int max, double delta){
    BULLET_LOOP(max, bullet)
    {
        if(b->alive){

            if(b->pos.y < -16){
                b->alive = FALSE;
            }

            if(b->pos.y > video_height()+16){
                b->alive = FALSE;
            }


            if(b->flags & BULLET_FLAG_GRAVITY){
                //b->vel.x =  cos(b->angle * DEG2RAD) * 1.0 * GRAVITY * delta;
                //b->vel.y =  b->gravity * sin(b->angle * DEG2RAD) * 1.0 * GRAVITY * delta;
                b->vel.y =  sin(b->angle * DEG2RAD) * -1.0 * delta * b->gravity;
                b->vel.x =  cos(b->angle * DEG2RAD) *  1.0 * delta * b->gravity;
            }else {
                b->vel.y =  -1.0 * delta;
            }

            b->pos.x += b->vel.x;
            b->pos.y += b->vel.y;
        }
    }
    BULLET_LOOP_END()
}
void bullet_draw(struct bullet_t *bullet, int max, SDL_Texture *tex){
    BULLET_LOOP(max, bullet)
    {
        if(b->alive){
            SDL_FRect dst  = {
                .x =  b->pos.x,
                .y =  b->pos.y,
                .w =  11,
                .h =  11
            };

            SDL_RenderCopyExF(gamewindow.renderer, tex, NULL, &dst, 0.0, NULL, SDL_FLIP_NONE);
        }
    }
    BULLET_LOOP_END()
}

struct bullet_t *bullet_get_free(struct bullet_t *bullet, int max){
    int c = 0;
    while(bullet[c].alive && c < max) c++;
    if(c == max) return NULL;
    return &bullet[c];
}
