#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define FPS 60

int  Dsp_InitVideo(void);
void Dsp_ShutdownVideo(void);
void Dsp_getMonitorInfo(int adapter, int *w, int *h);
void Dsp_RegisterEvents(void);
void Dsp_CreateDisplay(int w, int h, int fullscreen, int enable_vsync, const char *window_title);
void Dsp_RenderNoise(void);
void Dsp_CreateScreenBuffer(int w, int h);
void Dsp_CapFrameRate(Uint32 deltatime);

int Dsp_GetWindowWidth(void);
int Dsp_GetWindowHeight(void);
int Dsp_GetTimerSecs(void);
int Dsp_GetTimerTicks(void);
double Dsp_GetTime(void);
void Dsp_Render(void);

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Texture *screen;
    int width;
    int height;
    int vsync;
}DISPLAY;

extern DISPLAY g_display;

#endif // DISPLAY_H
