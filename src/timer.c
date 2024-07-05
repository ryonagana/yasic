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
Uint32 GameTimer_GetTicks(GAMETIMER *timer){
    Uint32 actual_time = 0;

    if(timer->is_started){

        if(timer->is_paused){
            actual_time = timer->paused_ticks;
            return actual_time;
        }


        actual_time = SDL_GetTicks() - timer->start_ticks;
    }

    return actual_time;
}
