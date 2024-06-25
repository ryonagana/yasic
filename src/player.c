#include "player.h"
#include "display.h"
#include "level.h"
#include "resources.h"

static float s_speed_modifier[4] = {
    4.5f,
    5.0f,
    6.2f,
    6.9f
};
//void Bullet_SetUpdateCallback(TBULLETS *bullets, void (*update_callback)(struct BULLET *b, int bullet_length));
//void Bullet_SetDrawCallback(TBULLETS *bullets,   void (*render_callback)(struct BULLET *b, int bullet_length));
static void S_player_default_shot_update(struct BULLET *b, int bullet_length){
    b->vy = -1 * 3.0;
    b->vx = 1;

    b->x += b->vx;
    b->y += b->vy;
}

static void S_player_default_shot_render(struct BULLET *b, int bullet_length, ALLEGRO_BITMAP *bmp){
    //al_draw_filled_circle(b->x, b->y, 10, al_map_rgb(0,0,255));
    al_draw_bitmap(bmp, b->x, b->y, 0);
}

void Player_Init(PLAYER *p){
    p->x = 0;
    p->y = 0;
    p->shoot_timer = 50;
    p->vx = 1;
    p->vy = 0;
    p->shoot_count = 0;
    p->direction = 1;
    p->accel_timer = 70;
    p->accel_index = 0;
    p->state = 0;

    Bullet_Init(&p->bullets, 35);
    Bullet_SetDrawCallback(&p->bullets, S_player_default_shot_render);
    Bullet_SetUpdateCallback(&p->bullets, S_player_default_shot_update);

}
void Player_SpawnPos(PLAYER *p, int x, int y){
    p->x = x;
    p->y = y;
}

void Player_SetShootTime(PLAYER *p, int timer){
    p->shoot_timer = timer;
}


void Player_SetShootCount(PLAYER *p, int count){
    p->shoot_timer = count;
}

void Player_MoveLeft(PLAYER *p){
    if(p->state <= 0){
        p->state = 1;
    }
    p->direction = -1;
}
void Player_MoveRight(PLAYER *p){
    if(p->state <= 0){
        p->state = 1;
    }

    p->direction = 1;
}


void Player_Shoot(PLAYER *p){
    if(p->shoot_count == 0){
        p->shoot_count = p->shoot_timer;
        printf("SHOOT");
    }
}

void Player_Update(PLAYER *p){

    p->vx = p->direction * s_speed_modifier[0];
    //p->vy = p->direction * s_speed_modifier[0];

    p->x += p->vx;
    p->y += p->vy;
    p->direction = 0;

    if(p->shoot_count  == 0){

        BULLET* b = Bullet_FindFree(&p->bullets);

        b->x = p->x;
        b->y = p->y;
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

    al_draw_filled_rectangle(p->x, p->y, p->x + 32, p->y + 32, al_map_rgb(0,0,255));
    Bullet_Draw(&p->bullets, g_sprite[ SPRITE_SHOOT_SPR]);
}
