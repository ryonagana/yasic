#include "ship.h"

#include "video.h"
#include "convert.h"
#include <assert.h>
#include "utils.h"


struct spaceship ship;
SDL_Texture *ship_texture = NULL;
SDL_Texture *bullet_texture = NULL;


void ship_init(void){
     SDL_Surface *ship_surf = IMG_Load("assets//cannon.bmp");
     create_colorkey(ship_surf, SDL_MapRGBA(ship_surf->format, 255,0,255,255));
     ship_texture = convert_surface_to_texture(ship_surf); //load_texture_from_filepath("assets//cannon.bmp", NULL);
     SDL_FreeSurface(ship_surf);


     SDL_Surface *bullet_surf = IMG_Load("assets//pshot.bmp");
     create_colorkey(bullet_surf, SDL_MapRGBA(bullet_surf->format, 255,0,255,255));
     bullet_texture = convert_surface_to_texture(bullet_surf);

     int dsp_w, dsp_h;
     video_display_size(&dsp_w, &dsp_h);

     ship.pos.x = dsp_w / 2;
     ship.pos.y = dsp_h - 32;
     ship.alive = TRUE;


     bullet_init(ship.bullets, SHIP_BULLET_MAX, 0);

    /*
    SDL_Surface *ship_surf = IMG_Load("assets//cannon.bmp");
    assert(ship_surf != NULL);
    ship_surf = create_colorkey(ship_surf, SDL_MapRGBA(ship_surf->format, 255,0,255,255));
    ship_texture = convert_surface_to_texture(ship_surf);
    SDL_FreeSurface(ship_surf);

    int dsp_w, dsp_h;
    video_display_size(&dsp_w, &dsp_h);

    ship.pos.x = dsp_w / 2;
    ship.pos.y = dsp_h - 32;
    ship.alive = TRUE;

    bullet_texture = load_texture_from_filepath("assets//pshot.bmp");
    bullet_init(ship.bullets, SHIP_BULLET_MAX, 0);

    memset(&ship.control,0, sizeof(ship.control));
    */

}

void S_ship_do_shoot(double delta){
    struct bullet_t *b = bullet_get_free(ship.bullets, SHIP_BULLET_MAX);

    if(NULL == b)
        return;

    b->alive = TRUE;
    b->pos.x = ship.pos.x + 16;
    b->pos.y = ship.pos.y + 16;
    b->vel.y = 1 * delta;
    b->vel.x = 0;
}

void ship_unload(void){
    SDL_DestroyTexture(ship_texture);
    SDL_DestroyTexture(bullet_texture);
}

void ship_update(double deltaTime){

    if(ship.alive){

            if(ship.control.left && ship.pos.x >= 0 ){
                ship.vel.x = -.1f * deltaTime;
            }else if(ship.control.right && ship.pos.x <= (video_width() / VIDEO_SCALE)-32){
                ship.vel.x = .1f * deltaTime;
            } else {
                ship.vel.x = 0;
            }


        if(ship.control.shoot && ship.shoot_timer == 0){
            ship.shoot_timer = 30;
            S_ship_do_shoot(deltaTime);
        }


        ship.pos.x += ship.vel.x;
        ship.pos.y += ship.vel.y;

        bullet_update(ship.bullets, SHIP_BULLET_MAX, deltaTime);

        if(ship.shoot_timer > 0){
            ship.shoot_timer--;
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

    bullet_draw(ship.bullets, SHIP_BULLET_MAX, bullet_texture);
    SDL_RenderCopyExF(gamewindow.renderer, ship_texture, NULL,  &dest, 0.0, NULL, SDL_FLIP_NONE);


}


struct spaceship *ship_get(void){
    return &ship;
}
