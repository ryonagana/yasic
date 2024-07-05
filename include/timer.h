#ifndef TIMER_H
#define TIMER_H
#include <SDL2/SDL.h>

typedef struct GAMETIMER {
    Uint32 start_ticks;
    Uint32 paused_ticks;

    int is_paused;
    int is_started;

}GAMETIMER;


void GameTimer_Init(GAMETIMER *timer);
void GameTimer_Start(GAMETIMER *timer);
void GameTimer_Stop(GAMETIMER *timer);
void GameTimer_Pause(GAMETIMER *timer);
void GameTimer_Unpause(GAMETIMER *timer);
Uint32 GameTimer_GetTicks(GAMETIMER *timer);



#endif // TIMER_H
