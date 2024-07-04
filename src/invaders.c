#include "invaders.h"
#include "display.h"
#include "level.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "resources.h"
#include "utils.h"



static LEVEL game_level;
static ENEMY enemies[ENEMY_ROWS][ENEMY_COLS];
static PLAYER player;
//static TBULLETS bullets;

uint8_t keys[227];
uint8_t released_keys[227];
uint8_t pressed_keys[227];

int shot_time_test = 60;

void Invaders_Start(void){
    resources_init();
    LVL_Init(&game_level, enemies);
    LVL_Start(enemies);
    Player_Init(&player);

    //Bullet_Init(&bullets, 10);

    memset(pressed_keys, 0, sizeof(pressed_keys));
    memset(released_keys, 0, sizeof(released_keys));

    int total = Enemy_AliveCount(enemies);

    printf("%d", total);

    Player_SpawnPos(&player, Dsp_GetWindowWidth() / 2 - 32, Dsp_GetWindowHeight() - 64);


}

int InvadersHandleLoop(float delta){
    SDL_Event e;
    SDL_PollEvent(&e);


    if(e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE){
        return 1;
    }

    return 0;

}

void Invaders_Loop(void){


    float deltaTime = 0;
    Uint32 prevFrameTime = 0;


    while(!InvadersHandleLoop(deltaTime)){
        deltaTime = (SDL_GetTicks() - prevFrameTime ) / 1000.0f;
        prevFrameTime = SDL_GetTicks();


        Enemy_Update(enemies, deltaTime);

        SDL_SetRenderTarget(g_display.renderer, g_display.screen);
        SDL_RenderClear(g_display.renderer);

        Enemy_Render(enemies);
        SDL_SetRenderTarget(g_display.renderer, NULL);

        Dsp_Render();


        if(g_display.vsync){
            Dsp_CapFrameRate(prevFrameTime);
        }
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
