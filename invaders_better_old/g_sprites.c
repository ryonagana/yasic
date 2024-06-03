#include <stdio.h>
#include "g_sprites.h"
#include "g_render.h"

ALLEGRO_BITMAP *sprites[SPRITE_MAX];

const char sprite_assets_path[SPRITE_MAX][255] = {
    {"assets//bg01.bmp"},
    {"assets//cannon.bmp"},
    {"assets//enemy01.bmp"},
    {"assets//enemy02.bmp"},
    {"assets//eshot.bmp"},
    {"assets//pshot.bmp"},
    {"assets//title.png"},
    {"assets//dshot.png"}
};


static ALLEGRO_BITMAP *al_load_bitmap_safe(const char *path){
    ALLEGRO_BITMAP *b = NULL;
    b = al_load_bitmap(path);

    if(!b){
        b = al_create_bitmap(32,32);
        al_set_target_bitmap(b);
        al_clear_to_color(al_map_rgb_f(255,0,0));
        al_set_target_backbuffer(display);
        return b;
    }

    return b;
}


void init_sprites(void){

    for(int i = 0; i < SPRITE_MAX; i++){
        sprites[i] = al_load_bitmap_safe(sprite_assets_path[i]);
        ALLEGRO_PATH *asset_path = al_create_path(sprite_assets_path[i]);


        if(strcmp(".bmp", al_get_path_extension(asset_path)) == 0){
            al_convert_mask_to_alpha(sprites[i], al_map_rgb(255,0,255));
        }


        if(!sprites[i]){
            fprintf(stderr, "Error Loading: %s", sprite_assets_path[i]);
        }

        al_destroy_path(asset_path);
        asset_path = NULL;
    }
}
