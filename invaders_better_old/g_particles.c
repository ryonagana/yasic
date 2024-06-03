#include <stdio.h>
#include <math.h>
#include "g_particles.h"
#include "g_render.h"
#include "g_utils.h"


//static PARTICLE particles[MAX_PARTICLES];


void particle_create(PARTICLE *p, int x, int y, double vx,  double vy, ALLEGRO_COLOR color, int life){
        p->x = x;
        p->y = y;
        p->vx = vx;
        p->vy = vy;
        p->color = color;
        p->ttl = life;
}


void particle_draw(ALLEGRO_BITMAP *bmp, PARTICLE *p, int ox, int oy, const ALLEGRO_COLOR color){
    ALLEGRO_VERTEX vtx[DEFAULT_PARTICLES] = {0};

    if(bmp)
        al_set_target_bitmap(bmp);


    for(int i = 0; i < MAX_PARTICLES; i++){
        if(p[i].ttl){

            int x, y;



            x = p[i].x - ox;
            y = p[i].y - oy;

            if(x < 0 || x > al_get_display_width(display) + 2 ||  y < 2 || y > al_get_display_height(display) + 16) return;

            vtx[i].x = x;
            vtx[i].y = y;
            vtx[i].z = 0;
            vtx[i].u = 0;
            vtx[i].v = 0;
            vtx[i].color = color;

        }
    }
    al_draw_prim(vtx, NULL, NULL, 0, DEFAULT_PARTICLES, ALLEGRO_PRIM_POINT_LIST);

    if(bmp)
        al_set_target_backbuffer(display);


}

void particle_update(PARTICLE *p){
    p->x += cos(1 * DEG2RAD) *  p->vx;
    p->y += sin(-90 * DEG2RAD) *  p->vy;
    p->ttl--;
}

void particle_reset(PARTICLE *p, int max){
    for(int i = 0; i < max; i++){
        p[i].ttl = 0;
    }
}

void particle_clear_buffer(PARTICLE *plist, ALLEGRO_BITMAP *bmp,  ALLEGRO_COLOR color){
   UNUSED(color);
    for(int i = 0; i < DEFAULT_PARTICLES; i++){
        if(!plist[i].ttl){

            al_set_target_bitmap(bmp);
            al_lock_bitmap(bmp, al_get_bitmap_format(bmp), ALLEGRO_LOCK_WRITEONLY);
            //al_put_pixel(plist[i].x, plist[i].y, color);
            //al_draw_pixel(plist[i].x, plist[i].y, color);
            al_unlock_bitmap(bmp);
            al_set_target_backbuffer(display);

        }
    }

}

PARTICLE *particle_free(PARTICLE *p, int max){


   int i = 0;
   while(p[i].ttl && i != max) i++;
   if(i == max) return NULL;
   return &p[i];



}


void particle_explosion(PARTICLE *plist, int x, int y, int spread, int particle_count, int life, ALLEGRO_COLOR color){
    int i;
    PARTICLE *p = NULL;

    for(i = 0;i < particle_count;i++){
        p = particle_free(plist, MAX_PARTICLES);
        float angle, speed;

        if(p != NULL){

            speed = game_rand_range(1,3) + 0.5;
            angle = game_rand_range(0,360);

            particle_create(p, x + game_rand(spread) - spread / 2,
                               y + game_rand(spread) - spread / 2,
                               cos(angle * DEG2RAD) * speed,
                               sin(angle * DEG2RAD) * speed,
                                color,
                               (life ? life : 70 -  game_rand(50))
                            );
        }
    }

}
