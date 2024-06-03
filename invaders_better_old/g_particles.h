#ifndef GAME_PARTICLES_HEADER
#define GAME_PARTICLES_HEADER

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#define DEFAULT_PARTICLES 2000
#define MAX_PARTICLES     10000


typedef struct PARTICLE {
    int x,y;
    double vx,vy;
    int ttl;
    ALLEGRO_COLOR color;
}PARTICLE;



void particle_create(PARTICLE *p, int x, int y, double vx,  double vy, ALLEGRO_COLOR color, int life);
void particle_update(PARTICLE *p);
void particle_reset(PARTICLE *p, int max);
PARTICLE *particle_free(PARTICLE *p, int max);
void particle_explosion(PARTICLE *plist, int x, int y, int spread, int particle_count, int life, ALLEGRO_COLOR color);
void particle_clear_buffer(PARTICLE *plist, ALLEGRO_BITMAP *bmp, ALLEGRO_COLOR color);
void particle_draw(ALLEGRO_BITMAP *bmp, PARTICLE *p, int ox, int oy, const ALLEGRO_COLOR color);

#endif // GAME_PARTICLES_HEADER
