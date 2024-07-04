#include "enemy.h"
#include <allegro5/allegro_primitives.h>
#include "level.h"
#include "display.h"
#include "resources.h"


#define ENEMY_LOOP(type, ptr) for(int i = 0; i <  ENEMY_ROWS; i++){\
                          for(int j = 0; j < ENEMY_COLS;j++){\
                             type *e = &(ptr)[i][j];\


#define ENEMY_LOOP_END()  }\
                        }



//static ALLEGRO_BITMAP *tmp_enemies = NULL;

void Enemy_Init(ENEMY (*enemies)[ENEMY_COLS]){

    ENEMY_LOOP(ENEMY, enemies)
    {
        e->alive = 1;
        e->direction = ENEMY_DIR_LEFT;
        e->flags = 0;
        e->vx = 1.0f;
        e->vy = 0.0f;
        e->x = (j * TILE) * ENEMY_GRID_SPACE;
        e->y = (i * TILE) * ENEMY_GRID_SPACE;
    }
    ENEMY_LOOP_END();

}

int Enemy_AliveCount(ENEMY (*enemies)[ENEMY_COLS]){


  int count = 0;


    ENEMY_LOOP(ENEMY,  enemies)
    {
        if(e->alive){
            count++;
        }
    }
    ENEMY_LOOP_END();


  return count;

}

void Enemy_Update(ENEMY (*enemies)[ENEMY_COLS], float deltaTime){
    ENEMY_LOOP(ENEMY, enemies)
    {
        if(e->alive)
        {       e->vx = 2.0 * deltaTime;
                //e->vy = 2.0;

                e->x += e->vx;
                e->y += e->vy;
        }
    }
    ENEMY_LOOP_END();
}

void Enemy_ChangeDirection(ENEMY (*enemies)[ENEMY_COLS], int direction){
    ENEMY_LOOP(ENEMY, enemies)
    {
        if(e->alive){


            if(direction == ENEMY_DIR_RIGHT){
                e->vx *=  ENEMY_DIR_LEFT * ENEMY_GRID_SPACE ;
                e->direction = ENEMY_DIR_LEFT;
            }

            if(direction == ENEMY_DIR_LEFT){
                e->vx *=  ENEMY_DIR_LEFT * ENEMY_GRID_SPACE;
                e->direction = ENEMY_DIR_RIGHT;
            }

        }
    }
    ENEMY_LOOP_END();
}


void Enemy_MoveDown(ENEMY (*enemies)[ENEMY_COLS], LEVEL *level){

    ENEMY_LOOP(ENEMY, enemies)
    {
        if(e->alive){
            e->y += TILE * ENEMY_GRID_SPACE;
        }
    }
    ENEMY_LOOP_END();

}



void Enemy_Render(ENEMY (*enemies)[ENEMY_COLS]){

      for(int row = 0; row < ENEMY_ROWS;row++){
        for(int col = 0; col < ENEMY_COLS;col++){
            ENEMY *e = &enemies[row][col];


            if(e->alive){

                SDL_Texture* enemy = resources_get_sprite(SPRITE_ENEMY01_SPR);
                SDL_Rect r = (SDL_Rect){e->x, e->y, 32,32 };
                SDL_RenderCopyEx(g_display.renderer, enemy, NULL, &r , 0, NULL, SDL_FLIP_NONE);

            }

        }
      }
}

void Enemy_Shutdown(void){
    //if(tmp_enemies) al_destroy_bitmap(tmp_enemies);
    return;
}
