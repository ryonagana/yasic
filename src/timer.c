#include "timer.h"
#include "utils.h"

void GameTimer_Init(GAMETIMER *timer){

    timer->start_ticks =  0;
    timer->paused_ticks = 0;
    timer->is_paused = FALSE;
    timer->is_started = FALSE;

    return;
}
void GameTimer_Start(GAMETIMER *timer){

    timer->is_started = TRUE;
    timer->is_paused =  FALSE;

    timer->start_ticks  = SDL_GetTicks();
    timer->paused_ticks = 0;
    timer->fpsDesired = 60.0f;
    timer->deltaTime = 0;

    return;
}
void GameTimer_Stop(GAMETIMER *timer){

    timer->start_ticks =  0;
    timer->paused_ticks = 0;
    timer->is_paused = FALSE;
    timer->is_started = FALSE;
    return;
}
void GameTimer_Pause(GAMETIMER *timer){
    if(timer->is_started && !timer->is_paused){

        timer->is_paused = TRUE;
        timer->paused_ticks = SDL_GetTicks() - timer->start_ticks;
        timer->start_ticks = 0;
    }
    return;
}
void GameTimer_Unpause(GAMETIMER *timer){
    if(timer->is_started && timer->is_paused){

        timer->is_paused = FALSE;
        timer->start_ticks = SDL_GetTicks() - timer->paused_ticks;
        timer->paused_ticks = 0;
    }

    return;
}
void GameTimer_UpdateTicks(GAMETIMER *timer){
    timer->deltaTime = (SDL_GetTicks() - timer->lastTime) * (timer->fpsDesired/1000.0f);

    if(timer->deltaTime > 1.5f){
        timer->deltaTime = 1.5f;
    }

    timer->lastTime = SDL_GetTicks();

    return;
}

void GameTimer_SetFPS(GAMETIMER *timer, float fps){
    timer->fpsDesired = fps;
}

float GameTimer_GetDelta(GAMETIMER *timer){
    return timer->deltaTime;
}
