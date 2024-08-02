#include "ship.h"

#include "video.h"
#include "convert.h"
#include <assert.h>
#include "utils.h"


struct spaceship ship;

SDL_Texture *ship_texture = NULL;

void ship_init(void){
    SDL_Surface *ship_surf = IMG_Load("assets//cannon.bmp");
    assert(ship_surf != NULL);
    create_colorkey(ship_surf, 0xFF00FF);
    ship_texture = convert_surface_to_texture(ship_surf);
    SDL_FreeSurface(ship_surf);

    int dsp_w, dsp_h;
    video_display_size(&dsp_w, &dsp_h);

    ship.pos.x = dsp_w / 2;
    ship.pos.y = dsp_h - 64;
    ship.alive = TRUE;

    memset(&ship.keys,0, sizeof(ship.keys));

}

void ship_unload(void){
    SDL_DestroyTexture(ship_texture);
}

void ship_update(double deltaTime){

    if(ship.alive){

        if(ship.keys.left){
            ship.vel.x = 0.2 * deltaTime;
        }

        if(ship.keys.right){
            ship.vel.x = -0.2 * deltaTime;
        }


        ship.pos.x += ship.vel.x;
        ship.pos.y += ship.vel.y;


        if(KeyPressed(SDL_SCANCODE_D) && !ship.keys.left ){
            ship.keys.left = 1;
        }else {
            ship.keys.left = 0;
         }

        if(KeyPressed(SDL_SCANCODE_A) && !ship.keys.right){
            ship.keys.right = 1;
        }else {
            ship.keys.right = 0;
        }
    }

}
void ship_draw(void){

    SDL_FRect dest = {
        .x = ship.pos.x,
        .y = ship.pos.y,
        .w = 32,
        .h = 32,
    };

    SDL_RenderCopyExF(gamewindow.renderer, ship_texture, NULL,  &dest, 0.0, NULL, SDL_FLIP_NONE);
}

void ship_move_left(void){
    ship.keys.left = 1;
}

void ship_move_right(void){
    ship.keys.right  = 1;
}

struct spaceship *ship_get(void){
    return &ship;
}
