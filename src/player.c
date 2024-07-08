#include "player.h"
#include "display.h"
#include "level.h"
#include "resources.h"
#include <SDL2/SDL.h>





PlayerKeyEvent s_player_key_events;



static void S_player_default_shot_update(struct BULLET *b, int bullet_length){
    b->vy = -1 * 3.0;
    b->vx = 1;

    b->x += b->vx;
    b->y += b->vy;
}

static void S_player_default_shot_render(struct BULLET *b, int bullet_length, SDL_Texture *bmp){
    //al_draw_filled_circle(b->x, b->y, 10, al_map_rgb(0,0,255));
    //al_draw_bitmap(bmp, b->x, b->y, 0);
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

void Player_MoveLeft(PLAYER *p){
  p->direction = -1;

}
void Player_MoveRight(PLAYER *p){
    p->direction = 1;
}


void Player_Shoot(PLAYER *p){
    if(p->shoot_count == 0){
        p->shoot_count = p->shoot_timer;
        printf("SHOOT");
    }
}

void Player_Update(PLAYER *p, float deltaTime){



    p->vel.x = p->direction * deltaTime;

    p->pos.x += p->vel.x;
    p->pos.y += p->vel.y;
    p->direction = 0;
    if(p->shoot_count  == 0){

        BULLET* b = Bullet_FindFree(&p->bullets);

        b->x = p->pos.x;
        b->y = p->pos.y;
        b->vy = -1;
        b->vy = 1;
        p->shoot_count = p->shoot_timer;
    }

    if(p->shoot_count > 0){
        p->shoot_count--;
    }

    Bullet_Update(&p->bullets);
    return;
}

void Player_Render(PLAYER *p){

    SDL_Texture *bullet_spr = resources_get_sprite(SPRITE_SHOOT_SPR);
    SDL_Texture  *player_spr = resources_get_sprite(SPRITE_CANNON);

    SDL_FRect r = {p->pos.x, p->pos.y, 32,32};

    SDL_RenderCopyExF(g_display.renderer, player_spr, NULL, &r, 0.0, NULL, SDL_FLIP_NONE);

    //al_draw_filled_rectangle(p->x, p->y, p->x + 32, p->y + 32, al_map_rgb(0,0,255));
    Bullet_Draw(&p->bullets, bullet_spr);
}
