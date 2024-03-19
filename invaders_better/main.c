#include <stdio.h>
#include <math.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "main.h"
#include "g_render.h"
#include "g_sound.h"
#include "g_particles.h"
#include "g_item.h"
#include "g_player.h"
#include "g_enemies.h"
#include "g_sprites.h"

#include "miniz.h"

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

static  int g_close_game = 0;

ALLEGRO_MUTEX *key_mutex = NULL;
char keybuffer[255] = {0};
int keybuffer_counter = 0;

#define PI  ALLEGRO_PI
#define PI2 (ALLEGRO_PI*2)

#define RAD2DEG 57.29578049
#define DEG2RAD 0.017453292

#define FPS 60.0

#define COLOR_WHITE al_map_rgb(255,255,255)
#define COLOR_RED al_map_rgb(255,0,0)
#define COLOR_MAGENTA al_map_rgb(255,9,255)
#define COLOR_ORANGE al_map_rgba(241, 196, 15,255)

static int screen_width  = 0;
static int screen_height = 0;

#define MAX_BULLETS 50

#define DEFAULT_PARTICLES 2000
#define MAX_PARTICLES     10000



PARTICLE particles[MAX_PARTICLES] = {0};

enum SPRITE_INTRO {
    SPR_INTRO_CREATOR,
    SPR_INTRO_ROYALTIES,
    SPR_INTRO_TOTAL
};

ALLEGRO_BITMAP *spr_intro[10];



static int enemy_wave = 1;
static int enemy_wave_time = 0;
static int enemy_wave_time_total = 0;

void wave_reset(void);


#define MAX_ITEM_LIST 8


static ITEM item_list[MAX_ITEM_LIST] = {0};



static PLAYER player;
static ENEMY enemies[ENEMY_ROW_Y][ENEMY_ROW_X];



#define SPRITE_MAX 12


typedef struct TEXT {
    float x, y;
    char text[255];
    int ttl;
    int shadow;
    ALLEGRO_COLOR color;
}TEXT;


static TEXT score_list[10] = {0};



int gameover = 0;
static int g_game_started  = FALSE;
int player_keys[ALLEGRO_KEY_MAX] = {0};
static int player_released_keys[ALLEGRO_KEY_MAX]  = {0};
int line = 0;
int  enemy_shoot_time = 70;
static int g_game_paused = FALSE;





int game_difficulty = DIFF_NOVICE;


DIFFICULTY_PARAMS difficulty[DIFF_COUNT] = {
    {
        DIFF_NOVICE,
        2.2,
        100,
        2.0,
        1
    },

    {
        DIFF_EASY,
        3.2,
        100,
        2.0,
        1
    },

    {
        DIFF_NORMAL,
        3.8,
        90,
        2.5f,
        2
    },

    {
        DIFF_HARD,
        4.5,
        80,
        2.5f,
        4
    },

    {
        DIFF_NIGHTMARE,
        6.0,
        70,
        3.00,
        4
    }

};



typedef struct STAR {
        POINT pos;
        POINT vel;
        int ttl;
}STAR;

ALLEGRO_BITMAP *stars_bg = NULL;

#define MAX_STARS 3000
#define MAX_STARS_PART (int)(MAX_STARS/3)

static STAR star_list[3][MAX_STARS_PART];

enum FONT_ID {

    FONT_PIXEL_SMALL,
    FONT_PIXEL_BIG,
    FONT_PIXEL_MENU_BIG,
    FONT_PIXEL_MENU_SMALL,
    FONT_COUNT

};

#define MAX_FONTS 6
ALLEGRO_FONT *font_list[MAX_FONTS] = {0};

int g_count_init_round = 0;
int g_ship_counter = 0;
int g_ship_active = FALSE;
ENEMY *g_spaceship_entity = NULL;

enum GAMESTATE_TYPE {
        GAMESTATE_TYPE_MENU,
        GAMESTATE_TYPE_GAMEPLAY,
        GAMESTATE_TYPE_HISCORE,
        GAMESTATE_TYPE_OPTIONS,
        GAMESTATE_TYPE_GAMEOVER,
        GAMESTATE_TYPE_USER_HISCORE,
        GAMESTATE_TYPE_INTROS
};

static int g_gamestate = GAMESTATE_TYPE_MENU;
void new_game(int start);

void gameover_init(void);
void gameover_update(void);
void gameover_draw(void);

enum MENU_TYPE{
        MENU_TYPE_SINGLE_BUTTON,
        MENU_TYPE_SUBMENU
};

typedef struct MENU {
    int menu_id;
    char opt_name[255];
    void (*menu_callback)(struct MENU *menu, void *args);
    int type; // new page or just run;
    struct MENU* next_page;
    float x;
    float y;
}MENU;


typedef struct MENU_CURSOR {
    float x;
    float y;
    int opt;
    int menu_count;
    ALLEGRO_BITMAP *cursor;
}MENU_CURSOR;

#define MAX_MENU 16

static void menu_start_game_click(struct MENU *menu, void *args);
static void menu_hiscore_click(struct MENU *menu, void *args);
static void menu_quit_click(struct MENU *menu, void *args);
static void menu_continue_game_click(struct MENU *menu, void *args);


MENU game_menu[MAX_MENU] = {
        {1, "START GAME\0", menu_start_game_click, 0, NULL,0,0},
        {2, "CONTINUE GAME\0",menu_continue_game_click, 0, NULL,0,0},
        {3, "HI-SCORE\0", menu_hiscore_click, 0, NULL,0,0},
        {4, "ABOUT\0", NULL, 1, NULL,0,0},
        {5, "OPTIONS\0", NULL, 1, NULL,0,0},
        {6, "QUIT\0", menu_quit_click, 0, NULL,0,0}
};


MENU_CURSOR g_menu_cursor = {0};

void menu_update(MENU *menu_list, int opt);
void menu_draw(MENU *menu_list);

ENEMY* getFreeEnemy(void){
    for(int i = 0; i < ENEMY_ROW_Y; i++){
        for(int j = 0; j < ENEMY_ROW_X;j++){

            if(!enemies[i][j].alive){
                return &enemies[i][j];
            }

        }
    }
    return NULL;
}

void font_init(void){

    font_list[FONT_PIXEL_SMALL] = al_load_font("assets//fonts//o4b_03.ttf", 12, 0);
    font_list[FONT_PIXEL_BIG] = al_load_font("assets//fonts//o4b_03.ttf", 16, 0);
    font_list[FONT_PIXEL_MENU_BIG] = al_load_font("assets//fonts//o4b_03.ttf", 28, 0);
    font_list[FONT_PIXEL_MENU_SMALL] = al_load_font("assets//fonts//o4b_03.ttf", 20, 0);

}

void font_destroy(void){
    for(int i = 0; i < FONT_COUNT; i++){
        if(font_list[i]){
            al_destroy_font(font_list[i]);
            font_list[i] = NULL;
        }
    }
}


struct RENDER_ARGS {
         ALLEGRO_BITMAP *particles_buffer;
};


int  g_demo_start = FALSE;
void demo_update(void);
void demo_draw(struct RENDER_ARGS *args);

typedef struct HISCORE {
    char name[56];
    int score;
}HISCORE;

FILE *hiscore_fp =  NULL;
#define MAX_HISCORE 10
HISCORE hiscore[MAX_HISCORE] = {
        {"DANIEL_SAN\0",       99999},
        {"DISTRO\0",           99998},
        {"NICK\0",               997},
        {"ROGER\0",            99996},
        {"CODEMAN\0",          99995},
        {"JARVIS\0",           99993},
        {"SORA\0",             99991},
        {"DAGUY\0",            99990},
        {"KERMIT_THE_FROG\0",   5690},
        {"\0",         0},

};


void score_draw_text(void);
TEXT* score_free(TEXT score[10]);
void score_add(TEXT *score, const int num, const float x, const float y, ALLEGRO_COLOR color);
void score_update_text(void);

ALLEGRO_BITMAP *hiscore_bitmap = NULL;

void hiscore_init(void);
void hiscore_update(void);
void hiscore_draw(void);
int hiscore_load_file(char *filename, HISCORE *hsc);
int hiscore_save_file(const char *output_name);
void  hiscore_sort(HISCORE *hsc);
void hiscore_revert_array(HISCORE *hsc);

void hiscore_user_input(void);
void hiscore_user_input_update(void);

int hiscore_compress(FILE *f);
int hiscore_decompress(FILE *f);


void hiscore_create_default(ALLEGRO_FILE *f, HISCORE *hsc);
void hiscore_create_default_memory(void **dest);


MOUSECOORD g_mouse;


inline double angle_distance_rad(double x1, double y1, double x2, double y2){
    double dx,dy;

    dx = x2 - x1;
    dy = y2 - y1;

    return  atan2(dy,dx);
}

#define INVERT_ANGLE(angle) (angle *= -1)

int print_fade_text(ALLEGRO_FONT *fnt, float x, float y, ALLEGRO_COLOR c, float fade, const char *text, ...);


int  game_rand(int num)
{
    return num * ((1.0 * rand())/RAND_MAX) - 1;
}

int game_rand_range(int n, int m)
{
    return m + rand() / (RAND_MAX / (n-m + 1) + 1);
}

int rect_collision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
    if(x1 + w1 >= x2 &&
       x1 <= x2 + w2 &&
       (y1 + h1) >= y2 &&
       y1 <= y2 + h2){
        return TRUE;
    }

    return FALSE;
}










void allegro_get_desktop_size(int adapter, int *w, int *h){
    ALLEGRO_MONITOR_INFO info;
    al_get_monitor_info(adapter, &info);
    *w = info.x2 - info.x1;
    *h = info.y2 - info.y1;
}

int allegro_create_display_context(int width, int height, int fullscreen, int vsync, const char *title){
    int error = 0;
    int flags = 0;

    #define NOT_VALID_PTR(x) (x == NULL || !x ? 1 : 0)

    flags = ALLEGRO_OPENGL_3_0 | ALLEGRO_GENERATE_EXPOSE_EVENTS;

    al_set_new_display_option(ALLEGRO_VSYNC, vsync > 0 ? 2 : 1 , ALLEGRO_REQUIRE);



    if(fullscreen){
        flags |= ALLEGRO_FULLSCREEN_WINDOW;
    }else {
        flags |= ALLEGRO_WINDOWED;
    }

    allegro_get_desktop_size(0, &screen_width, &screen_height);
    al_set_new_display_flags(flags);

    if(strlen(title) > 0 || title != NULL){
        al_set_new_window_title(title);
    }

    int new_w, new_h;

    new_w = screen_width * 0.75;
    new_h = screen_height * 0.75;

    if(width > 0 || height > 0){

        new_w = width;
        new_h = height;
    }

    display = al_create_display(new_w, new_h);
    if(NOT_VALID_PTR(display)){
        error++;
        return error;
    }

    screen = al_get_backbuffer(display);

    queue = al_create_event_queue();
    if(NOT_VALID_PTR(queue)){
        error++;
        return error;
    }

    timer = al_create_timer(1.0/FPS);

    if(NOT_VALID_PTR(timer)){
        error++;
        return error;
    }


    debug_font = al_create_builtin_font();

    if(NOT_VALID_PTR(debug_font)){
        error++;
        return error;
    }



    buffer = al_create_bitmap(new_w, new_h);

    if(NOT_VALID_PTR(buffer)){
        error++;
        return error;
    }

    #undef NOT_VALID_PTR

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    //al_register_event_source(queue, al_get_timer_event_source(menu_timer));
    al_start_timer(timer);
    //al_start_timer(menu_timer);


    al_clear_to_color(al_map_rgb_f(0,0,0));
    al_flip_display();

    return error;
}

void allegro_destroy(void){

    if(voice) al_destroy_voice(voice);
    if(mixer) al_destroy_mixer(mixer);
    if(screen) screen = NULL;
    if(display) al_destroy_display(display);
    if(queue) al_destroy_event_queue(queue);
    if(timer) al_destroy_timer(timer);
    if(debug_font) al_destroy_font(debug_font);
    return;
}



void mode_blend(void){
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
}

void mode_solid(void){
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
}

void score_draw_text(void){

    for (int i= 0; i < 10; i++){
        if(score_list[i].ttl > 0){
            unsigned char a,r,g,b;
            al_unmap_rgba(score_list[i].color,&r,&g,&b,&a);
            int ttl = score_list[i].ttl;
            ALLEGRO_COLOR c = al_premul_rgba(r*ttl,g*ttl,b*ttl,a*ttl);
            al_draw_textf(font_list[FONT_PIXEL_BIG], c, score_list[i].x, score_list[i].y,0, "%s", score_list[i].text);
        }
    }
}

TEXT* score_free(TEXT score[10]){
    int c = 0;
    for(c = 0;c < 10; c++){
        if(score[c].ttl <= 0){
            break;
        }
    }
    return &score[c];
}

void score_add(TEXT *score, const int num, const float x, const float y, ALLEGRO_COLOR color){

    TEXT *s = score_free(score);

    s->x = x;
    s->y = y;
    s->ttl = 80;
    s->shadow = 0;
    s->color = color;
    if(num < 0){
         snprintf(s->text, 255, "%02d", num);
         return;
    }

    snprintf(s->text, 255, "+%02d", num);
    return;
}

void score_update_text(void){
    for (int i= 0; i < 10; i++){
        if(score_list[i].ttl > 0){
            score_list[i].y--;
            score_list[i].ttl--;
        }

    }
}


void new_game(int start){

    if(start){
        g_game_started = TRUE;
        enemies_reset(enemies);
        player_init(&player);

        if(enemy_wave == 1){
                enemy_wave_time = 200;
                enemy_wave_time_total = enemy_wave_time;
        }

        enemies_init(enemies);
        wave_reset();
    }

    player_init(&player);

    return;
}


void do_gameover(void){
     g_gamestate = GAMESTATE_TYPE_GAMEOVER;
     enemy_wave = 0 ;
     enemy_wave_time_total = 0;
     g_ship_active = FALSE;

    for(int y = 0; y < ENEMY_ROW_Y;y++){
        for(int x = 0; x < ENEMY_ROW_X;x++){
                if(enemies[y][x].alive){
                    enemies[y][x].alive = false;
                }

        }
    }
    g_gamestate = GAMESTATE_TYPE_USER_HISCORE;
    wave_reset();
}

void draw_life_bar(void){

    float total = fabs(player.life/100.0);
    float w = 300 * total;
    al_draw_filled_rectangle(0, 0, w, 15 , al_map_rgb_f(1,0,0));
    al_draw_multiline_textf(font_list[FONT_PIXEL_SMALL], al_map_rgb(255,255,255), w/2,0, 0,20,0,"%.2f",  fabs(player.life/100.0) * 100);
}

void draw_debug(void){
    al_draw_multiline_textf(debug_font, al_map_rgb_f(1.0,0,0), 0,0, 300, 20,0,
    "TOTAL ENEMIES: %d\n"
    "SHOT TIME: %d\n"
    "SHOOT : %s\n"
    "LINES: %d lines\n"
    "Enemy Shoot Time %d\n"
    "Player life: %.2f",
    enemies_count(enemies),
    player.shot_time,
    player.shoot ? "YES" : "No",
    line,
    enemy_shoot_time,
    player.life

   );
   return;
}




void update_spaceship(void){
    if(g_ship_active && g_spaceship_entity && g_spaceship_entity->alive){
        g_spaceship_entity->x -= 1.0;

        if(g_spaceship_entity->x < -32){
            g_spaceship_entity->alive = false;
            g_ship_active = FALSE;
        }
    }
}

void draw_spaceship(void){
    if(g_ship_active && g_spaceship_entity && g_spaceship_entity->alive){
            al_draw_bitmap(sprites[SPR_ENEMY02], g_spaceship_entity->x, g_spaceship_entity->y, 0);
    }
}





void init_intro(void){
   spr_intro[SPR_INTRO_CREATOR] = al_load_bitmap("assets//arch.jpg");
   if(!spr_intro[SPR_INTRO_CREATOR]){
       allegro_destroy();
       exit(1);
       return;
   }
}

void init_destroy(void){
    for(int i = 0; i < SPR_INTRO_TOTAL;i++){
        if(spr_intro[i]){
            al_destroy_bitmap(spr_intro[i]);
            spr_intro[i] = NULL;
        }
    }
}

void stars_update(void) {

    for(int layer = 1; layer < 3;layer++){
        for(int star = 0; star < MAX_STARS_PART;star++){
            STAR *s = &star_list[layer][star];
            if(s->pos.y < 0){
                s->pos.x = game_rand_range(0, al_get_display_width(display));
                s->pos.y = -10;
            }

            s->pos.y += 4.0;
        }
    }

}



void stars_draw(ALLEGRO_BITMAP *bmp){


    int format = al_get_bitmap_format(bmp);


    al_set_target_bitmap(bmp);
    al_lock_bitmap(bmp, format, ALLEGRO_LOCK_READWRITE);

    for(int stars = 0; stars < MAX_STARS_PART; stars++){
        STAR *s = &star_list[0][stars];
        al_put_pixel(s->pos.x, s->pos.y, al_map_rgba_f(1,1,1,0.5));
    }

    for(int stars = 0; stars < MAX_STARS_PART; stars++){
        STAR *s = &star_list[1][stars];
        al_put_pixel(s->pos.x, s->pos.y, al_map_rgba_f(1,0,1,0.1));
    }


    for(int stars = 0; stars < MAX_STARS_PART; stars++){
        STAR *s = &star_list[2][stars];
        al_put_pixel(s->pos.x, s->pos.y, al_map_rgba_f(1,0,0.5,0.5));
    }


    al_unlock_bitmap(bmp);
    al_set_target_backbuffer(display);
    al_draw_bitmap(bmp,0,0,0);


}


void stars_init(void){

    for(int layer = 0; layer < 3;layer++){
        for(int star = 0; star < MAX_STARS_PART;star++){
            STAR *s = &star_list[layer][star];


            s->pos.x = game_rand_range(1, al_get_display_width(display)-1);
            s->pos.y = game_rand_range(1, al_get_display_height(display)-1);
            s->ttl = game_rand_range(100,600);
            s->vel.x = 1.0;
            s->vel.y = 1.0;
        }
    }

    stars_bg = al_create_bitmap(al_get_display_width(display), al_get_display_height(display));

    if(stars_bg == NULL){
        return;
    }

}

void stars_destroy(void){


    if(stars_bg) {
        al_destroy_bitmap(stars_bg);
        stars_bg = NULL;
    }
}





void menu_update(MENU *menu_list, int opt){

    UNUSED(opt);
    int w,h,i;
    int height = 32;
    static int sound_played = 0;

    w =  al_get_display_width (display);
    h =  al_get_display_height(display);


    if(w >= 1360 && h >= 768){
        height = 64;
    }


    for(i = 0; i < MAX_MENU-1;i++){

        if(menu_list[i].menu_id <= 0)
            continue;


        if(g_game_started && menu_list[i].menu_id  ==  1 ) continue;
        if(!g_game_started && menu_list[i].menu_id ==  2 ) continue;

        int px             = al_get_display_width(display) / 2   - al_get_text_width(font_list[FONT_PIXEL_MENU_BIG], menu_list[i].opt_name);
        int py             = al_get_display_height(display) / 2  - al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]);
        int text_size      = al_get_text_width(font_list[FONT_PIXEL_MENU_BIG], menu_list[i].opt_name);
        int text_line_size = al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]);

        menu_list[i].x = px;
        menu_list[i].y = py + i * height;

        if(rect_collision(g_mouse.x, g_mouse.y,16,16, menu_list[i].x, menu_list[i].y, text_size, text_line_size)){
            if((g_mouse.buttons & 1)){
                g_menu_cursor.opt = menu_list[i].menu_id;
                continue;
            }

            if(!sound_played && menu_list[i].menu_id == opt){
                //play_sound(SFX_LASER, 1.0f,1.0f, 1.0f * 0.2 * opt , ALLEGRO_PLAYMODE_ONCE);
                sound_played = 1;
            }
        }

        if(menu_list[i].menu_id == opt){

            /* check if it has sub menu */
            if(menu_list[i].type != MENU_TYPE_SINGLE_BUTTON){
                for(int j = 0; j < MAX_MENU; j++){
                    if(menu_list[i].next_page && menu_list[i].next_page[j].menu_id == opt){
                        menu_list[i].next_page[j].menu_callback(&menu_list[i].next_page[j], NULL);
                        g_menu_cursor.opt = 0;
                        return;
                    }
                }
            }

            if(menu_list[i].menu_callback == NULL){
                fprintf(stderr, "MENU NULL\n");
                g_menu_cursor.opt = 0;
                continue;

            }else {
                menu_list[i].menu_callback(&menu_list[i], NULL);
                g_menu_cursor.opt = 0;
                continue;

            }
        }

    }

    sound_played = 0;
    return;

}

void demo_init(void){
    new_game(FALSE);
}

int64_t g_record_actual_frame = 0;

void demo_update(void){
        player_update_shot(&player);
        enemies_update_bullet(&player, enemies);

        stars_update();
        item_update(&player, item_list);

}

void demo_draw(struct RENDER_ARGS *args){

                al_set_target_bitmap(buffer);
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(stars_bg,0,0,0);
                al_draw_bitmap(args->particles_buffer,0,0,0);
                //draw_spaceship();
                //draw_enemies(enemies,0,0);

                //draw_player(player.x, player.y);
                //player_draw_shot();
                //pickup_draw();
                enemies_draw_bullets(enemies);
                //draw_life_bar();
                score_draw_text();
                //al_draw_textf(font_list[FONT_PIXEL_SMALL], al_map_rgb_f(1,1,1),10,20, 0, "SCORE: %08d", player.score);
                //al_draw_textf(font_list[FONT_PIXEL_SMALL], al_map_rgb_f(1,1,1),10,35, 0, "LIVES %02d", player.lives);
                particle_draw(NULL, particles,0,-8.0, al_map_rgb(255, 168, 0));
                al_set_target_backbuffer(display);
                al_draw_bitmap(buffer, 0,0,0);
}




void menu_draw(MENU *menu_list){

    ALLEGRO_COLOR blue = al_premul_rgba(52, 73, 94, 128);
    ALLEGRO_COLOR white = al_premul_rgba(52, 73, 94, 24);

    ALLEGRO_VERTEX menu_bg[3] = {
        {0,0,0,1,1, al_map_rgb(52, 73, 94)},
        {0, al_get_display_height(display),0,1,1, blue},
        {al_get_display_width(display), al_get_display_height(display),0,1,1, white}
    };

    al_draw_prim(menu_bg, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_FAN);

    int w = al_get_display_width(display);
    int h = al_get_display_height(display);
    ALLEGRO_FONT *fnt = NULL;
    int space_height = 32;

    if(w >= 1360 && h >= 768){
        fnt = font_list[FONT_PIXEL_MENU_BIG];
        space_height = 64;
    }else {
        fnt = font_list[FONT_PIXEL_MENU_SMALL];
        space_height = 32;
    }

    for(int i = 0; i < MAX_MENU; i++){

        if(menu_list[i].menu_id <= 0) continue;

        if(g_game_started && menu_list[i].menu_id  ==  1 ) continue;
        if(!g_game_started && menu_list[i].menu_id ==  2 ) continue;

        int px = al_get_display_width(display) / 2   - al_get_text_width(font_list[FONT_PIXEL_MENU_BIG], menu_list[i].opt_name);
        int py = al_get_display_height(display) / 2  - al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]);

        int text_size = al_get_text_width(font_list[FONT_PIXEL_MENU_BIG], menu_list[i].opt_name);
        int text_line_size = al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]);

        al_draw_textf(fnt, al_map_rgb_f(1,1,1),  px,  py + i * space_height, 0, "%s", menu_list[i].opt_name);
        al_draw_rectangle(menu_list[i].x,menu_list[i].y, menu_list[i].x + text_size, menu_list[i].y + text_line_size, al_map_rgb_f(1,0,0),1);
    }
}

void gameplay_states(void){

    switch(g_gamestate)
    {
        case GAMESTATE_TYPE_MENU:
            {
                al_show_mouse_cursor(display);
                if(g_game_started){
                    g_game_paused = TRUE;
                }
            }
            break;

        case GAMESTATE_TYPE_GAMEPLAY:
            {
                al_hide_mouse_cursor(display);
                if(g_game_started){
                    g_game_paused = FALSE;
                }
            }
            break;

    }

}

void gameplay_update(void){

        if(player_keys[ALLEGRO_KEY_ESCAPE]){
            g_gamestate = GAMESTATE_TYPE_MENU;
            return;
        }

#ifndef DEBUG
        if(player_keys[ALLEGRO_KEY_R]){
            if(gameover) gameover = 0;
            g_gamestate = GAMESTATE_TYPE_MENU;
            return;
        }
#endif // DEBUG


        if(g_game_paused == FALSE){

            player_update_shot(&player);
            enemies_update_bullet(&player, enemies);
            enemies_update(&player, item_list,  enemies);
            player_update(&player);
            score_update_text();
            stars_update();
            item_update(&player, item_list);
            update_spaceship();

            for(int i = 0; i  < MAX_PARTICLES-1; i++){
                  if(particles[i].ttl) particle_update(&particles[i]);
            }

            g_ship_counter = (al_get_timer_count(timer) / 60) % 12;

            if(g_ship_counter >= 11 && !g_ship_active){
                int ship_chance = game_rand(1000);

                if(ship_chance == 3){

                    g_ship_active = TRUE;

                    if(!g_spaceship_entity){
                        // i need to get out of this condition but keep the loop active not resetting for 1 frame
                        // just skip everything and continue
                        g_ship_counter = 0;
                        g_ship_active = FALSE;
                        g_spaceship_entity->alive = FALSE;
                        return;
                    }

                    play_sound(SFX_SHIP, 1.0, 0.0, 1.0,  ALLEGRO_PLAYMODE_LOOP);
                    g_spaceship_entity->x = al_get_display_width(display) + 32;
                    g_spaceship_entity->y = game_rand_range(65,100);
                    g_spaceship_entity->alive = TRUE;


                }

                g_ship_counter = 0;
            }


            if(enemy_wave >= 15 && game_difficulty <= DIFF_HARD){
                game_difficulty = DIFF_HARD;
            }

            if(enemy_wave >= 30 && game_difficulty <= DIFF_HARD){
                game_difficulty = DIFF_HARD;
            }

            if(enemy_wave >= 50  && game_difficulty <= DIFF_HARD){
                game_difficulty = DIFF_NIGHTMARE;
            }



            if(enemies_count(enemies) <= 0){
                enemy_wave++;
                al_stop_timer(timer);
                al_start_timer(timer);
                enemy_wave_time = 200;
                enemy_wave_time_total = enemy_wave_time;
            }

        }
}

void gameplay_draw(struct RENDER_ARGS *args){

                mode_blend();
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(stars_bg,0,0,0);
                al_draw_bitmap(args->particles_buffer,0,0,0);
                draw_spaceship();
                draw_enemies(enemies,0,0);

                player_draw_shot(&player);
                item_draw(item_list);

                enemies_draw_bullets(enemies);
                draw_life_bar();
                score_draw_text();
                al_draw_textf(font_list[FONT_PIXEL_SMALL], al_map_rgb_f(1,1,1),10,20, 0, "SCORE: %08d", player.score);
                al_draw_textf(font_list[FONT_PIXEL_SMALL], al_map_rgb_f(1,1,1),10,35, 0, "LIVES %02d", player.lives);
                al_draw_textf(font_list[FONT_PIXEL_SMALL], al_map_rgb_f(1,1,1),10,55, 0, "AMMO %02d", player.ammo <= 0 ? 0 : player.ammo);

                particle_draw(NULL, particles,0,-8.0, al_map_rgb(255, 168, 0));
                player_draw(player.x, player.y);

                if(enemy_wave_time > 0){

                    float wave_x = 0.0;
                    float wave_y = 0.0;
                    float alpha =  (float)enemy_wave_time_total / enemy_wave_time;

                    ALLEGRO_COLOR trans_color = al_premul_rgba_f(1,0,0, alpha);
                    char buf[25];
                    sprintf(buf, "WAVE: %d", enemy_wave);

                    al_draw_text(font_list[FONT_PIXEL_MENU_BIG],
                    trans_color,
                    wave_x+((float)al_get_display_width(display) / 2) - al_get_text_width(font_list[FONT_PIXEL_MENU_BIG], buf),
                    -wave_y+((float)al_get_display_height(display) / 2),
                    0,
                    buf
                    );

                    wave_y += 0.9;
                    enemy_wave_time--;

                }

                print_fade_text(font_list[FONT_PIXEL_MENU_BIG],10,95,al_map_rgb_f(1,1,1), 0.4, "TESTE 123");

}

int main(int argc, char **argv)
{
    UNUSED(argc); UNUSED(argv);

    char error[1024] = {0};

    if(allegro_init(error) > 0 ){
        fprintf(stderr, "Error: %s", error);
        exit(1);
    }

    al_change_directory(".");

#ifdef PHYSFS_DATAFILES
        if(!PHYSFS_init(argv[0])){
            al_show_native_message_box(display, "Error!", "PHYSFS Error:", "Error Trying to init pshyfs", NULL, 0);
            exit(-1);
            return 0;
        }

        if(!PHYSFS_mount(GAME_DATAFILES,NULL, 1)){
            al_show_native_message_box(display, "Error!", "PHYSFS Error:", "Error to find gamedata assets", NULL, 0);
            exit(-1);
            return 0;
        }

        al_set_physfs_file_interface();


#endif

    char title[200];

    snprintf(title, 200, "Invaders From Space (better) %d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);

    if(allegro_create_display_context(0,0,0,2, title) > 0){
        fprintf(stderr, "Error: failed to load display context");
        exit(1);
    }

    int redraw = 0;

    key_mutex = al_create_mutex();

    if(!key_mutex){
            al_show_native_message_box(display, "Error", "Mutex Error", "Keyboard Mutex cannot be created!", NULL, 0);
            free(g_spaceship_entity);
            g_spaceship_entity = NULL;
            font_destroy();
            stars_destroy();
            destroy_audio();
            allegro_destroy();
            return 1;

    }

    if(init_audio() > 0){
        exit(1);
    }

    init_sprites();
    font_init();

    hiscore_init();
    stars_init();
    item_init(item_list);


    g_spaceship_entity = al_malloc(sizeof(ENEMY));
    memset(g_spaceship_entity, 0, sizeof(*g_spaceship_entity));

    //int flags = al_get_new_bitmap_flags();
    //al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    ALLEGRO_BITMAP *particles_buffer = al_create_bitmap(al_get_display_width(display), al_get_display_height(display));
    //al_set_new_bitmap_flags(flags);

    particle_reset(particles, DEFAULT_PARTICLES);
    stars_draw(stars_bg);
    g_menu_cursor.menu_count = sizeof(game_menu) / sizeof(game_menu[0]);
    g_menu_cursor.cursor = sprites[SPR_ENEMY02];


    struct RENDER_ARGS render_args;
    render_args.particles_buffer = particles_buffer;


    while(!g_close_game){

        ALLEGRO_EVENT e;

       if(redraw && al_event_queue_is_empty(queue)){

            al_set_clipping_rectangle(0, 0, screen_width, screen_height);
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if(g_gamestate == GAMESTATE_TYPE_MENU){

                    if(g_demo_start) demo_draw(&render_args);
                    menu_draw(game_menu);
            }

            if(g_gamestate == GAMESTATE_TYPE_GAMEPLAY){
                gameplay_draw(&render_args);

            }

            if(g_gamestate == GAMESTATE_TYPE_HISCORE){
                hiscore_draw();
            }

            if(g_gamestate == GAMESTATE_TYPE_GAMEOVER){
                gameover_draw();
            }

            if(g_gamestate == GAMESTATE_TYPE_USER_HISCORE){
                hiscore_user_input();
            }

            al_flip_display();
            redraw = 0;
        }


        do {

            al_wait_for_event(queue, &e);

            switch(e.type){
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    g_close_game =  TRUE;
                    break;

                case ALLEGRO_EVENT_KEY_DOWN:
                    player_keys[e.keyboard.keycode] = TRUE;
                    player_released_keys[e.keyboard.keycode] = FALSE;
                    break;

                case ALLEGRO_EVENT_KEY_UP:
                    player_keys[e.keyboard.keycode] = FALSE;
                    player_released_keys[e.keyboard.keycode] = TRUE;
                    break;

                case ALLEGRO_EVENT_KEY_CHAR:
                    {

                        switch(g_gamestate){
                            case  GAMESTATE_TYPE_USER_HISCORE:
                            {
                                if(keybuffer_counter < 20){
                                    al_lock_mutex(key_mutex);
                                    keybuffer[keybuffer_counter%255] = (char)e.keyboard.unichar;
                                    al_unlock_mutex(key_mutex);
                                    keybuffer_counter++;
                                }
                            }
                                break;
                        }

                    }
                    break;

                case ALLEGRO_EVENT_MOUSE_AXES:
                    {
                        g_mouse.x = e.mouse.x;
                        g_mouse.y = e.mouse.y;
                    }
                    break;

                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    {
                        g_mouse.buttons |= e.mouse.button;
                    }
                    break;

                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    {
                        g_mouse.buttons &= ~e.mouse.button;
                    }
                    break;
                case ALLEGRO_EVENT_TIMER:
                {
                    gameplay_states();

                    if(g_gamestate == GAMESTATE_TYPE_HISCORE){
                        hiscore_update();
                    }

                    if(g_gamestate == GAMESTATE_TYPE_MENU){


                        if(!g_demo_start){
                            demo_init();
                            g_demo_start = TRUE;
                        }

                        demo_update();

                        menu_update(game_menu, g_menu_cursor.opt);
                    }

                    if(g_gamestate == GAMESTATE_TYPE_GAMEPLAY){

                            if(g_demo_start){
                                g_demo_start = FALSE;
                            }

                            gameplay_update();

                    }

                    if(g_gamestate == GAMESTATE_TYPE_GAMEOVER){
                        do_gameover();
                        gameover_update();
                    }

                    if(g_gamestate == GAMESTATE_TYPE_USER_HISCORE){
                        hiscore_user_input_update();
                    }
                    redraw = 1;
                }
                    break;
            }


        }while(!al_event_queue_is_empty(queue));
    }

    if(particles_buffer){
        al_destroy_bitmap(particles_buffer);
    }

    free(g_spaceship_entity);
    g_spaceship_entity = NULL;
    font_destroy();
    stars_destroy();
    destroy_audio();
    allegro_destroy();

	return 0;
}

static void menu_start_game_click(struct MENU *menu, void *args){
    UNUSED(menu); UNUSED(args);

        if(!g_game_started){
            new_game(TRUE);

        }
    g_gamestate = GAMESTATE_TYPE_GAMEPLAY;
    return;
}


static void menu_hiscore_click(struct MENU *menu, void *args){
    UNUSED(menu); UNUSED(args);
    g_gamestate = GAMESTATE_TYPE_HISCORE;
}

static void menu_quit_click(struct MENU *menu, void *args){
    UNUSED(menu); UNUSED(args);
    g_close_game = TRUE;

}

typedef struct HISCORE_HDR {
        char magic[5];
        mz_ulong uncompressed_size;
        mz_ulong compressed_size;
        int type_compress;
        int strategy;
}HISCORE_HDR;


void hiscore_init(void){

    int lines = 0;
    int height = 0;

    for(int i = 0; i < MAX_HISCORE-1;++i){
        if(hiscore[i].score != 0) lines++;
    }

    height = lines * al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]) + 128;
    hiscore_bitmap = al_create_bitmap(al_get_display_width(display), height);

    if(hiscore_bitmap == NULL){
        al_show_native_message_box(display, "Error","Hiscore Failed", "Failed to create hiscore bitmap!", NULL,0);
        return;
    }

    ALLEGRO_FILE *fp = al_fopen(HISCORE_DATA,"rb+");

    if(fp == NULL) {
        al_fclose(fp);

        fp = al_fopen(HISCORE_DATA,"wb+");
        hiscore_create_default(fp, hiscore);
        hiscore_sort(hiscore);
        hiscore_revert_array(hiscore);
        hiscore_save_file(HISCORE_DATA);
        al_fclose(fp);
        return;

    }

    al_fclose(fp);

    hiscore_load_file(HISCORE_DATA, hiscore);
    hiscore_save_file(HISCORE_DATA);
    hiscore_sort(hiscore);
    hiscore_revert_array(hiscore);


}

long hiscore_size(void){
    struct stat st;

    if(stat("hiscore.dat",&st) < 0){
        return -1;
    }
    return st.st_size;

}


int hiscore_compress(FILE *f){

     FILE *out = NULL;
     unsigned char *unc_data = NULL;
     unsigned char *dest_data = NULL;
     mz_ulong unc_len = 0;

    if(ftell(f) != 0){
        fseek(f,0, SEEK_SET);
    }

    // skip header
    HISCORE_HDR header;
    fread(&header, sizeof(HISCORE_HDR),1,f);

    if(strncmp(header.magic, "HSC1\0",5) != 0){
        return -1;
    }


    unc_data = malloc(header.uncompressed_size);
    dest_data =  malloc(header.compressed_size);
    unc_len = header.uncompressed_size;

    fread(unc_data, sizeof(HISCORE) * MAX_HISCORE,1,f);

    int status = compress(dest_data, &header.compressed_size, unc_data, unc_len);


    out = fopen("hiscore.cdata", "wb+");
    fwrite(dest_data,header.compressed_size,1,out);
    fclose(out);


    if(status != Z_OK){
        free(unc_data);
        return Z_ERRNO;
    }


    free(unc_data);
    return Z_OK;


}

void hiscore_create_default(ALLEGRO_FILE *f, HISCORE *hsc){
    if(f){
        HISCORE_HDR header;
        snprintf(header.magic,5,"%s","HSC1\0");
        header.uncompressed_size = hiscore_size();
        header.compressed_size = 0;
        header.type_compress = Z_BEST_COMPRESSION;
        header.strategy = Z_DEFAULT_STRATEGY;

        al_fwrite(f,&header, sizeof(HISCORE_HDR));

        for(int i = 0; i < MAX_HISCORE;i++){
            al_fwrite(f, hsc[i].name, 20);
            al_fwrite(f, &hsc[i].score, sizeof(int));
        }


        al_fwrite(f, "\\ENDHSC\0",9);

    }

}

void hiscore_create_default_memory(void **dest){
    if(! (*dest)){
        *dest = malloc(sizeof(HISCORE) * MAX_HISCORE);
        memset(*dest, 0x0, sizeof(HISCORE) * MAX_HISCORE);
    }

    for(int i = 0; i < MAX_HISCORE;i++){
        memcpy(*dest,&hiscore[i], sizeof(HISCORE));
    }

}


int hiscore_load_file(char *filename, HISCORE *hsc){

    ALLEGRO_FILE *fp = NULL;
    char buf[255];

    snprintf(buf,255,"%s", filename);

    if((fp = al_fopen(buf,"rb+")) == NULL){
        return -1;
    }

    HISCORE_HDR header;

    al_fread(fp, &header, sizeof(HISCORE_HDR));

    if(header.uncompressed_size != al_fsize(fp)){
        return -2;
    }

    al_fread(fp, hsc, sizeof(HISCORE) * MAX_HISCORE);

    al_fclose(fp);
    return 0;
}
int hiscore_save_file(const char *output_name){

    ALLEGRO_FILE *fp = NULL;
    struct stat st;
    char buf[255];

    sprintf(buf, "%s", output_name);

    if( (fp = al_fopen(buf, "wb+")) == NULL){
        return -1;
    }

    HISCORE_HDR hdr;

    if(stat(buf, &st) < 0){
        return -1;
    }

    sprintf(hdr.magic, "HSC");
    hdr.compressed_size = st.st_size;
    hdr.uncompressed_size = 0;
    hdr.strategy = 1;
    hdr.type_compress = 1;

    al_fwrite(fp, &hdr, sizeof(HISCORE_HDR));
    al_fwrite(fp, &hiscore, sizeof(HISCORE) * MAX_HISCORE);

    al_fclose(fp);
    return 0;


}





void hiscore_update(void){


     if(player_keys[ALLEGRO_KEY_ENTER]){
        g_gamestate = GAMESTATE_TYPE_MENU;
        return;
     }

     if(keybuffer[keybuffer_counter] != '\0' || g_mouse.buttons & 1 || g_mouse.buttons & 2 || keybuffer[ALLEGRO_KEY_ENTER]){
        memset(keybuffer,0,255);
        g_gamestate = GAMESTATE_TYPE_MENU;
        return;
     }
}

void hiscore_draw(void){

    al_draw_bitmap(stars_bg,0,0,0);

    for(int i = 0; i < MAX_HISCORE; i++){

        int h = al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]);

        if(hiscore[i].score > 0){
            al_draw_textf(font_list[FONT_PIXEL_BIG], al_map_rgb_f(1,1,1),
            10,
            200 + h*i,
            0,
            "%s", hiscore[i].name
            );


            al_draw_textf(font_list[FONT_PIXEL_MENU_BIG], al_map_rgb_f(1,1,1),
            (float)al_get_bitmap_width(hiscore_bitmap)/2,
            200 + h*i,
            0,
            "%05d", hiscore[i].score
            );

        }
    }

    return;
}


void hiscore_user_input(void){

    int w;

    w =  al_get_display_width(display) - (al_get_display_width(display) / 2);

    al_draw_bitmap(stars_bg,0,0,0);
    al_draw_text(font_list[FONT_PIXEL_MENU_BIG], al_map_rgb_f(1,0,1), w-100, 100, 0,"CONGRATULATIONS YOU ACHIEVED A HISCORE!!!");
    al_draw_textf(font_list[FONT_PIXEL_MENU_BIG], al_map_rgb_f(1,1,1), w, al_get_display_height(display) - 250, 0,"TYPE YOUR NAME : %s", keybuffer);


}

void hiscore_user_input_update(void){

    if(player_keys[ALLEGRO_KEY_BACKSPACE]){

        if(keybuffer_counter > 0){

            al_lock_mutex(key_mutex);
            keybuffer[keybuffer_counter] = '\0';
            al_unlock_mutex(key_mutex);
            keybuffer_counter--;
        }else {
            keybuffer[0] = '\0';
        }

    }

    if(player_keys[ALLEGRO_KEY_ENTER]){
        do_gameover();
        g_gamestate = GAMESTATE_TYPE_HISCORE;

    }

}

void hiscore_revert_array(HISCORE *hsc){

   HISCORE *ptr = &hsc[MAX_HISCORE-1];
   HISCORE h[MAX_HISCORE];

   for(int i = 0; i < MAX_HISCORE-1; i++){
        h[i] = *(ptr--);
   }

   memcpy(hsc, &h, sizeof(HISCORE) * MAX_HISCORE);
}

void hiscore_sort(HISCORE *hsc){


    int i,j;

    for(i = 0; i < MAX_HISCORE-1; ++i){
        for(j = i+1; j < MAX_HISCORE-1;++j){
            if(hsc[i].score > hsc[j].score){
                HISCORE tmp = hsc[i];
                hsc[i] = hsc[j];
                hsc[j] = tmp;

            }
        }
    }

    hiscore_revert_array(hsc);
    return;
}

void gameover_init(void){

}
void gameover_update(void){
     if(keybuffer[keybuffer_counter] != '\0' || g_mouse.buttons & 1 || g_mouse.buttons & 2){
         memset(keybuffer, 0, 255);
         g_gamestate = GAMESTATE_TYPE_HISCORE;
     }

    return;
}
void gameover_draw(void){

    char text[255];

    snprintf(text,255,"GAME OVER");

    int size = al_get_text_width(font_list[FONT_PIXEL_MENU_BIG], text);
    al_draw_text(font_list[FONT_PIXEL_MENU_BIG], al_map_rgb_f(1,0,0), al_get_display_width(display) / 2 - size,0,0, "GAME OVER!");

}

void wave_reset(void){
    enemy_wave = 1;
    enemy_wave_time = 0;
    enemy_wave_time_total = 0;
}











void menu_continue_game_click(struct MENU *menu, void *args){
    UNUSED(menu); UNUSED(args);
    if(g_gamestate != GAMESTATE_TYPE_GAMEPLAY){
        g_gamestate = GAMESTATE_TYPE_GAMEPLAY;
        return;
    }

    return;

}

int print_fade_text(ALLEGRO_FONT *fnt, float x, float y, ALLEGRO_COLOR color, float fade, const char *text, ...){
    char buf[255];
    va_list lst;
    float c = 1+(y-fade) / 360 / 2.0;
    float col = c > 1 ? 1 : c < 0 ? 0 : 0;
    ALLEGRO_COLOR tmp_col = al_map_rgba_f(color.r *col, color.g * col, color.b * col, color.a * col);

    va_start(lst,text);
    vsnprintf(buf,255,text, lst);
    va_end(lst);

    al_draw_textf(fnt, tmp_col, x,y, 0, "%s", buf);
    return (int)y + al_get_font_line_height(fnt);

}
