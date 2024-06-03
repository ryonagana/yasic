#ifndef GAME_RENDER_HEADER
#define  GAME_RENDER_HEADER

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_physfs.h>
#include <physfs.h>


extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_EVENT_QUEUE *queue;
extern ALLEGRO_TIMER *timer;
extern ALLEGRO_BITMAP *screen;
extern ALLEGRO_BITMAP *buffer;
extern ALLEGRO_FONT *debug_font;

int allegro_init(char *error_message);

#endif // GAME_RENDER_HEADER
