#ifndef RESOURCES_H
#define RESOURCES_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define RESOURCES_MAX 255

#define SPRITE_SHOOT_SPR   0
#define SPRITE_ENEMY01_SPR 1
#define SPRITE_ENEMY02_SPR 2
#define SPRITE_CANNON      3


void resources_init(void);
SDL_Texture *resources_get_sprite(int index);
#endif // RESOURCES_H
