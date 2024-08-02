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
        e->pos.x = (i * ENEMY_COL) * 2;
        e->pos.y = (j * ENEMY_COL) * 2;
        e->alive = TRUE;
        e->direction = 1;
        e->line = 0;
    }
    ENEMY_LOOP_END()

    en->cols = max_col;
    en->rows = max_row;
}


void enemy_update(struct enemy_data *en, float delta){


    ENEMY_LOOP(en->enemies, en->cols, en->rows)
    {
        if(e->alive){

            e->vel.x = e->direction * 0.5 * delta;
            e->vel.y = e->line  * 0.5 * delta;

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

        }

    }
    ENEMY_LOOP_END()
}
