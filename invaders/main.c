#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>


static void write_log(const char *fmt,...);

#define MAX_ENEMIES_COUNT 25

static ALLEGRO_TEXTLOG *log_window = NULL;


#define SCREEN_W 800
#define SCREEN_H 600


#define TILE_SIZE 32

#ifdef DEBUG
#define LOGW(x, ...) write_log(x"\n",##__VA_ARGS__);
#else
#define LOGW(x,...)
#endif
 
#define ENEMY_ROW 5
#define ENEMY_COL 5

struct PLAYER {
    float x;
    float y;
    float vel_x;
    float vel_y;
    int alive;
    int health;
    int score;
};

typedef struct PLAYER PLAYER;

struct ENEMY {
    float x;
    float y;
    int hitpoints;
    int type;
    int alive;
    float vel_x;
    float vel_y;
    int direction;
};

typedef struct ENEMY ENEMY;


#define MAX_BULLETS 50

struct BULLET {
    float x;
    float y;
    float vel_x;
    float vel_y;
    int ttl;
    int alive;
    int type;
};

typedef struct BULLET BULLET;


static BULLET player_bullets[MAX_BULLETS] = {0};
static BULLET enemies_bullets[MAX_BULLETS] = {0};
static int bullet_counter = 0;
static int enemy_bullet_counter = 0;


static ALLEGRO_DISPLAY* display = NULL;
static ALLEGRO_EVENT_QUEUE* queue = NULL;
static ALLEGRO_BITMAP* screen = NULL;
static ALLEGRO_KEYBOARD_STATE kbdstate;
static ALLEGRO_TIMER* timer = NULL;

static ALLEGRO_BITMAP* background = NULL;
static ALLEGRO_BITMAP *enemy_spr01 = NULL;
static ALLEGRO_BITMAP *player_sprite = NULL;

static ALLEGRO_BITMAP *enemy_shot = NULL;
static ALLEGRO_BITMAP *player_shot = NULL;
static ALLEGRO_BITMAP *title_intro = NULL;
static ALLEGRO_BITMAP *enemy_spr02 = NULL;
ALLEGRO_FONT *text_font = NULL;

static float  shot_time = 0;
static float enemy_shot_time = 0;

enum {
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_FIRE
};

static ENEMY  s_enemy_list[MAX_ENEMIES_COUNT] = {0};
static int keys[6] = {0};
static int enemy_count = MAX_ENEMIES_COUNT;


 
enum{
    PLAYER_1,
    PLAYER_2
};

static PLAYER player = {0};


enum {
    ENEMY_SPEED_NORMAL,
    ENEMY_SPEED_FAST,
    ENEMY_SPEED_FASTER
};

static float enemy_speed_table[] = {
    1.0,
    2.5,
    4.5
};


enum {
        GAMESTATE_TITLE,
        GAMESTATE_PLAY,
        GAMESTATE_GAMEOVER,
        GAMESTATE_CLOSE
};


int game_state = GAMESTATE_TITLE;



//prototypes
void remove_enemy(ENEMY *enemy);
static void write_log(const char *fmt,...);
void init_player();
int collision_rect(float xa, float ya, int wa, int ha, float xb, float yb, int wb, int hb);
void init_enemies();
int init_allegro(void);
void register_events();
void load_sprites();
void draw_enemies();
int create_window(int w, int h, int fullscreen);
void unload_assets();
void destroy_window();
void draw_background();
void update_player_pos();
void draw_player();
void bullet_fire(BULLET bullets[MAX_BULLETS], float x, float y);
void bullet_delete(BULLET *bullet);
void bullet_update(BULLET bullets[MAX_BULLETS]);
void draw_bullets(BULLET bullets[MAX_BULLETS]);
void collision_bullet_to_enemies(BULLET bullets[MAX_BULLETS], ENEMY enemies[MAX_ENEMIES_COUNT]);
void player_input_keyboard(ALLEGRO_EVENT *ev);
void remove_enemy(ENEMY *enemy);
void update_enemies();
void update();



static void write_log(const char *fmt,...){
    
    char buffer[1024] = {0};
    
    va_list lst;
    va_start(lst, fmt);
    
    vsnprintf(buffer, sizeof(buffer), fmt, lst);
    va_end(lst);
    
    if(!log_window){
        fprintf(stdout, buffer);
        return;
    }
    
    al_append_native_text_log(log_window, buffer);

}



void init_player(){
    player.x = (SCREEN_W / 2) - TILE_SIZE;
    player.y = (SCREEN_H / 2) + 140;
    player.vel_x = 2.5f;
    player.vel_y = 0.0;
}

int collision_rect(float xa, float ya, int wa, int ha, float xb, float yb, int wb, int hb)
{
    if (xa < xb + wb && xa + wa > xb && ya < yb + hb && ha + ya > yb){
        return 1;
    }
    return 0;
}

void init_enemies(){
    
    
    int gx,gy;
    
    for(gy = 0; gy < ENEMY_ROW;gy++){
        for(gx = 0; gx < ENEMY_COL;gx++){
            ENEMY *en =  &s_enemy_list[gy * ENEMY_ROW + gx];
            
            en->x = gx * (TILE_SIZE * 1.5);
            en->y = gy * (TILE_SIZE * 1.5);
            en->hitpoints = 2;
            en->alive     = 1;
            en->type      = rand() % 2;
            en->direction = 1;
            en->vel_x =   enemy_speed_table[ENEMY_SPEED_NORMAL];
            en->vel_y =  enemy_speed_table[ENEMY_SPEED_NORMAL];
        }
    }
    
}


int init_allegro(void)
{

    if (!al_init()) {
        goto error_critical;
    }

    if (!al_install_keyboard()) {
        goto error_critical;
    }

    if (!al_install_mouse()) {
        goto error_critical;
    }
    
    if (!al_init_native_dialog_addon()){
        LOGW("Cannot Load NATIVE DIALOG");
    }

    if (!al_init_font_addon()) {
        LOGW("Cannot Init Fonts!");
    }

    if (!al_init_ttf_addon()) {
        LOGW("Cannot Init TTF Fonts!");
    }

    if (!al_init_primitives_addon()) {
        LOGW("Failed to load primitives");
    }

    if (!al_install_audio()) {
        LOGW("Audio not found");
    }

    if (!al_init_acodec_addon()) {
        LOGW("Audio not found");
    }
    

    
    if(!al_init_image_addon()) {
        LOGW("Images Loaded");
    }
    
#ifdef DEBUG
    log_window = al_open_native_text_log("DEBUG:", ALLEGRO_TEXTLOG_NO_CLOSE|ALLEGRO_TEXTLOG_MONOSPACE);
#endif


    return 1;

error_critical:
    al_uninstall_system();
    fprintf(stdout, "Error Critical!");
    return 0;
}

void register_events()
{

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
}


ALLEGRO_BITMAP *image_safe_mode(){
    ALLEGRO_BITMAP *tmp = al_create_bitmap(32,32);
    
    al_set_target_bitmap(tmp);
    ALLEGRO_FONT *fnt = al_create_builtin_font();
    al_clear_to_color(al_map_rgb(255,0,0));
    al_draw_text(fnt,al_map_rgb(255,255,255), 0,0,0,"TMP");
    al_set_target_backbuffer(display);
    al_destroy_font(fnt);
    return tmp;
}

void load_sprites()
{
    background = al_load_bitmap("assets//bg01.bmp");
    if (!background) {
         background =     enemy_spr01 = image_safe_mode();
        return;
    }
    
    enemy_spr01 = al_load_bitmap("assets//enemy01.bmp");
    
    if (!enemy_spr01){
        enemy_spr01 = image_safe_mode();
    }
    
    enemy_spr02 = al_load_bitmap("assets//enemy02.bmp");
    
    if(!enemy_spr02){
        enemy_spr02 = image_safe_mode();
    }
    al_convert_mask_to_alpha(enemy_spr02, al_map_rgb(255,0,255));
    
    player_sprite = al_load_bitmap("assets//cannon.bmp");
    
    if(!player_sprite){
        player_sprite = image_safe_mode();
    }
    
    
    player_shot = al_load_bitmap("assets//pshot.bmp");
    enemy_shot = al_load_bitmap("assets//eshot.bmp");
    
    title_intro = al_load_bitmap("assets//title.png");
    
   al_convert_mask_to_alpha(enemy_spr01, al_map_rgb(255,0,255));
   al_convert_mask_to_alpha(player_sprite, al_map_rgb(255,0,255));
   al_convert_mask_to_alpha(player_shot, al_map_rgb(255,0,255));
   al_convert_mask_to_alpha(enemy_shot, al_map_rgb(255,0,255));
   
   text_font = al_create_builtin_font();
   
}

void draw_enemies(){
     for(int i = 0; i < MAX_ENEMIES_COUNT;i++){
         if (s_enemy_list[i].alive){
             
             ALLEGRO_BITMAP *spr = s_enemy_list[i].type == 0 ? enemy_spr01 : enemy_spr02; 
             
             al_draw_bitmap(spr, s_enemy_list[i].x,s_enemy_list[i].y,0);
         }
    }
}

int create_window(int w, int h, int fullscreen)
{

    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
    display = al_create_display(w, h);

    if(!display) {
        LOGW("display failed to be created, error");
        return 0;
    }

    LOGW("display created with %dx%d", w, h);

    queue = al_create_event_queue();

    if(!queue) {
        LOGW("display queue failed to be created!");
        return 0;
    }

    al_get_keyboard_state(&kbdstate);

    timer = al_create_timer(1.0 / 60.0);
    al_start_timer(timer);

    al_set_window_title(display, "SPACE INVADERS?");
    register_events();
    return 1;
}

void unload_assets(){
    if(background) al_destroy_bitmap(background);
    if(enemy_spr01) al_destroy_bitmap(enemy_spr01);
}

void destroy_window(){
    unload_assets();
    if(display) al_destroy_display(display);
    if(queue) al_destroy_event_queue(queue);
    if(screen) al_destroy_bitmap(screen);
    if(timer) al_destroy_timer(timer);
    
    if(log_window) al_close_native_text_log(log_window);
}

void draw_background()
{
    for(int i = 0; i < al_get_display_width(display) / 32; i++) {

        int sw = al_get_bitmap_width(background);
        int sh = al_get_bitmap_height(background);
        al_draw_bitmap(background, TILE_SIZE * i, 0,0);
    }
}


void update_player_pos(){
        
    if(player.x > SCREEN_W - TILE_SIZE){
        player.x =  SCREEN_W - TILE_SIZE;
    }
    
    if(player.x <= 0){
        player.x = 0;
    }
    
}



void draw_player(){
    
    al_draw_bitmap(player_sprite, player.x, player.y,0);
}


void bullet_fire(BULLET bullets[MAX_BULLETS], float x, float y){
        
    if(bullet_counter >= MAX_BULLETS){
                bullet_counter = 0;
        }
        
        if(!bullets[bullet_counter].alive){
            bullets[bullet_counter].x = player.x;
            bullets[bullet_counter].y = (SCREEN_H - 140);
            bullets[bullet_counter].alive = 1;
            bullets[bullet_counter].vel_x = 0;
            bullets[bullet_counter].vel_y = 4.5;
            LOGW("%ld", al_get_timer_count(timer));
            
            
            
            bullet_counter++;
        }

}

void bullet_clear(BULLET bullet[MAX_BULLETS]){
    for(int i = 0; i < MAX_BULLETS;i++){
        bullet[i].x = 0;
        bullet[i].y = 0;
        bullet[i].vel_x = 0;
        bullet[i].vel_y = 0;
        bullet[i].alive = 0;
    }
}

void enemy_shoot_bullets(ENEMY *enemy, BULLET bullets[MAX_BULLETS]){
    
    if(enemy_bullet_counter >= MAX_BULLETS){
        enemy_bullet_counter = 0;
    }
    if(!bullets[enemy_bullet_counter].alive){

        bullets[enemy_bullet_counter].x = enemy->x;
        bullets[enemy_bullet_counter].y = enemy->y;
        bullets[enemy_bullet_counter].alive = 1;
        bullets[enemy_bullet_counter].vel_x = 0;
        bullets[enemy_bullet_counter].vel_y = 4.5;
        bullets[enemy_bullet_counter].type = 1;
        enemy_bullet_counter++;
    }
}

void bullet_delete(BULLET *bullet){
    if(bullet){
        bullet->alive = 0;
        bullet->ttl = 0;
        bullet->x = 0;
        bullet->y = 0;
    }
}

void bullet_update(BULLET bullets[MAX_BULLETS]){
    for(int i = 0; i < MAX_BULLETS; i++){
        
        if(bullets[i].y <  -10 || bullets[i].y > (SCREEN_H + 10) ||  bullets[i].x < 0 ||  bullets[i].x > (SCREEN_W + 10) ){ // OUT OF THE SCREEN
            //LOGW("[%d] bullet is dead",i)
            bullet_delete(&bullets[i]);
        }
        
        if(bullets[i].alive){
            bullets[i].x += bullets[i].vel_x;
            bullets[i].y += -bullets[i].vel_y;
            bullets[i].ttl--;
        }
        
    }
}

void enemy_bullet_update(BULLET bullets[MAX_BULLETS], PLAYER *p){
    for(int i = 0; i < MAX_BULLETS; i++){
        
        if(bullets[i].y <  -10 || bullets[i].y > (SCREEN_H + 10) ||  bullets[i].x < 0 ||  bullets[i].x > (SCREEN_W + 10) ){ // OUT OF THE SCREEN
            //LOGW("[%d] bullet is dead",i)
            bullet_delete(&bullets[i]);
        }
        
        if(bullets[i].alive){
            
            float dx =  player.x  - bullets[i].x;
            float dy =  player.y  - bullets[i].y;
            float h = sqrt((dx*dx)+(dy*dy));
            
            dx /= h;
            // dy /= h;
        
            bullets[i].x += dx;
            bullets[i].y += bullets[i].vel_y * 0.5;
            
            if(collision_rect(player.x, player.y, TILE_SIZE, TILE_SIZE, bullets[i].x, bullets[i].y, 10,10)){
                if(player.health != 0){
                    player.health--;
                }
                
                if(!player.health){
                    game_state = GAMESTATE_GAMEOVER;
                }
                
            }
        }
        
    }    
}


void draw_enemies_bullets(BULLET bullets[MAX_BULLETS]){
     
    for(int i = 0; i < MAX_BULLETS; i++){
         BULLET *b = & bullets[i];
         if(b->alive){
            al_draw_bitmap(enemy_shot, b->x, b->y,0);
         }
     }    
}



void draw_bullets(BULLET bullets[MAX_BULLETS]){
     
     for(int i = 0; i < MAX_BULLETS; i++){
         BULLET *b = & bullets[i];
         if(b->alive){
            al_draw_bitmap(player_shot, b->x, b->y,0);
         }
     }
}


void collision_bullet_to_enemies(BULLET bullets[MAX_BULLETS], ENEMY enemies[MAX_ENEMIES_COUNT]){
    for(int i = 0; i < MAX_ENEMIES_COUNT;i++){
        for(int j = 0; j < MAX_BULLETS;j++){
            
            BULLET *b = &bullets[j];
            ENEMY *e = &enemies[i];
            
            
            if(collision_rect(b->x,b->y,10,10, e->x,e->y, TILE_SIZE, TILE_SIZE) && b->alive && e->alive){
                if(e->hitpoints > 1 ){
                    e->hitpoints--;
                    b->alive = 0;
                    LOGW("hitpoint : %d", e->hitpoints);
                    return;
                }else {
                  
                    e->alive = 0;
                    b->alive = 0;
                    
                    if(e-> type == 1 ){
                        player.score += 2 * 100;
                    }else {
                        player.score += 1 * 100;
                    }
                    
                      
                    if(enemies_count_alive() == 0 ){
                        game_state = GAMESTATE_GAMEOVER;
                    }
                    
                    LOGW("Enemy Count: %d", enemies_count_alive());
                }
            }
            
        }
        
    }
}

void player_input_keyboard(ALLEGRO_EVENT *ev){
    
    
        if(ev->type == ALLEGRO_EVENT_KEY_DOWN){
            if(ev->keyboard.keycode == ALLEGRO_KEY_W){
                keys[KEY_UP] = 1;
            }
             if(ev->keyboard.keycode == ALLEGRO_KEY_S){
                keys[KEY_DOWN] = 1;
            }
            
            if(ev->keyboard.keycode == ALLEGRO_KEY_A){
                keys[KEY_LEFT] = 1;
            }
            
            if(ev->keyboard.keycode == ALLEGRO_KEY_D){
                keys[KEY_RIGHT] = 1;
            }
            
            if(ev->keyboard.keycode == ALLEGRO_KEY_SPACE || ev->keyboard.keycode == ALLEGRO_KEY_RCTRL){
                keys[KEY_FIRE] = 1; 
            }
        }
        
        if(ev->type == ALLEGRO_EVENT_KEY_UP){
            if(ev->keyboard.keycode == ALLEGRO_KEY_W){
                keys[KEY_UP] = 0;
            }
             if(ev->keyboard.keycode == ALLEGRO_KEY_S){
                keys[KEY_DOWN] = 0;
            }
            
            if(ev->keyboard.keycode == ALLEGRO_KEY_A){
                keys[KEY_LEFT] = 0;
            }
            
            if(ev->keyboard.keycode == ALLEGRO_KEY_D){
                keys[KEY_RIGHT] = 0;
            }
            
            if(ev->keyboard.keycode == ALLEGRO_KEY_SPACE||ev->keyboard.keycode == ALLEGRO_KEY_RCTRL){
                keys[KEY_FIRE] = 0; 
            }
            
            if(ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                game_state = GAMESTATE_TITLE;
            }
        }
}


void remove_enemy(ENEMY *enemy){
    enemy->alive = 0;
    enemy->direction = 0;
    enemy_count--;
}

void update_enemies(){
    
    for(int i = MAX_ENEMIES_COUNT; i >= 0;i--){
        ENEMY *en = &s_enemy_list[i];
        
        if(en->x < 0 || en->x > SCREEN_W - TILE_SIZE){
            en->direction = -en->direction;
            en->y += 16;
        }
        
        en->x += en->vel_x *  en->direction;
    }
    

    if( (al_get_timer_count(timer) / 5) - enemy_shot_time > 1 ){
        enemy_shot_time = (al_get_timer_count(timer) / 5) + 1;
        
        int selected = rand() % MAX_ENEMIES_COUNT - 1;
        ENEMY* en = &s_enemy_list[selected];
        
        if(en->alive){
            enemy_shoot_bullets(en, enemies_bullets);
            LOGW("Enemy Shot!");
        }
        

    } 
    
}


void start_new_game(){
    init_enemies();
    init_player();
    bullet_clear(player_bullets);
    bullet_clear(enemies_bullets);
}


void update(){
    
    if(keys[KEY_LEFT]){
        player.x -= player.vel_x * 2.5;
    }
    
    if(keys[KEY_RIGHT]){
        player.x += player.vel_x * 2.5;
    }
    
    if(keys[KEY_FIRE] && (al_get_timer_count(timer) / 10) > shot_time ){
        LOGW("FIRE  - %d", (al_get_timer_count(timer) / 60));
        shot_time = (al_get_timer_count(timer) / 10) + 1;
    
        bullet_fire(player_bullets, player.x, SCREEN_H - 140);
    
    }
    
    
    update_player_pos();
    bullet_update(player_bullets);
    enemy_bullet_update(enemies_bullets, &player);
    collision_bullet_to_enemies(player_bullets, s_enemy_list);
    update_enemies();
}

int enemies_count_alive(){
    int alive_count = 0;
    
    for(int i = MAX_ENEMIES_COUNT; i >= 0; i--){
        ENEMY *e = &s_enemy_list[i];
        if(e->alive) alive_count++;
    }
    
    return alive_count;
    
}



void menu_update(ALLEGRO_EVENT *ev){
    return;
}

void menu_draw(){
    
    al_draw_bitmap(title_intro, 0,0,0);
}

void menu_input_update(ALLEGRO_EVENT *ev){
    
    if(ev->type == ALLEGRO_EVENT_KEY_UP){
        if(ev->keyboard.keycode == ALLEGRO_KEY_SPACE){
            start_new_game();
            game_state = GAMESTATE_PLAY;
        }
        
        if(ev->keyboard.keycode == ALLEGRO_KEY_ESCAPE){
            game_state = GAMESTATE_CLOSE;
        }
    }
    
     
}



void gameover_update(ALLEGRO_EVENT *e){

}

void gameover_update_input(ALLEGRO_EVENT *e){
    if(e->type == ALLEGRO_EVENT_KEY_UP){
        if(e->keyboard.keycode == ALLEGRO_KEY_SPACE){
            game_state = GAMESTATE_TITLE;
        }
       
        if(e->keyboard.keycode == ALLEGRO_KEY_ESCAPE){
             game_state = GAMESTATE_TITLE;
        }
    }
}

void gameover_draw(){
    
    al_draw_textf(text_font,al_map_rgb(255,255,255), SCREEN_W / 2, SCREEN_H / 2, 0, "GAME OVER!");
    al_draw_textf(text_font,al_map_rgb(255,255,255), SCREEN_W / 2, (SCREEN_H / 2) + 10, 0, "Score: %d", player.score);
    al_draw_textf(text_font,al_map_rgb(255,255,255), SCREEN_W / 2, (SCREEN_H / 2) + 30, 0, "Press SPACE or ESC to Restart!");
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    if(!init_allegro()) {
        LOGW("Allegro Failed!");
    }

    LOGW("Allegro Loaded with Success!");
    create_window(SCREEN_W, SCREEN_H, 0);
    
    load_sprites();
    init_player();
    init_enemies();

    int is_closed = 0;
    int redraw = 0;

    while(!is_closed) {

        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

      

        if(ev.type == ALLEGRO_EVENT_TIMER) {
            switch(game_state){
                case GAMESTATE_PLAY:
                    update();
                break;
                
                case GAMESTATE_TITLE:
                    menu_update(&ev);
                break;
                
                case GAMESTATE_GAMEOVER:
                    gameover_update(&ev);
                break;
                
                case GAMESTATE_CLOSE:
                    is_closed = 1;
                break;
            }
            redraw = 1;
        }
        
        
        switch(game_state){
                
                case GAMESTATE_TITLE:
                    menu_input_update(&ev);
                break;
                
                case GAMESTATE_PLAY:
                    player_input_keyboard(&ev);
                break;
                
                case GAMESTATE_GAMEOVER:
                    gameover_update_input(&ev);
                break;
        }

        if(redraw && al_event_queue_is_empty(queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            switch(game_state){
                case GAMESTATE_TITLE:
                    menu_draw();
                break;
                
                case GAMESTATE_PLAY:
                    draw_background();
                    draw_enemies();
                    draw_player();
                    draw_bullets(player_bullets);
                    draw_enemies_bullets(enemies_bullets);
                    
                    al_draw_textf(text_font, al_map_rgb(255,255,255),0,0,0, "%d", player.score);
                break;
                
                case GAMESTATE_GAMEOVER:
                    gameover_draw();
                break;
            }
            
            al_flip_display();
            redraw = 0;
        }
    }
    
    destroy_window();

    return 0;
}
