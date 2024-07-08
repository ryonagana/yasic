#ifndef TIMER_H
#define TIMER_H
#include <SDL2/SDL.h>

typedef struct GAMETIMER {
    Uint32 start_ticks;
    Uint32 paused_ticks;
    float deltaTime;
    float lastTime;
    float fpsDesired;
    int is_paused;
    int is_started;

}GAMETIMER;


void GameTimer_Init(GAMETIMER *timer);
void GameTimer_Start(GAMETIMER *timer);
void GameTimer_Stop(GAMETIMER *timer);
void GameTimer_SetFPS(GAMETIMER *timer, float fps);
void GameTimer_Pause(GAMETIMER *timer);
void GameTimer_Unpause(GAMETIMER *timer);
float GameTimer_GetDelta(GAMETIMER *timer);
void GameTimer_UpdateTicks(GAMETIMER *timer);



#endif // TIMER_H
