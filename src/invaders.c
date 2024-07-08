#include "invaders.h"
#include "display.h"
#include "level.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "resources.h"
#include "utils.h"
#include "timer.h"
#include "keyboard.h"


static LEVEL game_level;
static ENEMY enemies[ENEMY_ROWS][ENEMY_COLS];
static PLAYER player;
//static TBULLETS bullets;

uint8_t keys[227];
uint8_t released_keys[227];
uint8_t pressed_keys[227];
int shot_time_test = 60;

GAMETIMER game_timer;

void Invaders_Start(void){
    resources_init();
    LVL_Init(&game_level, enemies);
    LVL_Start(enemies);
    Player_Init(&player);

    //Bullet_Init(&bullets, 10);

    GameTimer_Init(&game_timer);
    GameTimer_SetFPS(&game_timer, 60.0f);
    GameTimer_Start(&game_timer);

    memset(pressed_keys, 0, sizeof(pressed_keys));
    memset(released_keys, 0, sizeof(released_keys));

    int total = Enemy_AliveCount(enemies);

    printf("%d", total);

    Player_SpawnPos(&player, Dsp_GetWindowWidth() / 2 - 32, Dsp_GetWindowHeight() - 64);


}


void InvadersHandleKeyboard(SDL_Event *e){
    if(e->type == SDL_KEYUP){
        if(e->key.repeat == 0){

        }
    }

    if(e->type == SDL_KEYDOWN){

    }
}

void Invaders_Loop(void){


    int window_close = FALSE;


    while(!window_close){
        SDL_Event event;
        float dt = GameTimer_GetDelta(&game_timer);



        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE){
                window_close = TRUE;
            }

            InvadersHandleKeyboard(&event);
        }

        LVL_Update(&game_level, enemies);
        Enemy_Update(enemies, dt);
        Player_Update(&player);

        SDL_SetRenderTarget(g_display.renderer, g_display.screen);
        SDL_RenderClear(g_display.renderer);
        Enemy_Render(enemies);
        Player_Render(&player);
        SDL_SetRenderTarget(g_display.renderer, NULL);
        Dsp_Render();

        /*
        if(Keyboard_IsPressed(SDLK_UP)){
            SDL_Log("UP!");
        }
        */

        GameTimer_UpdateTicks(&game_timer);


    }

/*
    while(!InvadersHandleLoop(deltaTime)){
        deltaTime = (SDL_GetTicks() - prevFrameTime ) / 1000.0f;
        prevFrameTime = SDL_GetTicks();


        Enemy_Update(enemies,  deltaTime);

        SDL_SetRenderTarget(g_display.renderer, g_display.screen);
        SDL_RenderClear(g_display.renderer);

        Enemy_Render(enemies);
        SDL_SetRenderTarget(g_display.renderer, NULL);

        Dsp_Render();


        if(g_display.vsync){
            Dsp_CapFrameRate(prevFrameTime);
        }
    }
    */

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
