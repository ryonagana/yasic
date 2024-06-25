#include "enemy.h"
#include <allegro5/allegro_primitives.h>
#include "level.h"
#include "display.h"



#define ENEMY_LOOP(type, ptr) for(int i = 0; i <  ENEMY_ROWS; i++){\
                          for(int j = 0; j < ENEMY_COLS;j++){\
                             type *e = &(ptr)[i][j];\


#define ENEMY_LOOP_END()  }\
                        }



static ALLEGRO_BITMAP *tmp_enemies = NULL;

void Enemy_Init(ENEMY (*enemies)[ENEMY_COLS]){

    ENEMY_LOOP(ENEMY, enemies)
    {
        e->alive = 1;
        e->direction = ENEMY_DIR_LEFT;
        e->flags = 0;
        e->vx = 1.0f;
        e->vy = 1.0f;
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

/*
void Enemy_CorrectPosition(ENEMY (*enemies)[ENEMY_COLS]){

    ENEMY_LOOP(ENEMY, enemies)
    {
        if(e->alive){
            e->x = (e->x * TILE) * 1.2f;
            e->y = (e->y * TILE) * 1.2f;

        }
    }
    ENEMY_LOOP_END();


}
*/


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
       if(NULL == tmp_enemies){
            tmp_enemies  = al_create_bitmap(TILE,TILE);
            al_set_target_bitmap(tmp_enemies);
            al_clear_to_color(al_map_rgb(255,0,0));
            al_set_target_backbuffer(g_display.dsp);
       }

      for(int row = 0; row < ENEMY_ROWS;row++){
        for(int col = 0; col < ENEMY_COLS;col++){
            ENEMY *e = &enemies[row][col];

            if(e->alive){
                    al_draw_bitmap(tmp_enemies,  e->x,e->y,0);
            }
        }
      }
}

void Enemy_Shutdown(void){
    if(tmp_enemies) al_destroy_bitmap(tmp_enemies);
}
