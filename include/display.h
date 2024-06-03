#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#define FPS 60

int  Dsp_InitVideo(void);
void Dsp_ShutdownVideo(void);
void Dsp_getMonitorInfo(int adapter, int *w, int *h);
void Dsp_RegisterEvents(void);
void Dsp_CreateDisplay(int w, int h, int fullscreen, int enable_vsync, const char *window_title);
void Dsp_RenderNoise(void);

int Dsp_GetWindowWidth(void);
int Dsp_GetWindowHeight(void);
int Dsp_GetTimerSecs(void);
int Dsp_GetTimerTicks(void);
double Dsp_GetTime(void);

typedef struct {
    ALLEGRO_DISPLAY *dsp;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP *screen;
}DISPLAY;

extern DISPLAY g_display;

#endif // DISPLAY_H
