#include "invaders.h"
#include "display.h"
#include "level.h"
#include "enemy.h"


static int s_close = 0;
static int s_redraw = 0;

static LEVEL game_level;
static ENEMY enemies[ENEMY_ROWS][ENEMY_COLS];


void Invaders_Start(void){
    LVL_Init(&game_level, enemies);
    LVL_Start(enemies);
}
void Invaders_Loop(void){

    while(!s_close){
        ALLEGRO_EVENT e;

        if(s_redraw && al_event_queue_is_empty(g_display.queue)){
            s_redraw = 0;
            al_clear_to_color(al_map_rgb(0,0,0));

            al_set_target_bitmap(g_display.screen);
            al_clear_to_color(al_map_rgb(0,0,0));
            Dsp_RenderNoise();
            Enemy_Render(enemies);

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

                case ALLEGRO_EVENT_TIMER:
                    {
                        Enemy_Update(enemies);
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
