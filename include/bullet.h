#ifndef BULLET_HEADER
#define BULLET_HEADER
#include <SDL2/SDL.h>

#define BULLET_FLAG_GRAVITY 0x2

struct bullet_t {

    SDL_FRect pos;
    SDL_FRect vel;
    int alive;
    int flags;
    float angle;
    float gravity;

};

void bullet_init(struct bullet_t *bullet, int max, int flags);
void bullet_update(struct bullet_t *bullet, int max, double delta);
void bullet_draw(struct bullet_t *bullet, int max, SDL_Texture *tex);

struct bullet_t *bullet_get_free(struct bullet_t *bullet, int max);

#endif // BULLET_HEADER
