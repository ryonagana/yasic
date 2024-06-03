#include "enemy.h"
#include <allegro5/allegro_primitives.h>
#include "level.h"
#include "display.h"


static ALLEGRO_BITMAP *tmp_enemies = NULL;
static int enemies_direction = 1;


static void S_Enemy_MoveDown(ENEMY (*enemies)[ENEMY_COLS], int direction){
  for(int row = 0; row < ENEMY_ROWS;row++){
    for(int col = 0; col < ENEMY_COLS;col++){
            ENEMY *e = &enemies[row][col];
            e->y += TILE<<1;

    }
  }

  enemies_direction = direction;
}

void Enemy_Update(ENEMY (*enemies)[ENEMY_COLS]){

  for(int row = 0; row < ENEMY_ROWS;row++){
    for(int col = 0; col < ENEMY_COLS;col++){
        ENEMY *e = &enemies[row][col];

        if(e->x  > Dsp_GetWindowWidth()){
            S_Enemy_MoveDown(enemies, -1);
        }

        if(e->x  < 0){
            S_Enemy_MoveDown(enemies, 1);
        }

        e->x += 6 * e->vx * enemies_direction;

    }
  }
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
                    al_draw_bitmap(tmp_enemies,  e->x * e->vx ,e->y * e->vy,0);
                //al_draw_filled_rectangle(e->x, e->y, e->x + col * TILE , e->y * row + TILE, al_map_rgb(255,0,0));
            }
        }
      }
}

void Enemy_Shutdown(void){
    if(tmp_enemies) al_destroy_bitmap(tmp_enemies);
}
