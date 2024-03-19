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

#define VERSION_MAJOR 0
#define VERSION_MINOR 2
#define VERSION_BUILD 2

#define TRUE  (1ul)
#define FALSE (0l)
#define TILE 32
#define UNUSED(x) ((void)x)

#define ENEMY_ROW_X 10
#define ENEMY_ROW_Y 5
#define GAME_DATAFILES "game.pk0"
#define HISCORE_DATA "hiscore.dat"


#define PI  ALLEGRO_PI
#define PI2 (ALLEGRO_PI*2)

#define RAD2DEG 57.29578049
#define DEG2RAD 0.017453292

#define FPS 60.0

#define COLOR_WHITE al_map_rgb(255,255,255)
#define COLOR_RED al_map_rgb(255,0,0)
#define COLOR_MAGENTA al_map_rgb(255,9,255)
#define COLOR_ORANGE al_map_rgba(241, 196, 15,255)

extern int gameover;
extern int line;
extern int enemy_shoot_time;
extern int game_difficulty;
extern int g_demo_start;

int  game_rand(int num);
int game_rand_range(int n, int m);
int rect_collision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);
