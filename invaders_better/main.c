#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include <sys/types.h>
#include <sys/stat.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>



#define TRUE  (1ul)
#define FALSE (0l)
#define TILE 32

#define UNUSED(x) (void)x
#define ENEMY_ROW_X 10
#define ENEMY_ROW_Y 5

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *buffer  = NULL;
ALLEGRO_FONT *debug_font = NULL;


ALLEGRO_MIXER *mixer = NULL;
ALLEGRO_VOICE *voice = NULL;


ALLEGRO_MUTEX *key_mutex = NULL;
char keybuffer[255] = {0};
int keybuffer_counter = 0;

#define PI  ALLEGRO_PI
#define PI2 (ALLEGRO_PI*2)

#define RAD2DEG 57.29578049
#define DEG2RAD 0.017453292

#define FPS 60.0

static int screen_width  = 0;
static int screen_height = 0;

#define MAX_BULLETS 20


#define DEFAULT_PARTICLES 2000
#define MAX_PARTICLES     10000


typedef struct PARTICLE {
    int x,y;
    double vx,vy;
    int ttl;
    int color;
}PARTICLE;

typedef struct PARTICLE_VERTEX {
    int u,v;
    float x,y;
    int ttl;

}PARTICLE_VERTEX;



PARTICLE particles[DEFAULT_PARTICLES] = {0};



void particle_create(PARTICLE *p, int x, int y, double vx,  double vy, int color, int life);
void particle_update(PARTICLE *p);
void particle_reset(PARTICLE *p, int max);
PARTICLE *particle_free(PARTICLE *p, int max);
void particle_explosion(PARTICLE *plist, int x, int y, int spread, int particle_count, int life);
void particle_clear_buffer(PARTICLE *plist, ALLEGRO_BITMAP *bmp, ALLEGRO_COLOR color);



enum SPRITE_INTRO {
    SPR_INTRO_CREATOR,
    SPR_INTRO_ROYALTIES,
    SPR_INTRO_TOTAL
};

ALLEGRO_BITMAP *spr_intro[10];

typedef struct BULLET {
    float x,y;
    float vx,vy;
    int ttl;
    int alive;
    float angle;
    ALLEGRO_COLOR color;
} BULLET;


enum  ENEMY_TYPE {
    ENEMY_TYPE_BLUE,
    ENEMY_TYPE_GREEN,
    ENEMY_TYPE_SHIP,
    
    ENEMY_TYPE_COUNT
};

typedef struct ENEMY {
    float x,y;
    int type;
    int life;
    int alive;
    BULLET bullets[MAX_BULLETS];
    int shoot_time;

} ENEMY;

typedef struct PLAYER {
    float x,y;
    float vx,vy;
    float life;
    int lives;
    int score;
    int alive;
    int direction;
    BULLET bullets[MAX_BULLETS];
    int shot_time;
    int shoot;
    int keypressed[4];
} PLAYER;




enum EFFECT_TYPE {
    EFFECT_TYPE_FADE_IN,
    EFFECT_TYPE_FADE_OUT
};

typedef struct EFFECT {
    int type;
    int time;
    void (*effect_update_callback)(struct EFFECT *effect);
    void (*effect_draw_callback)(struct EFFECT *effect);
}EFFECT;


typedef struct  PICKUP {
    int type;
    float x,y;
    float vx,vy;
    int alive;
}PICKUP;

#define MAX_PICKUP 12

enum {
    PICKUP_DOUBLE_SHOOT,
    PICKUP_SHIELD,
    PICKUP_TOTAL
};


static PLAYER player;
static ENEMY enemies[ENEMY_ROW_Y][ENEMY_ROW_X];
static PICKUP pickup[MAX_PICKUP];



#define SPRITE_MAX 12


typedef struct TEXT {
    float x, y;
    char text[255];
    int ttl;
    int shadow;
}TEXT;


static TEXT score_list[10] = {0};


#define MAX_SFX 20

ALLEGRO_SAMPLE *sfx_list[MAX_SFX] = {0};



static int gameover = 0;
static int player_keys[ALLEGRO_KEY_MAX] = {0};
static int player_released_keys[ALLEGRO_KEY_MAX]  = {0};
static int enemy_direction = 1; //1 or -1
static int line = 0;
static int  enemy_shoot_time = 70;


enum SPRITE_ID {
    SPR_BG01,
    SPR_CANNON,
    SPR_ENEMY01,
    SPR_ENEMY02,
    SPR_ENEMYSHOT,
    SPR_PLAYERSHOT,
    SPR_TITLE,
    SPR_DSHOT
    
};



enum GAME_DIFFICULTY {
    DIFF_NOVICE,
    DIFF_EASY,
    DIFF_NORMAL,
    DIFF_HARD,
    DIFF_NIGHTMARE,
    
    DIFF_COUNT
};



int game_difficulty = DIFF_NOVICE;


#define WALK_TIME_DELAY_PHASE1 1000
#define WALK_TIME_DELAY_PHASE2 900
#define WALK_TIME_DELAY_PHASE3 600
#define WALK_TIME_DELAY_PHASE4 500
#define WALK_TIME_DELAY_PHASE5 300

static int walk_time = 1000;


const char sprite_assets_path[SPRITE_MAX][255] = {
    {"assets//bg01.bmp"},
    {"assets//cannon.bmp"},
    {"assets//enemy01.bmp"},
    {"assets//enemy02.bmp"},
    {"assets//eshot.bmp"},
    {"assets//pshot.bmp"},
    {"assets//title.png"}, 
    {"assets//dshot.png"}
};

ALLEGRO_BITMAP *sprites[SPRITE_MAX] = {0};



typedef struct DIFFICULTY_PARAMS {
    int type;
    float speed_multiplier;
    int enemy_shot_time;
    float shot_speed;

}DIFFICULTY_PARAMS;

const DIFFICULTY_PARAMS difficulty[DIFF_COUNT] = {
    {
        DIFF_NOVICE,
        2.2,
        100,
        2.0
    },
    
    {
        DIFF_EASY,
        3.2,
        100,
        2.0
    },
    
    {
        DIFF_NORMAL,
        4.2,
        80,
        3.0f
    },
    
    {
        DIFF_HARD,
        6.5,
        80,
        3.0f
    },
    
    {
        DIFF_NIGHTMARE,
        8.0,
        60,
        4.00
    }
    
};

typedef struct STAR {
        float x,y;
        float vx,vy;
        int ttl;
}STAR;


ALLEGRO_BITMAP *stars_bg = NULL;

#define MAX_STARS 2000
STAR stars[MAX_STARS] = {0};

enum FONT_ID {

    FONT_PIXEL_SMALL,
    FONT_PIXEL_BIG,
    FONT_PIXEL_MENU_BIG,

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
        GAMESTATE_TYPE_OPTIONS
};

static int g_gamestate = GAMESTATE_TYPE_MENU;
void new_game(void);

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

MENU game_menu[MAX_MENU] = {
        {1, "START GAME\0", menu_start_game_click, 0, NULL,0,0},
        {2, "HI-SCORE\0", menu_hiscore_click, 0, NULL,0,0},
        {3, "OPTIONS\0", NULL, 1, NULL,0,0},
        {4, "QUIT\0", NULL, 0, NULL,0,0}
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
    char name[20];
    int score;
}HISCORE;

FILE *hiscore_fp =  NULL;
#define MAX_HISCORE 10
HISCORE hiscore[MAX_HISCORE] = {
        {"CIROBAIABA\0",   99999},
        {"ZE DA MANGA\0",  99998},
        {"NICK\0",         99997},
        {"ROGER DEIOAH\0", 99996},
        {"MR CODEMAN\0",   99994},
        {"JRCL123\0",      99993},
        {"TANDE\0",        99992},
};


ALLEGRO_BITMAP *hiscore_bitmap = NULL;

void hiscore_init(void);
void hiscore_update(void);
void hiscore_draw(void);

int allegro_init(char *error_message) {
    
    int error = 0;
    
    if(!al_init()){
        error++;
        sprintf(error_message, "Allegro Cant Initilialized\n");
    }
    
    if(!al_install_keyboard()){
        error++;
        sprintf(error_message, "Keyboard not Installed\n");
    }
    
    if(!al_install_mouse()){
        error++;
        sprintf(error_message, "Mouse not Installed\n");
    }
    
    
    if(!al_install_audio()){
        error++;
        sprintf(error_message, "Audio Error!\n");
    }
    
    if(!(al_init_font_addon() && al_init_ttf_addon())){
                error++;
        sprintf(error_message, "TTF Error!\n");
    }
    
    if(!al_init_acodec_addon()){
        error++;
        sprintf(error_message, "Audio Codecs Errors!\n");
    }
    
    al_reserve_samples(16);
    
    if(!al_init_native_dialog_addon()){
        error++;
        sprintf(error_message, "Native Dialog Errors!\n");
    }
    
    if(!al_init_primitives_addon()){
        error++;
        sprintf(error_message, "Primitives Errors!\n");
    }
    
    if(!al_init_image_addon()){
        error++;
        sprintf(error_message, "Image Not Loaded!\n");
    }
    
    
   voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
   
   if(voice == NULL) {
      error++;
      sprintf(error_message, "Failed created voice\n");
   }

   mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
   if(mixer == NULL) {
      error++;
      sprintf(error_message, "Failed created mixer\n");
   }
   
   
   if(!al_attach_mixer_to_voice(mixer, voice)){
      error++;
      sprintf(error_message, "Failed to attach mixer to voice\n");
   }

   if(!al_set_default_mixer(mixer)) {
      error++;
      sprintf(error_message, "Failed set default mixer\n");
   }

   if(!al_reserve_samples(16)) {
      error++;
      sprintf(error_message, "Failedto reserve samples\n");
   }
    
    return error;
}


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





int getCountBulletsAlive(BULLET *bullets){
    
    int c = 0;
    while(++c < MAX_BULLETS && bullets[c].alive);
    return c;
    
    
}

BULLET* getFreeBullet(BULLET bullets[MAX_BULLETS]){
   int c = 0;
   
   for(c = 0; c < MAX_BULLETS; c++){
    if(!bullets[c].alive){
            break;
        }
    }
    
   return &bullets[c];    
}


enum {
        SFX_EXPLOSION1,
        SFX_EXPLOSION2,
        SFX_EXPLOSION3,
        SFX_LASER,
        SFX_WALK,
        SFX_HIT,
        SFX_POWERUP,
        SFX_SHIP,
        
        SFX_TOTAL
};

int init_audio(void){
    int error = 0;
    #define SND_PTR_FAILED(x) (!x||x == NULL)
    sfx_list[SFX_EXPLOSION1] = al_load_sample("assets//sfx//explosion01.wav");
    sfx_list[SFX_EXPLOSION2] = al_load_sample("assets//sfx//explosion02.wav");
    sfx_list[SFX_EXPLOSION3] = al_load_sample("assets//sfx//explosion03.wav");
    sfx_list[SFX_LASER] = al_load_sample("assets//sfx//laserShoot.wav");
    sfx_list[SFX_WALK] = al_load_sample("assets//sfx//walk.wav");
    sfx_list[SFX_HIT] = al_load_sample("assets//sfx//hit.wav");
    sfx_list[SFX_POWERUP] = al_load_sample("assets//sfx//powerUp.wav");
    sfx_list[SFX_SHIP] =  al_load_sample("assets//sfx//ship.wav");
    
    if(SND_PTR_FAILED(sfx_list[0])){
        error++;
    }
    
    if(SND_PTR_FAILED(sfx_list[1])){
        error++;
    }
    
    if(SND_PTR_FAILED(sfx_list[2])){
        error++;
    }
    
    #undef SND_PTR_FAILED
    
    return error;
}




void play_sound(int id, float volume, float pan, float speed, ALLEGRO_PLAYMODE mode){
    if(sfx_list[id]){
        al_play_sample(sfx_list[id], volume, pan, speed, mode, NULL);
        return;
    }
    
    fprintf(stderr,"sound invalid");
    return;
}

ALLEGRO_SAMPLE_ID play(int id){
    ALLEGRO_SAMPLE_ID sample_id;
    if(sfx_list[id]){
        al_play_sample(sfx_list[id], 1.0,0.0,1.0, ALLEGRO_PLAYMODE_ONCE, &sample_id);
        return sample_id;
    }
    
    fprintf(stderr,"sound invalid");
    return sample_id;
}

void destroy_audio(void){
    
    for(int i = 0; i < MAX_SFX;i++){
        if(sfx_list[i]){
            al_destroy_sample(sfx_list[i]);
            sfx_list[i] = NULL;
        }
    }
}


PICKUP *getFreePickup(void){
    
    uint32_t c;
    
    for(c = 0; c < MAX_PICKUP ;c++){
        if(!pickup[c].alive) break;
    }
    return &pickup[c];
    
}


void pickup_add(ENEMY enemies[ENEMY_ROW_Y][ENEMY_ROW_X], int index_x, int index_y, int id){
    
    PICKUP *p = getFreePickup();
    float dx = enemies[index_y][index_x].x - player.x;
    float dy = enemies[index_y][index_x].y - player.y;
    
    double angle = atan2(dy,dx);
    
    p->x =  enemies[index_y][index_x].x + cos(angle * RAD2DEG);
    p->y =  enemies[index_y][index_x].y + sin(angle * RAD2DEG);
    p->type = id;
    p->alive = TRUE;
    p->vx = 1.0;
    p->vy = 1.0;
    
}


void pickup_update(void){
    for(int i = 0; i < PICKUP_TOTAL; i++){
        if(!pickup[i].alive) continue;
        
        //float dx = pickup[i].x - player.x;
        //float dy = pickup[i].y -  al_get_display_height(display) - 300;
        //double a = atan2(dy,dx);
        
        
        if(pickup[i].x > al_get_display_width(display)){
            pickup[i].alive = FALSE;
        }
        
        if(pickup[i].x < 0){
            pickup[i].alive = FALSE;
        }
        
        if(pickup[i].y > al_get_display_height(display)){
            pickup[i].alive = FALSE;
        }
        
        if(pickup[i].y < 0){
            pickup[i].alive = FALSE;
        }
        
        
        
        pickup[i].x += pickup[i].vx;
        pickup[i].y += pickup[i].vy;
    }
}


void pickup_draw(void){
    for(int i = 0; i < MAX_PICKUP; i++){
        if(!pickup[i].alive) continue;
        
        switch(pickup[i].type){
            case PICKUP_DOUBLE_SHOOT:
                al_draw_bitmap(sprites[SPR_DSHOT], pickup[i].x, pickup[i].y,0);
                break;
            
            case PICKUP_SHIELD:
                al_draw_circle(pickup[i].x, pickup[i].y, 30, al_map_rgb_f(1,1,1),1.0);
                break;
        }
        
    }
}


BULLET *create_shot(BULLET *bullets, float x, float y, float vx, float vy){
    
    BULLET *bullet = getFreeBullet(bullets);
    
    bullet->x = x;
    bullet->y = y;
    bullet->alive = TRUE;
    bullet->ttl = game_rand_range(80,255);
    bullet->vx = vx;
    bullet->vy = vy;
    
    
    return bullet;
    
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
    
    flags = ALLEGRO_OPENGL_3_0 | ALLEGRO_EVENT_DISPLAY_EXPOSE;
    
    al_set_new_display_option(ALLEGRO_VSYNC, vsync, ALLEGRO_REQUIRE);
    
    if(fullscreen){
        flags |= ALLEGRO_FULLSCREEN_WINDOW;
    }else {
        flags |= ALLEGRO_WINDOWED;
    }
    
    allegro_get_desktop_size(0, &screen_width, &screen_height);
    

    al_set_new_display_flags(flags);
    
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    
    if(strlen(title) > 0 || title){
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
    }
    
    queue = al_create_event_queue();
    if(NOT_VALID_PTR(queue)){
        error++;
    }
    
    timer = al_create_timer(1.0/FPS);
    
    if(NOT_VALID_PTR(timer)){
        error++;
    }
    
    debug_font = al_create_builtin_font();
    
    if(NOT_VALID_PTR(debug_font)){
        error++;
    }
    
    

    buffer = al_create_bitmap(new_w, new_h);
    
    if(NOT_VALID_PTR(buffer)){
        error++;
    }
    
    #undef NOT_VALID_PTR
    
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    
    
   
    

    al_clear_to_color(al_map_rgb_f(0,0,0));
    al_flip_display();
    
    return error;
}

void allegro_destroy(void){
    
    if(voice) al_destroy_voice(voice);
    if(mixer) al_destroy_mixer(mixer);
    
    if(display) al_destroy_display(display);
    if(queue) al_destroy_event_queue(queue);
    if(timer) al_destroy_timer(timer);
    if(debug_font) al_destroy_font(debug_font);
    return;
}

ALLEGRO_BITMAP *al_load_bitmap_safe(const char *path){
    ALLEGRO_BITMAP *b = NULL;
    b = al_load_bitmap(path);
    
    if(!b){
        b = al_create_bitmap(32,32);
        al_set_target_bitmap(b);
        al_clear_to_color(al_map_rgb_f(255,0,0));
        al_set_target_backbuffer(display);
        return b;
    }
    
    return b;
}

int enemy_count(void){
    int count = 0;
    for(int y = 0; y < ENEMY_ROW_Y; y++){
        for(int x = 0; x < ENEMY_ROW_X;x++){
            if(enemies[y][x].alive) count++;
        }
    }
    
    return count;
}

void load_sprites(void){
    
    for(int i = 0; i < SPRITE_MAX; i++){
        sprites[i] = al_load_bitmap_safe(sprite_assets_path[i]);
        ALLEGRO_PATH *asset_path = al_create_path(sprite_assets_path[i]);
        
        
        if(strcmp(".bmp", al_get_path_extension(asset_path)) == 0){
            al_convert_mask_to_alpha(sprites[i], al_map_rgb(255,0,255));
        }
        
        
        if(!sprites[i]){
            fprintf(stderr, "Error Loading: %s", sprite_assets_path[i]);
        }
        
        al_destroy_path(asset_path);
        asset_path = NULL;
    }
}

void enable_blend(void){
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
}

void enable_solid(void){
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
}


void score_draw_text(void){
    
    for (int i= 0; i < 10; i++){
        if(score_list[i].ttl > 0){
            al_draw_textf(font_list[FONT_PIXEL_BIG], al_map_rgb(255, 168, 0), score_list[i].x, score_list[i].y,0, "%s", score_list[i].text);
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

void score_add(TEXT *score, int num, float x, float y){
    

    TEXT *s = score_free(score);
    
    s->x = x;
    s->y = y;
    s->ttl = 80;
    s->shadow = 0;
    snprintf(s->text, 255, "+%d", num);
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


void new_game(void){
    
    gameover = 0;
    player.x = al_get_display_width(display) / 2 - 32;
    player.y = al_get_display_height(display) - 50;
    player.direction = 1;
    player.alive = TRUE;
    player.shot_time = 0;
    player.life = 100.0;
    player.lives = 3;
    line = 0;
    walk_time  = WALK_TIME_DELAY_PHASE1;
    
    
    memset(player.keypressed, 0, sizeof(player.keypressed));
    
    
    
    for(int y = 0; y < ENEMY_ROW_Y;y++){
        for(int x = 0; x < ENEMY_ROW_X;x++){
            
            int rand_type = game_rand(100);
            
            enemies[y][y].type =   rand_type > 50 ? 1 : 2;
            enemies[y][x].x = x * TILE * 1.5;
            enemies[y][x].y = y * TILE * 1.5;
            enemies[y][x].alive = 1;
            enemies[y][x].type = 1;
            enemies[y][x].life = 1;
            memset(enemies[y][x].bullets, 0, sizeof(enemies[y][x].bullets));
            
        }
      
    }
    
    //update_time = al_get_time();
    
}


void do_gameover(void){
    gameover = 1;
    
    for(int y = 0; y < ENEMY_ROW_Y;y++){
        for(int x = 0; x < ENEMY_ROW_X;x++){
                if(enemies[y][x].alive){
                    enemies[y][x].alive = false;
                }
        }
    }
}



void draw_life_bar(void){
    
    float total = fabs(player.life/100.0);
    
    float w = al_get_display_width(display) * total;
    

    al_draw_filled_rectangle(0, 0, w, 15 , al_map_rgb_f(0,0,1));
    al_draw_multiline_textf(font_list[FONT_PIXEL_SMALL], al_map_rgb(0,255,0), w/2,0, 0,20,0,"%.2f",  fabs(player.life/100.0) * 100);
}

void draw_debug(void){
    
    
    al_draw_multiline_textf(debug_font, al_map_rgb_f(1.0,0,0), 0,0, 300, 20,0,
    "TOTAL ENEMIES: %d\n"
    "SHOT TIME: %d\n"
    "SHOOT : %s\n"
    "LINES: %d lines\n"
    "Enemy Shoot Time %d\n"
    "Player life: %.2f",
    enemy_count(),
    player.shot_time,
    player.shoot ? "YES" : "No",
    line,
    enemy_shoot_time,
    player.life

   );
   
   
   return;
}


void draw_enemies(ENEMY enemy_list[ENEMY_ROW_Y][ENEMY_ROW_X], float offset_x, float offset_y){
    
    (void)offset_x; (void)offset_y;
    
    if(!gameover){
        for(int y = 0; y < ENEMY_ROW_Y;y++){
            for(int x = 0; x < ENEMY_ROW_X;x++){
                if( enemy_list[y][x].alive){
                    int type = enemy_list[y][x].type == 1 ? SPR_ENEMY01 : SPR_ENEMY02;
                    ALLEGRO_BITMAP *enemy_spr = sprites[type];
                    
                    al_draw_bitmap(enemy_spr, enemy_list[y][x].x,  enemy_list[y][x].y, 0);
                }

            }
        }
    
    }
    
    if(gameover > 0){
        al_draw_textf(debug_font, al_map_rgb_f(1.0,0,0), (int)al_get_display_width(display)/ 2,  
         (int)al_get_display_height(display) / 2.0,0, "GAME OVER BABY!"
        );
    }
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


void enemies_draw_bullets(void){
    
    for(int y = 0; y < ENEMY_ROW_Y;y++){
        for(int x = 0; x < ENEMY_ROW_X;x++){
            if(!enemies[y][x].alive) continue;
            
            for(int i = 0; i < MAX_BULLETS; i++){
                if(!enemies[y][x].bullets[i].alive) continue;
                
                BULLET *b = &enemies[y][x].bullets[i];
                //al_draw_filled_rectangle(b->x,b->y,b->x+8, b->y+8, al_map_rgb(255, 159, 67));
                al_draw_bitmap(sprites[SPR_ENEMYSHOT],b->x, b->y,0);
            }
            
        }
    }
}

void enemies_update_bullet(void){
        
    
        for(int y = 0; y < ENEMY_ROW_Y; y++){
            for(int x = 0;x < ENEMY_ROW_X;x++){

                ENEMY *enemy = &enemies[y][x];
                
                    
                    for(int i = 0; i < MAX_BULLETS;i++){
                        BULLET *bullet = &enemy->bullets[i];
                        
                        if(bullet->alive){
                          
                           int total = getCountBulletsAlive(enemy->bullets);
                           
                           for(int i = 0; i < total; i++){
                               bullet->x += cos(bullet->angle * DEG2RAD) * difficulty[game_difficulty].shot_speed;
                               bullet->y += sin(bullet->angle * DEG2RAD) * difficulty[game_difficulty].shot_speed;
                           }
                           
                           
                           if(!g_demo_start && rect_collision(player.x, player.y,32,32, bullet->x, bullet->y, 8,8) && player.alive){

                                if(player.life > 0){
                                    
                                    switch(difficulty[game_difficulty].type){
                                        default:
                                        case DIFF_NOVICE:
                                            player.life -= 40;
                                            break;
                                        
                                        case DIFF_EASY:
                                            player.life -= 40;
                                            break;
                                        
                                        case DIFF_NORMAL:
                                            player.life -= 25;
                                            break;
                                            
                                        case DIFF_HARD:
                                            player.life -= 25;
                                            break;
                                            
                                        case DIFF_NIGHTMARE:
                                            player.life -= 25;
                                            break;
                                    }
                                    
                                    bullet->alive = FALSE;
                                    play(SFX_HIT);
                                }
                           }
                           
                           if(bullet->x >= al_get_display_width(display)){
                               bullet->alive = false;
                           }
                           
                          if(bullet->x < 0){
                               bullet->alive = false;
                           }
                           
                            if(bullet->y >= al_get_display_height(display)){
                               bullet->alive = false;
                           }
                           
                          if(bullet->y < 0){
                               bullet->alive = false;
                           }
                                                     
                           
                        }
                    }
            }
        }

        
        if(enemy_shoot_time > 0){
            enemy_shoot_time--;
        }else {
            int enemy_index_x =  game_rand_range(0, ENEMY_ROW_X);
            int enemy_index_y =  game_rand_range(0, ENEMY_ROW_Y);
            
            ENEMY *e = &enemies[enemy_index_y][enemy_index_x];
            
            if(e->alive){
                int rnd = 0;
                
                switch(difficulty[game_difficulty].type){
                    case DIFF_NOVICE:
                        rnd = game_rand_range(1,1);
                        break;
                        
                    case DIFF_EASY:
                        rnd = game_rand_range(1,2);
                        break;
                        
                    case DIFF_NORMAL:
                        rnd = game_rand_range(2,3);
                        break;
                        
                    case DIFF_HARD:
                        rnd = game_rand_range(2,4);
                        break;
                    case DIFF_NIGHTMARE:
                        rnd = game_rand_range(1,6);
                        break;
                }
                
                for(int i = 0; i < rnd; i++){
                    
                    
                    
                    BULLET *b = create_shot(e->bullets, e->x, e->y, 1.0, 1.0);
                    b->color = al_map_rgb(255,0, 0);
                    
                   float dx = player.x - b->x;
                   float dy = player.y - b->y;
                   
                   double player_angle = atan2(dy, dx);
                   b->angle = RAD2DEG * player_angle;
                               
                   b->x = e->x +  cos(b->angle * DEG2RAD) * difficulty[game_difficulty].speed_multiplier;
                   b->y = e->y +  sin(b->angle * DEG2RAD) * difficulty[game_difficulty].speed_multiplier;
                    
                }
                
                enemy_shoot_time =  difficulty[game_difficulty].enemy_shot_time;
            }
            
            
        }
        
    
}


void enemies_set_down(ENEMY enemies[ENEMY_ROW_Y][ENEMY_ROW_X]){
        for(int y = 0; y < ENEMY_ROW_Y; y++){
            for(int x = 0;x < ENEMY_ROW_X;x++){
                enemies[y][x].y +=  TILE;
                line++;
            }
        }
}


void draw_player(float x, float y){
        
    al_draw_filled_triangle(x,y,x+32,y+32,x,y+32, al_map_rgb_f(1.0,0,0));
}



void player_update_shot(void){
    for(int i = 0; i < MAX_BULLETS; i++){
        if(player.bullets[i].alive && player.bullets[i].ttl > 0){
           player.bullets[i].y -= 8.0;
           
           if( player.bullets[i].y < -16 || player.bullets[i].y > al_get_display_height(display) + 16 ||
               player.bullets[i].x < -16 || player.bullets[i].x > al_get_display_width(display) + 16){
                player.bullets[i].alive = false;
           }
           
           
           if(g_spaceship_entity->alive && g_spaceship_entity && rect_collision(player.bullets[i].x, player.bullets[i].y, 8,8, g_spaceship_entity->x, g_spaceship_entity->y, 32,32)){
                g_spaceship_entity->alive = FALSE;
                g_ship_active = FALSE;
                score_add(score_list, 1000,  g_spaceship_entity->x, g_spaceship_entity->y);
                player.score += 1000;
                al_stop_samples();
            }
           
        }
    }
    
    if(player.shot_time > 0){
        player.shoot = TRUE;
    }else if(player.shot_time < 0){
        return;
    }else {
      player.shoot = FALSE;
    }
    
    player.shot_time--;
    return;
    
}

void player_draw_shot(void){
    
    for(int i = 0; i < MAX_BULLETS; i++){
        if(player.bullets[i].alive){
            
            al_draw_line( player.bullets[i].x+4, player.y+4, player.bullets[i].x + 4, player.bullets[i].y, al_map_rgba_f(0.5,0.5,0.5,0.2),1);
            al_draw_bitmap(sprites[SPR_PLAYERSHOT], player.bullets[i].x, player.bullets[i].y,0);

    
        }
    }
}

void player_update(void){
    
    if(player.alive){
        if(player_keys[ALLEGRO_KEY_A] || player_keys[ALLEGRO_KEY_LEFT]){
            player.x -= 2.0;
            player.keypressed[0] = TRUE;
        }else {
            player.keypressed[0] = FALSE;
        }
        
        if(player_keys[ALLEGRO_KEY_D] || player_keys[ALLEGRO_KEY_RIGHT]){
            player.x +=  2.0;
            player.keypressed[1] = TRUE;
        }else {
            player.keypressed[1] = FALSE;
        }
        
        if(player_keys[ALLEGRO_KEY_SPACE] && !player.shoot && !gameover){
            create_shot(player.bullets, player.x, player.y,0.0,1.0);
            player.shot_time = 25;
            play(SFX_LASER);
        }
    


    }
    
}


int  enemy_bullet_collision(ENEMY *enemy, BULLET *bullet){
    for(int i = 0; i < MAX_BULLETS; i++){
        
        if(rect_collision(enemy->x,enemy->y,32,32, bullet[i].x,bullet[i].y, 8,8) && bullet[i].alive){
            bullet[i].alive = FALSE;
            return TRUE;
        }
    }
    
    return FALSE;
}


const float explosion_speed[4] = {
     .5f,
     1.0f,
     1.2f,
     2.0f
};


int last_pickup = -1;

void enemies_update(void){
    
    int phase = WALK_TIME_DELAY_PHASE1;
    for(int y = 0; y < ENEMY_ROW_Y; y++){
        for(int x = 0;x < ENEMY_ROW_X;x++){
             if(enemies[y][x].alive) {
                 enemies[y][x].x +=  difficulty[game_difficulty].speed_multiplier * enemy_direction;
                 walk_time--;
                 
                 if(walk_time <= 0){
                    play(SFX_WALK);
                    
                    
                    if(line > 100){
                        phase = WALK_TIME_DELAY_PHASE2;
                    }else if(line > 150){
                        phase = WALK_TIME_DELAY_PHASE3;
                    }else if(line > 200){
                        phase = WALK_TIME_DELAY_PHASE4;
                    }else if(line > 300){
                        phase = WALK_TIME_DELAY_PHASE5;
                    }else {
                        phase = WALK_TIME_DELAY_PHASE1;
                    }
                    
                    
                    walk_time = phase;
                 }
                 
                 
                 if(enemies[y][x].x + TILE > (al_get_display_width(display) - TILE - 1)){
                     
                     enemies_set_down(enemies);
                     enemy_direction = -1;
                 }
                 
                 if(enemies[y][x].x  < 0 ){
                     enemies_set_down(enemies);
                     enemy_direction = 1;
                 }
                 
                 
                 if(enemies[y][x].y > al_get_display_height(display) - TILE - 1){
                            do_gameover();
                            return;
                 }
                 
                 
                 
                 if(enemy_bullet_collision(&enemies[y][x], player.bullets)){
                        enemies[y][x].alive = FALSE;
                        player.score += 100;
                        score_add(score_list, 100, enemies[y][x].x, enemies[y][x].y);
                        int rand = game_rand_range(0,2);
                        int speed_index = game_rand_range(0,4);
                        play_sound(rand,1.0,0.0, explosion_speed[speed_index], ALLEGRO_PLAYMODE_ONCE);
                        
                        int pickup_chance = game_rand_range(0,100);
                        int pickup = game_rand_range(0, PICKUP_TOTAL);
                        last_pickup = pickup;
                        
                        
                        if(pickup_chance < 6){
                            //int pickup = game_rand_range(0, PICKUP_TOTAL);
                            pickup_add(enemies,x,y, PICKUP_DOUBLE_SHOOT);
                            player.score += 50;
                            score_add(score_list, 50, enemies[y][x].x,enemies[y][x].y-50);
                            play(SFX_POWERUP);
                            
                            
                        }
                        
                      
                        particle_explosion(particles, enemies[y][x].x, enemies[y][x].y, 80, 50, 10);
                        
                 

                        return;
                 }
                    
                
                
                if(rect_collision(player.x,player.y, 32,32, enemies[y][x].x,enemies[y][x].y,32,32)){
                        if(!g_demo_start){
                            do_gameover();
                            
                        }else{
                            new_game();
                        }
                        return;
                }
                
        
             }
        }
    }
    

    return;
        
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



void stars_draw(ALLEGRO_BITMAP *bmp, STAR *stars, int frame_only){
    if(bmp){
       
       al_lock_bitmap(bmp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
       al_set_target_bitmap(bmp);
        for(int i  = 0; i < MAX_STARS; i++){
            if(frame_only){
                
                int rnd = game_rand(100);
                
                int rx  = game_rand_range(0, al_get_display_width(display));
                int ry  = game_rand_range(0, al_get_display_width(display));
                
                
                if(rnd <= 20){
                     al_put_pixel(rx,ry, al_map_rgb(255,255,255));
                     al_put_pixel(rx+1,ry, al_map_rgb(255,255,255));
                     al_put_pixel(rx,ry+1, al_map_rgb(255,255,255));
                     al_put_pixel(rx+1,ry+1, al_map_rgb(255,255,255));
                }else if(rnd <= 30){
                    
                     al_put_pixel(rx,ry, al_map_rgb(255,255,255));
                     al_put_pixel(rx+1,ry, al_map_rgb(255,255,255));
                     al_put_pixel(rx+2,ry, al_map_rgb(255,255,255));
                     al_put_pixel(rx,ry+1, al_map_rgb(255,255,255));
                     al_put_pixel(rx,ry+2, al_map_rgb(255,255,255));
                     al_put_pixel(rx+1,ry+1, al_map_rgb(255,255,255));
                     al_put_pixel(rx+2,ry+2, al_map_rgb(255,255,255));
                     
                     
                }else {   
                     al_put_pixel(rx,ry, al_map_rgb(255,255,255));
                }
                
                
                continue;
            }else {
                al_put_pixel(stars[i].x, stars[i].y, al_map_rgb_f(1,1,1));
            }
        }

        
        al_set_target_backbuffer(display);
        al_unlock_bitmap(bmp);
    }
}


void stars_init(void){
    
    stars_bg = al_create_bitmap(al_get_display_width(display), al_get_display_height(display));
    
    if(stars_bg == NULL){
        return;
    }
    
    for(int i  = 0; i < MAX_STARS; i++){
        int rx = game_rand_range(0, al_get_display_width(display));
        int ry = game_rand_range(0, al_get_display_height(display));
        
        stars[i].x = rx;
        stars[i].x = ry;
        stars[i].vx = 1.0;
        stars[i].vy = 1.0;
        stars[i].ttl = game_rand_range(100,600);
    }
    
}

void stars_destroy(void){
    if(stars_bg) {
        al_destroy_bitmap(stars_bg);
        stars_bg = NULL;
    }
}



void pickups_init(void){
    
    memset(pickup, 0x0, sizeof(PICKUP) * MAX_PICKUP);
}


void particle_create(PARTICLE *p, int x, int y, double vx,  double vy, int color, int life){
        p->x = x;
        p->y = y;
        p->vx = vx;
        p->vy = vy;
        p->color = color;
        p->ttl = life;
}


void particle_draw(ALLEGRO_BITMAP *bmp, PARTICLE *p, int ox, int oy, const ALLEGRO_COLOR color){
    
 

    ALLEGRO_VERTEX vtx[DEFAULT_PARTICLES] = {0};
    
    
    if(bmp)
        al_set_target_bitmap(bmp);
    
    
    for(int i = 0; i < DEFAULT_PARTICLES; i++){
        if(p[i].ttl){
            
            int x, y;
            
          

            x = p[i].x - ox;
            y = p[i].y - oy;
            
            if(x < 0 || x > al_get_display_width(display) + 16 ||  y < 16 || y > al_get_display_height(display) + 16) return;
            
            vtx[i].x = x;
            vtx[i].y = y;
            vtx[i].z = 0;
            vtx[i].u = 0;
            vtx[i].v = 0;
            vtx[i].color = color;
            
        }
    }
    
    al_draw_prim(vtx, NULL, NULL, 0, DEFAULT_PARTICLES, ALLEGRO_PRIM_POINT_LIST);

    if(bmp)
        al_set_target_backbuffer(display);
    
    
}

void particle_update(PARTICLE *p){
    p->x += cos(1 * DEG2RAD) *  p->vx;
    p->y += sin(-90 * DEG2RAD) *  p->vy;
    p->ttl--;
}

void particle_reset(PARTICLE *p, int max){
    for(int i = 0; i < max; i++){
        p[i].ttl = 0;
    }
}

void particle_clear_buffer(PARTICLE *plist, ALLEGRO_BITMAP *bmp,  ALLEGRO_COLOR color){
   UNUSED(color);
    for(int i = 0; i < DEFAULT_PARTICLES; i++){
        if(!plist[i].ttl){
            
            al_set_target_bitmap(bmp);
            al_lock_bitmap(bmp, al_get_bitmap_format(bmp), ALLEGRO_LOCK_WRITEONLY);
            //al_put_pixel(plist[i].x, plist[i].y, color);
            //al_draw_pixel(plist[i].x, plist[i].y, color);
            al_unlock_bitmap(bmp);
            al_set_target_backbuffer(display);
            
        }
    }
    
}

PARTICLE *particle_free(PARTICLE *p, int max){
   int i = 0;
   while(p[i].ttl && i < max) i++;
   if(i == max) return NULL;
   return &p[i];
}


void particle_explosion(PARTICLE *plist, int x, int y, int spread, int particle_count, int life){
    int i;
    PARTICLE *p;
    
    float angle, speed;
    
    
    
    
    for(i = 0;i < particle_count;i++){
        p = particle_free(plist, DEFAULT_PARTICLES);
        if(p != NULL){
            
            speed = game_rand_range(1,3) + 0.5;
            angle = game_rand_range(0,360);
            
            particle_create(p, x + game_rand(spread) - spread / 2, 
                               y + game_rand(spread) - spread / 2,
                               cos(angle * DEG2RAD) * speed,  //speed/100.0 - 0.5,
                               sin(angle * DEG2RAD) * speed,
                                rand()%4 + 1,
                               (life ? life : 70 -  game_rand(50))
                            );
        }
    }
    
}


void menu_update(MENU *menu_list, int opt){
    
    ALLEGRO_MOUSE_STATE mouse;
    
    al_get_mouse_state(&mouse);
    
    static int sound_played = 0;
    int counter = al_get_timer_count(timer) / 60 % 4;

    for(int i = 0; i < MAX_MENU; i++){

        
        if(menu_list[i].menu_id <= 0) continue;
        
        int px = al_get_display_width(display) / 2   - al_get_text_width(font_list[FONT_PIXEL_MENU_BIG], menu_list[i].opt_name);
        int py = al_get_display_height(display) / 2  - al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]);     
        
        
        int text_size = al_get_text_width(font_list[FONT_PIXEL_MENU_BIG], menu_list[i].opt_name);
        int text_line_size = al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]);
        
        menu_list[i].x = px;
        menu_list[i].y = py + i * 64;
        
        if(!sound_played && counter >= 3 && rect_collision(mouse.x, mouse.y,32,32, menu_list[i].x, menu_list[i].y, text_size, text_line_size)){
            play_sound(SFX_LASER,1.0f, 0.0, 0.5f, ALLEGRO_PLAYMODE_ONCE);
            sound_played = 1;
        }
        
        if(counter > 3){
            sound_played = 0;
        }
        
      
        
        if((mouse.buttons & 1) && rect_collision(mouse.x, mouse.y,32,32, menu_list[i].x, menu_list[i].y, text_size, text_line_size)){
            g_menu_cursor.opt = menu_list[i].menu_id;
           
        }
  
        
        
        if(menu_list[i].menu_id == opt){
            
            if(menu_list[i].type > 0){
                    for(int j = 0; j < MAX_MENU; j++){
                            if(menu_list[i].next_page && menu_list[i].next_page[j].menu_id == opt){
                                menu_list[i].next_page[j].menu_callback(&menu_list[i].next_page[j], NULL);
                                g_menu_cursor.opt = 0;
                                return;
                            }
                    } 
            }else {
                if(!menu_list[i].menu_callback){
                    printf("MENU CALLBACK NULL\n");
                    g_menu_cursor.opt = 0;
                    continue;
                }
                
                menu_list[i].menu_callback(& menu_list[i], NULL);
                g_menu_cursor.opt = 0;
            }
            
            return;
        }
        
        
        
    }
}

void demo_init(void){
    new_game();
}

int64_t g_record_actual_frame = 0;

void demo_update(void){
        player_update_shot();
        enemies_update_bullet();
        enemies_update();
        //player_update();
        pickup_update();

}

void demo_draw(struct RENDER_ARGS *args){
    
                al_set_target_bitmap(buffer);
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(stars_bg,0,0,0);
                al_draw_bitmap(args->particles_buffer,0,0,0);
                draw_spaceship();
                draw_enemies(enemies,0,0);
                //draw_player(player.x, player.y);
                player_draw_shot();
                pickup_draw();
                enemies_draw_bullets();
                //draw_life_bar();
                score_draw_text();
                //al_draw_textf(font_list[FONT_PIXEL_SMALL], al_map_rgb_f(1,1,1),10,20, 0, "SCORE: %08d", player.score);
                //al_draw_textf(font_list[FONT_PIXEL_SMALL], al_map_rgb_f(1,1,1),10,35, 0, "LIVES %02d", player.lives);     
                particle_draw(NULL, particles,0,-8.0, al_map_rgb(255, 168, 0));
                al_set_target_backbuffer(display);
                
                al_draw_bitmap(buffer, 0,0,0);
}




void menu_draw(MENU *menu_list){
    
    
    
    ALLEGRO_COLOR blue = al_premul_rgba(52, 73, 94, 0.1);
    ALLEGRO_COLOR white = al_premul_rgba(52, 73, 94, 0.1);
    
    ALLEGRO_VERTEX menu_bg[3] = {
        {0,0,0,1,1, al_map_rgb(52, 73, 94)},
        {0, al_get_display_height(display),0,1,1, blue},
        {al_get_display_width(display), al_get_display_height(display),0,1,1, white}
    };
    
    

    al_draw_prim(menu_bg, NULL, NULL, 0, 3, ALLEGRO_PRIM_TRIANGLE_FAN);
   
    
    for(int i = 0; i < MAX_MENU; i++){
        
        if(menu_list[i].menu_id <= 0) continue;
        
        int px = al_get_display_width(display) / 2   - al_get_text_width(font_list[FONT_PIXEL_MENU_BIG], menu_list[i].opt_name);
        int py = al_get_display_height(display) / 2  - al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]);
        
        int text_size = al_get_text_width(font_list[FONT_PIXEL_MENU_BIG], menu_list[i].opt_name);
        int text_line_size = al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]);
        
        al_draw_textf(font_list[FONT_PIXEL_MENU_BIG], al_map_rgb_f(1,1,1),  px,  py + i * 64, 0, "%s", menu_list[i].opt_name);
        al_draw_rectangle(menu_list[i].x,menu_list[i].y, menu_list[i].x + text_size, menu_list[i].y + text_line_size, al_map_rgb_f(1,0,0),1);
      
        
    }
    
  
}


void gameplay_update(void){
                    
        if(player_keys[ALLEGRO_KEY_R]){
            if(gameover) gameover = 0;
            //new_game();
            g_gamestate = GAMESTATE_TYPE_MENU;
        }

        if(player.life <= 0){
           if(player.lives > 1){
               player.life = 100;
               player.lives--;
           }else {
               do_gameover();
           }
        }

        player_update_shot();
        enemies_update_bullet();
        enemies_update();
        player_update();
        score_update_text();
        pickup_update();
        update_spaceship();
        


        for(int i = 0; i  < DEFAULT_PARTICLES; i++){
              if(particles[i].ttl) particle_update(&particles[i]);
        }


        g_ship_counter = (al_get_timer_count(timer) / 60) % 12;



        if(g_ship_counter >= 11 && !g_ship_active){
            int ship_chance = game_rand(1000);
            
            if(ship_chance < 3){
                
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
}

void gameplay_draw(struct RENDER_ARGS *args){
      
    
                al_set_target_bitmap(buffer);
                
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_bitmap(stars_bg,0,0,0);
                al_draw_bitmap(args->particles_buffer,0,0,0);
                draw_spaceship();
                draw_enemies(enemies,0,0);
                draw_player(player.x, player.y);
                player_draw_shot();
                pickup_draw();
                enemies_draw_bullets();
                draw_life_bar();
                score_draw_text();
                al_draw_textf(font_list[FONT_PIXEL_SMALL], al_map_rgb_f(1,1,1),10,20, 0, "SCORE: %08d", player.score);
                al_draw_textf(font_list[FONT_PIXEL_SMALL], al_map_rgb_f(1,1,1),10,35, 0, "LIVES %02d", player.lives);     
                particle_draw(NULL, particles,0,-8.0, al_map_rgb(255, 168, 0));
                
                al_set_target_backbuffer(display);
                
                al_draw_bitmap(buffer, 0,0,0);
                
                
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
    
    if(allegro_create_display_context(0,0,0,1, "INVADERS!") > 0){
        fprintf(stderr, "Error: failed to load display context");
        exit(1);
    }
    
    int close = 0;
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
    
    init_audio();
    load_sprites();
    font_init();
    
    hiscore_init();
    stars_init();
    new_game();
   
    
    g_spaceship_entity = al_malloc(sizeof(ENEMY));
    memset(g_spaceship_entity, 0, sizeof(*g_spaceship_entity));
    
    //int flags = al_get_new_bitmap_flags();
    //al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    ALLEGRO_BITMAP *particles_buffer = al_create_bitmap(al_get_display_width(display), al_get_display_height(display));   
    //al_set_new_bitmap_flags(flags);
    
    particle_reset(particles, DEFAULT_PARTICLES);
    stars_draw(stars_bg, stars, TRUE);
    
    
    g_menu_cursor.menu_count = sizeof(game_menu) / sizeof(game_menu[0]);
    g_menu_cursor.cursor = sprites[SPR_ENEMY02];
    //g_menu_cursor.x =  (float)al_get_display_width(display) / 2 - 32;
    
    
    struct RENDER_ARGS render_args;
    render_args.particles_buffer = particles_buffer;
    

    while(!close){
        ALLEGRO_EVENT e;
        
        al_wait_for_event(queue, &e);
        
        if(redraw && al_event_queue_is_empty(queue)){
            
            al_set_clipping_rectangle(0, 0, screen_width, screen_height);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            
            
            
            if(g_gamestate == GAMESTATE_TYPE_MENU){

                    
                    if(g_demo_start) 
                        demo_draw(&render_args);
                    menu_draw(game_menu);
            }
            
            if(g_gamestate == GAMESTATE_TYPE_GAMEPLAY){
                gameplay_draw(&render_args);             
            }
            
            
            if(g_gamestate == GAMESTATE_TYPE_HISCORE){
                hiscore_draw();
            }
            
            al_flip_display();
            redraw = 0;
        } 
        

        
        switch(e.type){
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                close =  TRUE;
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
                    if(keybuffer_counter > 255) keybuffer_counter = 0;
                    
                    al_lock_mutex(key_mutex);
                    keybuffer[keybuffer_counter] = e.keyboard.keycode;
                    al_unlock_mutex(key_mutex);
                }
                break;
                
            case ALLEGRO_EVENT_TIMER:
            {
                
                if(g_gamestate == GAMESTATE_TYPE_HISCORE){
                    hiscore_update();
                }
                
                if(g_gamestate == GAMESTATE_TYPE_MENU){
                    if(!g_demo_start){
                        demo_init();
                        g_demo_start = TRUE;
                    }
                    
                    
                    if(g_demo_start) 
                        demo_update();
                    menu_update(game_menu, g_menu_cursor.opt);
                }
                
                if(g_gamestate == GAMESTATE_TYPE_GAMEPLAY){
                    gameplay_update();
                }
                
                
                redraw = 1;
            }
                break;
        }
    }
    


    al_destroy_bitmap(particles_buffer);
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
    
    new_game();
    g_gamestate = GAMESTATE_TYPE_GAMEPLAY;
}


static void menu_hiscore_click(struct MENU *menu, void *args){
    UNUSED(menu); UNUSED(args);
    g_gamestate = GAMESTATE_TYPE_HISCORE;
}

void hiscore_init(void){

    
    int lines = 0;
    int line_height = 0;
    
    for(int i = 0; i < 25;i++){
        if(hiscore[i].score > 0) lines++;
    }
    
    line_height = lines * al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]) + 128;
    hiscore_bitmap = al_create_bitmap(al_get_display_width(display),  line_height );
    
    if(!hiscore_bitmap){
        return;
    }
    
    struct _stat st;
    
    if(_stat("hiscore.dat", &st) < 0){
        hiscore_fp = fopen("hiscore.dat", "wb+");
        
        if(!hiscore_fp){
            return;
        }
        
        for(int i = 0; i < MAX_HISCORE;i++){
            fwrite(hiscore[i].name, sizeof(hiscore[i].name),1, hiscore_fp);
            fwrite(&hiscore[i].score, sizeof(int),1, hiscore_fp);
        }
        
        fwrite("END\r\n",5,1,hiscore_fp);
        fclose(hiscore_fp);
    }
    
    
    hiscore_fp = fopen("hiscore.dat", "rb+");
    memset(hiscore, 0, sizeof(hiscore));
        
    int c = 0;
    while(!feof(hiscore_fp)){
        char name[20];
        int score;
        
        
        
        fread(name, 20,1, hiscore_fp);
        fread(&score,sizeof(int),1, hiscore_fp);
        
        if(score > 0){
            snprintf(hiscore[c].name,20,"%s", name);
            hiscore[c].score = score;
            c++;
        }
    }


    fclose(hiscore_fp);
    
    

}

void hiscore_update(void){
    
     if(keybuffer[keybuffer_counter] != '\0'){
         
         memset(keybuffer, 0, 255);
         g_gamestate = GAMESTATE_TYPE_MENU;
     }
     return;
    
}
void hiscore_draw(void){
    
 
    
    for(int i = 0; i < MAX_HISCORE; i++){
        
        int h = al_get_font_line_height(font_list[FONT_PIXEL_MENU_BIG]);
        
        if(hiscore[i].score > 0){
            al_draw_textf(font_list[FONT_PIXEL_MENU_BIG], al_map_rgb_f(1,1,1),
            10,
            200 + h*i,
            0,
            "%s", hiscore[i].name
            );
            
            
            al_draw_textf(font_list[FONT_PIXEL_MENU_BIG], al_map_rgb_f(1,1,1),
            (float)al_get_bitmap_width(hiscore_bitmap)/2 + 400,
            200 + h*i,
            0,
            "%05d", hiscore[i].score
            );
            
        }
    }
    //al_set_target_backbuffer(display);
    
    //al_draw_bitmap(hiscore_bitmap,0,0,0);
    
    
    return;
}