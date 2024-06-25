#ifndef RESOURCES_H
#define RESOURCES_H
#include <allegro5/allegro.h>

#define RESOURCES_MAX 255

#define SPRITE_SHOOT_SPR   0
#define SPRITE_ENEMY01_SPR 1


extern ALLEGRO_BITMAP *g_sprite[RESOURCES_MAX];
void resources_init(void);
#endif // RESOURCES_H
