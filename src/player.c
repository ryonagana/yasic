#include "player.h"
#include "display.h"
#include "level.h"
#include "resources.h"
#include <SDL2/SDL.h>
#include "utils.h"



typedef struct PlayerKeyEvent {
    SDL_Keysym left;
    SDL_Keysym right;
    SDL_Keysym shoot;

    int isleftPressed;
    int isRightPressed;
    int isShootPressed;
}PlayerKeyEvent;

PlayerKeyEvent s_player_key_events;



SDL_Texture *bullet01_spr = NULL;

static void S_player_default_shot_update(struct BULLET *b, int bullet_length){
    b->vy = -1 * 3.0;
    b->vx = 1;

    b->x += b->vx;
    b->y += b->vy;
}

static void S_player_default_shot_render(struct BULLET *b, int bullet_length, SDL_Texture *bmp){
    //al_draw_filled_circle(b->x, b->y, 10, al_map_rgb(0,0,255));
    //al_draw_bitmap(bmp, b->x, b->y, 0);
    SDL_FRect r = {b->x, b->y, 16,16};
    SDL_RenderCopyExF(g_display.renderer, bmp, NULL, &r, 0.0, NULL, SDL_FLIP_NONE);
}

void Player_Init(PLAYER *p){

    p->shoot_timer = 50;
    p->shoot_count = 0;
    p->direction = 1;
    p->accel_timer = 70;
    p->accel_index = 0;
    p->state = 0;


    Bullet_Init(&p->bullets, 35);
    Bullet_SetDrawCallback(&p->bullets, S_player_default_shot_render);
    Bullet_SetUpdateCallback(&p->bullets, S_player_default_shot_update);

    memset(&s_player_key_events, 0x0, sizeof(s_player_key_events));
    s_player_key_events.left.sym = SDLK_LEFT;
    s_player_key_events.right.sym = SDLK_RIGHT;
    s_player_key_events.shoot.sym = SDLK_SPACE;

    bullet01_spr = resources_get_sprite(SPRITE_SHOOT_SPR);


}
void Player_SpawnPos(PLAYER *p, int x, int y){
    p->pos.x = x;
    p->pos.y = y;
}

void Player_SetShootTime(PLAYER *p, int timer){
    p->shoot_timer = timer;
}


void Player_SetShootCount(PLAYER *p, int count){
    p->shoot_timer = count;
}

void Player_ReleasedLeft(PLAYER *p){
    s_player_key_events.isleftPressed = FALSE;

}
void Player_ReleasedRight(PLAYER *p){
    s_player_key_events.isRightPressed = FALSE;
}

void Player_MoveLeft(PLAYER *p){
    s_player_key_events.isleftPressed = TRUE;

}
void Player_MoveRight(PLAYER *p){
    s_player_key_events.isRightPressed = TRUE;
}


void Player_Shoot(PLAYER *p){
    if(p->shoot_count == 0){
        p->shoot_count = p->shoot_timer;
        printf("SHOOT");
    }
}




void Player_Update(PLAYER *p, float deltaTime){

    SDL_Log("w:%.2f\nh: %.2f", Dsp_GetWindowWidth() - p->pos.x, Dsp_GetWindowHeight() - p->pos.h);


    if(s_player_key_events.isleftPressed && ((Dsp_GetWindowWidth() - 1) - p->pos.x) <= Dsp_GetWindowWidth()
    ){
            p->direction = -1;
    }else if(s_player_key_events.isRightPressed && ((Dsp_GetWindowWidth() - 1) - p->pos.x) >= 32){
            p->direction = 1;
    }else {
        p->direction = 0;
    }



    p->vel.x = p->direction * 2 * (1 * deltaTime);

    p->pos.x += p->vel.x;
    p->pos.y += p->vel.y;


    /*
    if(p->shoot_count  == 0){

        BULLET* b = Bullet_FindFree(&p->bullets);

        b->x = p->pos.x;
        b->y = p->pos.y;
        b->vy = -1;
        b->vy = 1;

    }else {
        if(p->shoot_count > 0){
            p->shoot_count--;
            SDL_Log("Shoot Counter: %d", p->shoot_count);
        }else {
              p->shoot_count = p->shoot_timer;
        }
    }
    */


    Bullet_Update(&p->bullets);


    return;
}

void Player_Render(PLAYER *p){

    SDL_Texture  *player_spr = resources_get_sprite(SPRITE_CANNON);
    SDL_FRect r = {p->pos.x, p->pos.y, 32,32};

    SDL_RenderCopyExF(g_display.renderer, player_spr, NULL, &r, 0.0, NULL, SDL_FLIP_NONE);

    //al_draw_filled_rectangle(p->x, p->y, p->x + 32, p->y + 32, al_map_rgb(0,0,255));
    Bullet_Draw(&p->bullets, bullet01_spr);
}
