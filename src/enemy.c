#include <math.h>
#include "enemy.h"
#include "utils.h"
#include "video.h"


#define ENEMY_LOOP(ptr, max_col, max_row) for(int i = 0; i < (max_row); i++){\
                                for(int j = 0; j < (max_col); j++){\
                                    struct enemy_t *e = &(ptr)[i][j];\

#define ENEMY_LOOP_END()  }\
                        }\




void enemy_init(struct enemy_data *en, int max_col, int max_row){

    if(max_col >= ENEMY_COL) max_col = ENEMY_COL;
    if(max_row >= ENEMY_ROW) max_row = ENEMY_ROW;

    ENEMY_LOOP(en->enemies, max_col, max_row)
    {
        e->pos.x = (i * ENEMY_COL) * 2.5f;
        e->pos.y = (j * ENEMY_COL) * 2.5f;
        e->alive = TRUE;
        e->direction = 1;
        e->line = 0;
        e->flags = 0;


        bullet_init(e->bullets, ENEMY_MAX_BULLETS, 0);


    }
    ENEMY_LOOP_END()

    en->cols = max_col;
    en->rows = max_row;
}



int enemy_alive_count(struct enemy_data *en){
    int count = 0;
    ENEMY_LOOP(en->enemies, en->cols, en->cols)
    {
        if(e->alive) count++;
    }
    ENEMY_LOOP_END()

    return count;

}

void enemy_player_bullet_collision(struct enemy_t *e , struct spaceship *ship){


    for(int b = 0; b < SHIP_BULLET_MAX; b++){

        if(ship->bullets[b].alive){
                struct bullet_t *b_ptr = &ship->bullets[b];
                if(rect_collision(e->pos.x, e->pos.y, 10,10, b_ptr->pos.x, b_ptr->pos.y, 10,10)){
                    e->alive = FALSE;
                    b_ptr->alive  = FALSE;
                }

        }

    }
}


struct enemy_t* enemy_select_random_enemy(struct enemy_data *d){

   int row = rand_range(0,ENEMY_ROW);
   int col = rand_range(0,ENEMY_COL);

   struct enemy_t *e = &d->enemies[row][col];
   struct spaceship *ship = ship_get();

   if(!e->alive || e == NULL) return NULL;


    struct bullet_t *bullet = bullet_get_free(e->bullets, ENEMY_MAX_BULLETS);

    float dx = ship->pos.x - e->pos.x;
    float dy = ship->pos.y  - e->pos.y;
    float a = sqrt(dx*dx + dy*dy);

    bullet->alive = TRUE;
    bullet->pos = e->pos;
    bullet->vel.x =  0 * cos(a);
    bullet->vel.y = 0.5 * sin(a);


    return e;

}

void enemy_update(struct enemy_data *en, double delta){

    struct spaceship *s = ship_get();

    ENEMY_LOOP(en->enemies, en->cols, en->rows)
    {
        if(e->alive && (~e->flags & ENEMY_FLAG_BONUS_MODE)){

            e->vel.x = e->direction * 0.5 * delta;
            e->vel.y = e->line  * 0.5 * delta;

            //e->pos.x += e->vel.x;
            //e->pos.y += e->vel.y;

            enemy_player_bullet_collision(e,s);

            if(e->shoot_time > 0){
                bullet_update(e->bullets, ENEMY_MAX_BULLETS, delta);
                e->shoot_time--;
            }

        }

    }
    ENEMY_LOOP_END()
}
void enemy_draw(struct enemy_data *en, SDL_Texture *tex){

    ENEMY_LOOP(en->enemies, en->cols, en->rows)
    {

        if(e->alive){

            SDL_FRect dst = {
                .x = e->pos.x,
                .y = e->pos.y,
                .w = 16,
                .h = 16,
            };

            SDL_RenderCopyExF(gamewindow.renderer, tex, NULL, &dst, 0.0, NULL, SDL_FLIP_NONE);
            bullet_draw(e->bullets, ENEMY_MAX_BULLETS, tex);
        }

    }
    ENEMY_LOOP_END()

}

struct enemy_t *enemy_get_free(struct enemy_data *en){

    ENEMY_LOOP(en->enemies, en->cols, en->rows)
    {
        if(!e->alive){
           return e;
        }
    }
    ENEMY_LOOP_END()
    return NULL;
}

void enemy_init_array(struct enemy_t *e_list, int max){
    for(int i = 0; i < max;i++){
        e_list[i].alive = FALSE;
        e_list[i].direction = 1;
    }
    return;
}

