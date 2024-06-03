#include "enemy.h"
#include <allegro5/allegro_primitives.h>
#include "level.h"
#include "display.h"


#define ENEMY_DIR_RIGHT (1)
#define ENEMY_DIR_LEFT  (-1)

static ALLEGRO_BITMAP *tmp_enemies = NULL;
//static int s_enemies_direction = 1;
//static int s_enemy_state = 0;


static int S_Enemy_AliveCount(ENEMY (*enemies)[ENEMY_COLS]){
  int count = 0;

  for(int row = 0; row < ENEMY_ROWS;row++){
    for(int col = 0; col < ENEMY_COLS;col++){
        ENEMY *e = &enemies[row][col];
        if(e->alive){
                count++;
        }
    }
  }

  return count;
}

void Enemy_Update(ENEMY (*enemies)[ENEMY_COLS]){

  for(int row = 0; row < ENEMY_ROWS;row++){
    for(int col = 0; col < ENEMY_COLS;col++){
        ENEMY *e = &enemies[row][col];

        if(e->x >= Dsp_GetWindowWidth() - TILE - 1 && e->direction == ENEMY_DIR_RIGHT){
            e->direction = -1;
            e->y += TILE;
        }

        if(e->x <= 0 && e->direction == ENEMY_DIR_LEFT){
            e->direction = 1;
            e->y += TILE;
        }

        e->x += 6 * e->vx * e->direction;
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
            }
        }
      }
}

void Enemy_Shutdown(void){
    if(tmp_enemies) al_destroy_bitmap(tmp_enemies);
}
