#ifndef BULLET_H
#define BULLET_H
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define MAX_BULLETS 150
#define BULLET_RICOCHET_SIDE    (1<<0)
#define BULLET_RICOCHET_TOP     (1<<1)
#define BULLET_RICOCHET_BOTTOM  (1<<2)
#define BULLET_GRAVITY_AFFECTED (1<<3)

typedef struct BULLET {
    float x;
    float y;
    float vx;
    float vy;
    int alive;
    int flags;
    float gravity;
}BULLET;

typedef struct TBULLETS {
    BULLET bullets[MAX_BULLETS];
    int length;
    int reset;
    void (*update_callback)(struct BULLET *b, int bullet_length);
    void (*render_callback)(struct BULLET *b, int bullet_length);
}TBULLETS;

void Bullet_Init(TBULLETS *bullet_root, int size);
void Bullet_Update(TBULLETS *bullet_root);
void Bullet_Draw(TBULLETS *bullet_root, ALLEGRO_BITMAP *bmp);

BULLET* Bullet_FindFree(TBULLETS *bullet_root);

void Bullet_SetFlag(BULLET *b, int flags);
int Bullet_GetFlag(BULLET *b);

void Bullet_SetGravity(BULLET *b, float gravity);

int  Bullet_Collides_Top(BULLET *b);
int  Bullet_Collides_Bottom(BULLET *b);
int  Bullet_Collides_Left(BULLET *b);
int  Bullet_Collides_Right(BULLET *b);

#endif // BULLET_H
