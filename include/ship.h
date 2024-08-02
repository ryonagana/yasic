#ifndef SHIP_HEADER
#define SHIP_HEADER
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "bullet.h"

#define SHIP_BULLET_MAX 30



struct spaceship {
        SDL_FRect pos;
        SDL_FRect vel;
        int shoot_timer;
        int alive;
        struct bullet_t bullets[SHIP_BULLET_MAX];

        struct {
            int up;
            int down;
            int left;
            int right;
            int shoot;
        } control;

        //struct keys keys;
};

void ship_init(void);
void ship_unload(void);
void ship_update(double deltaTime);
void ship_draw(void);
struct spaceship *ship_get(void);
#endif // SHIP_HEADER
