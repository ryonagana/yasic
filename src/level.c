#include "level.h"
#include "enemy.h"
#include "display.h"
#include <string.h>
#include <allegro5/allegro_primitives.h>

static int s_shoot_timer_list[LEVEL_DIFFICULT_NIGHTMARE+1] = {
    150,
    120,
    90,
    80,
    50
};

static int s_shoot_counter_list[LEVEL_DIFFICULT_NIGHTMARE+1] = {
    1,
    2,
    3,
    5,
    7
};


static float march_walk_speed_list[LEVEL_DIFFICULT_NIGHTMARE+1] = {
    1.0,
    1.0,
    10.0,
    11.0,
    11.0
};



void LVL_Init(LEVEL *lvl, ENEMY (*enemies_list)[ENEMY_COLS]){
    lvl->enemies = *enemies_list;
    lvl->difficulty  = LEVEL_DIFFICULT_EASY;
    lvl->enemies_shoot_count = s_shoot_counter_list[lvl->difficulty % (LEVEL_DIFFICULT_NIGHTMARE+1)];
    lvl->shoot_counter = s_shoot_timer_list[lvl->difficulty % (LEVEL_DIFFICULT_NIGHTMARE+1)];
    lvl->march_walk_time = 0;
    lvl->march_walk_delay = 10;
    lvl->state = LEVEL_STATE_STOPPED;
    memset(lvl->enemies, 0, sizeof(lvl->enemies) * ENEMY_ROWS * ENEMY_COLS);
}


void LVL_Start(ENEMY (*enemies)[ENEMY_COLS]){

    Enemy_Init(enemies);
    /*
    for(int y = 0; y < ENEMY_COLS;y++){
        for(int x = 0;x < ENEMY_ROWS;x++){
            ENEMY *e = &enemies[y][x];



            e->x = (x * TILE) * 1.2f;
            e->y = (y * TILE) * 1.2f;
            //e->x = (x * TILE) * 1.5;
            //e->y = (y * TILE) * 1.5;
            e->alive = 1;
            e->flags = 0;
            e->vx = 1;
            e->vy = 1;
            e->direction = ENEMY_DIR_RIGHT;
        }
    }
    */
}


void LVL_Update(LEVEL *level, ENEMY (*enemies)[ENEMY_COLS]){

    //if(level->march_walk_time == 0 && level->state == LEVEL_STATE_MARCH_WALK){

        for(int row = 0; row < ENEMY_ROWS;row++){
            for(int col = 0; col < ENEMY_COLS;col++){
                ENEMY *e = &enemies[row][col];


                if(e->x >= Dsp_GetWindowWidth() - TILE - 1 && e->direction == ENEMY_DIR_RIGHT){

                    Enemy_ChangeDirection(enemies, e->direction);
                    Enemy_MoveDown(enemies,level);
                    //Enemy_CorrectPosition(enemies);
                    continue;
                }

                if(e->x <= 0 && e->direction == ENEMY_DIR_LEFT){

                    Enemy_ChangeDirection(enemies, e->direction);
                    Enemy_MoveDown(enemies,level);
                    //Enemy_CorrectPosition(enemies);
                    continue;
                }

                 e->vx = march_walk_speed_list[level->difficulty];
                 e->x += e->vx * e->direction * ENEMY_GRID_SPACE;
            }
        }

   //     level->march_walk_time = level->march_walk_delay;
    //}

   // if(level->march_walk_time > 0 && level->state == LEVEL_STATE_MARCH_WALK){
        level->march_walk_time--;
   // }



}

void LVL_RenderGrid(void){

    int tx,ty;
    //ALLEGRO_VERTEX vtx[255];
    ALLEGRO_VERTEX vty[255];


    tx = (Dsp_GetWindowWidth() / TILE);
    ty = (Dsp_GetWindowHeight() / TILE);

    for(int rows = 0 ; rows  < ty; rows++){

         vty[rows].x = tx * TILE; //tx * TILE;
         vty[rows].y = (rows * TILE) + TILE;
         vty[rows].z = 0;
         vty[rows].u = 0;
         vty[rows].v = 0;
         vty[rows].color = al_map_rgb(255,0,255);


    }

    /*
    for(int cols = 0 ; cols < tx; cols++){

         vtx[cols].x = (cols * TILE); //tx * TILE;
         vtx[cols].y = Dsp_GetWindowHeight();
         vtx[cols].z = 0;
         vtx[cols].u = 0;
         vtx[cols].v = 0;
         vtx[cols].color = al_map_rgb(255,0,255);
    }*/



    //al_draw_prim(vtx, NULL, NULL, 0, tx, ALLEGRO_PRIM_LINE_LIST);
    al_draw_prim(vty, NULL, NULL, 0, ty, ALLEGRO_PRIM_LINE_STRIP);

}


void LVL_End(void){


}
