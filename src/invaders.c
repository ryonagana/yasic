#include "invaders.h"
#include "display.h"
#include "level.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"

static int s_close = 0;
static int s_redraw = 0;

static LEVEL game_level;
static ENEMY enemies[ENEMY_ROWS][ENEMY_COLS];
static PLAYER player;
static TBULLETS bullets;

uint8_t keys[ALLEGRO_KEY_MAX];
uint8_t released_keys[ALLEGRO_KEY_MAX];
uint8_t pressed_keys[ALLEGRO_KEY_MAX];

int shot_time_test = 60;

void Invaders_Start(void){
    LVL_Init(&game_level, enemies);
    LVL_Start(enemies);
    Player_Init(&player);

    Bullet_Init(&bullets, 10);

    memset(pressed_keys, 0, sizeof(pressed_keys));
    memset(released_keys, 0, sizeof(released_keys));

    int total = Enemy_AliveCount(enemies);

    printf("%d", total);

    Player_SpawnPos(&player, Dsp_GetWindowWidth() / 2 - 32, Dsp_GetWindowHeight() - 64);


}
void Invaders_Loop(void){

    while(!s_close){
        ALLEGRO_EVENT e;

        if(s_redraw && al_event_queue_is_empty(g_display.queue)){
            s_redraw = 0;
            al_clear_to_color(al_map_rgb(0,0,0));

            al_set_target_bitmap(g_display.screen);
            al_clear_to_color(al_map_rgb(0,0,0));
            //Dsp_RenderNoise();
            Enemy_Render(enemies);
            Player_Render(&player);

            Bullet_Draw(&bullets, NULL);


            LVL_RenderGrid();
            al_set_target_backbuffer(g_display.dsp);


            al_draw_bitmap(g_display.screen,0,0,0);

            al_flip_display();
        }

        //deplete all event queue before render
        do {
            al_wait_for_event(g_display.queue, &e);

            switch(e.type){
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    s_close = 1;
                    break;


                case ALLEGRO_EVENT_KEY_DOWN:
                    {
                        pressed_keys[e.keyboard.keycode] = 1;
                        released_keys[e.keyboard.keycode] = 0;
                    }
                    break;

                case ALLEGRO_EVENT_KEY_UP:
                    {
                        pressed_keys[e.keyboard.keycode] = 0;
                        released_keys[e.keyboard.keycode] = 1;
                    }
                    break;

                case ALLEGRO_EVENT_TIMER:
                    {
                        if(player.state == 1){
                            game_level.state =  LEVEL_STATE_MARCH_WALK;
                        }

                        if(KeyDown(ALLEGRO_KEY_LEFT)){
                            Player_MoveLeft(&player);
                        }

                        if(KeyDown(ALLEGRO_KEY_RIGHT)){
                            Player_MoveRight(&player);
                        }


                        Player_Update(&player);
                        //Enemy_Update(enemies);
                        Bullet_Update(&bullets);


                        if(shot_time_test > 0){
                            shot_time_test--;
                        }

                        if(shot_time_test == 0){
                            //todo
                            BULLET * shoot = Bullet_FindFree(&bullets);

                            if(shoot){
                                shoot->alive = 1;
                                shoot->vx = 0;
                                shoot->vy = -1;
                                shoot->x = player.x;
                                shoot->y = player.y;
                                //Bullet_SetGravity(shoot, 0.06f);
                                //Bullet_SetFlag(shoot, BULLET_GRAVITY_AFFECTED);
                            }
                            shot_time_test = 60;
                        }

                        LVL_Update(&game_level, enemies);

                        s_redraw = 1;
                    }
                    break;
            }

        }while(!al_event_queue_is_empty(g_display.queue));





    }

}
void Invaders_Shutdown(void){
    Enemy_Shutdown();
}


int KeyDown(int key)
{
    return pressed_keys[key];
}
int KeyUp(int key)
{
    return released_keys[key];
}
