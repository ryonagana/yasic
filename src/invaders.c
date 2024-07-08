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

int shot_time_test = 60;

GAMETIMER game_timer;

void Invaders_Start(void){
    resources_init();
    LVL_Init(&game_level, enemies);
    LVL_Start(enemies);
    Player_Init(&player);

    GameTimer_Init(&game_timer);
    GameTimer_SetFPS(&game_timer, 60.0f);
    GameTimer_Start(&game_timer);

    int total = Enemy_AliveCount(enemies);

    SDL_Log("%d", total);

    Player_SpawnPos(&player, Dsp_GetWindowWidth() / 2 - 32, Dsp_GetWindowHeight() - 64);


}


void InvadersHandleKeyboard(SDL_Event *e){
    if(e->type == SDL_KEYUP){
        if(e->key.repeat == 0){
            Keyboard_PollKeyUp(e);
        }
    }

    if(e->type == SDL_KEYDOWN){
        if(e->key.repeat == 0){
           Keyboard_PollKeyDown(e);
        }
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
        Enemy_Update(enemies,  dt);
        Player_Update(&player, dt);

        SDL_SetRenderTarget(g_display.renderer, g_display.screen);
        SDL_RenderClear(g_display.renderer);
        Enemy_Render(enemies);
        Player_Render(&player);
        SDL_SetRenderTarget(g_display.renderer, NULL);
        Dsp_Render();

        if(Keyboard_isPressed(SDL_SCANCODE_A)){
            Player_MoveLeft(&player);
        }else if(Keyboard_isPressed(SDL_SCANCODE_D)){
            Player_MoveRight(&player);
        }

        if(Keyboard_isPressed(SDL_SCANCODE_SPACE)){
            Player_Shoot(&player);
        }
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


