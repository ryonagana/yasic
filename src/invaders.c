#include "win.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "video.h"
#include "utils.h"
#include "ship.h"
#include "enemy.h"


#define GAMESTATE_GAMEOVERSCREEN 0
#define GAMESTATE_PLAYSCREEN  1
#define GAMESTATE_SCORESCREEN 2
#define GAMESTATE_INTRO1 3


#define MAX_BONUS_ENEMIES 4

int keys[255];

struct enemy_data enemies;

SDL_Texture *enemy_spr;
int gamestate = 1;
int bonus_enemy_event = FALSE;


struct enemy_t bonus_enemies[MAX_BONUS_ENEMIES];

int rect_collision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
    if(x1 + w1 > x2 &&
       x1 < x2 + w2 &&
       (y1 + h1) > y2 &&
       y1 < y2 + h2){
        return TRUE;
    }

    return FALSE;
}


int rand_range(int min, int max){
    return rand() % (max-min+1)+min;
}

int rand_num(int m){
    return (rand()%m)+1;
}


int KeyPressed(int key){
    return (keys[key] & KEY_PRESSED) == 0;
}

int KeyReleased(int key){
    return (keys[key] & KEY_RELEASED) == 0;
}


void invaders_shoot_player(void){
    int r = rand_range(0,100);

    if(r < 30){
        struct enemy_t*  e  = enemy_select_random_enemy(&enemies);
        if(e == NULL) return;

        e->shoot_time = rand_range(30,100);
    }



}


void invaders_create_bonus_enemy(double delta)
{

   Uint32 tm = SDL_GetTicks() / 1000;
   static struct enemy_t *enemy =  NULL;
   int w,h;


    video_display_size(&w, &h);

    int m = rand_num(100);

    if( (tm%4) == 0 && tm > 0 && !bonus_enemy_event && m <= 10){
        SDL_Log("Evento Sucesso!");
        struct enemy_t *e = enemy_get_free(&enemies);

        if(NULL == e)
            return;


        e->pos.x = w-16;
        e->pos.y = 0;
        e->vel.x = (.03f * delta);
        e->vel.y = 0;
        e->alive = TRUE;
        e->direction  = 1;
        e->flags |= ENEMY_FLAG_BONUS_MODE;
        bonus_enemy_event = TRUE;
        enemy = e;

    }

    if(bonus_enemy_event && enemy != NULL){
        enemy->pos.x -= enemy->vel.x;
        struct spaceship* ship = ship_get();
        enemy_player_bullet_collision(enemy, ship);

        if(enemy->pos.x < 0 || enemy->pos.x > w-16){


            Uint32 tick = SDL_GetTicks() / 1000;

            if( (tick % 2) && bonus_enemy_event && enemy != NULL){
                bonus_enemy_event = FALSE;
                enemy->alive = FALSE;
                enemy->flags &= ENEMY_FLAG_BONUS_MODE;
                enemy = NULL;
            }
        }
        //enemy->pos.y += enemy->vel.y;
    }
}


int  invaders_update(double delta){

    switch(gamestate){
        case GAMESTATE_PLAYSCREEN:
        {
            invaders_create_bonus_enemy(delta);
            invaders_shoot_player();
            ship_update(delta);
            enemy_update(&enemies, delta);
        }
        break;
    }

    return gamestate;
}


int invaders_draw(void){

    switch(gamestate){
        case GAMESTATE_PLAYSCREEN:
        {
            ship_draw();
            enemy_draw(&enemies, enemy_spr);
        }
        break;
    }

    return gamestate;
}

int main(int argc, char *argv[]){

    UNUSED(argc);
    UNUSED(argv);
    srand(time(NULL));

    window_init();
    window_display_create(800,600,FALSE,TRUE, "INVADERS!");

    int close = 0;
    int vsync_on = TRUE;

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double deltaTime = 0;
    char fps_txt[125];
    ship_init();

    enemy_init(&enemies, 8,8);
    SDL_Surface *enemy_spr_surf = NULL;
    enemy_spr_surf = IMG_Load("assets//enemy01.bmp");
    create_colorkey(enemy_spr_surf, SDL_MapRGB(enemy_spr_surf->format, 255,0,255));

    enemy_spr = convert_surface_to_texture(enemy_spr_surf); //load_texture_from_filepath("assets//enemy01.bmp", NULL);
    SDL_FreeSurface(enemy_spr_surf);

    memset(keys, 0x0, sizeof(keys));
    enemy_init_array(bonus_enemies, MAX_BONUS_ENEMIES);

    while(!close){

        SDL_Event e;
        struct spaceship *ship = ship_get();
        now = SDL_GetPerformanceCounter();
        deltaTime = (double) ((now-last)*1000) / (double) SDL_GetPerformanceFrequency();

        snprintf(fps_txt, sizeof(fps_txt), "%.2f", deltaTime);
        SDL_SetWindowTitle(gamewindow.window, fps_txt);


        while(SDL_PollEvent(&e) != 0){

            if(e.type == SDL_QUIT){
                close = 1;
                break;
            }

            if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_a:
                    ship->control.left = 1;
                    break;

                    case SDLK_d:
                        ship->control.right = 1;
                        break;
                    case SDLK_SPACE:
                        ship->control.shoot = 1;
                        break;
                }
            }

            if(e.type == SDL_KEYUP){
                switch(e.key.keysym.sym){
                    case SDLK_a:
                        ship->control.left = 0;
                        break;

                    case SDLK_d:
                        ship->control.right = 0;
                        break;

                    case SDLK_SPACE:
                        ship->control.shoot = 0;
                        break;

                }
            }
        }

        fprintf(stderr, "left %d\n", ship->control.left);

        gamestate = invaders_update(deltaTime);

        SDL_SetRenderTarget(gamewindow.renderer, gamewindow.screen);
        SDL_RenderClear(gamewindow.renderer);
        invaders_draw();
        SDL_SetRenderTarget(gamewindow.renderer, NULL);

        video_render();
        last = now;

        if(!vsync_on){
            float elapsed = (last -now) / SDL_GetPerformanceFrequency() * 1000;
            SDL_Delay(floor(1.0/60.0) - elapsed);
        }
    }

    ship_unload();
    window_unload();

    return 0;
}
