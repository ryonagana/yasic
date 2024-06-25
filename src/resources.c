#include <stdio.h>
#include "resources.h"


ALLEGRO_BITMAP *g_sprite[RESOURCES_MAX];

const char *s_assets_folder = "assets";
const char s_sprites_filepath[RESOURCES_MAX][255] = {
    {"pshot.bmp\0"},
    {0}

};

void resources_init(void){

    for(int i = 0; i < RESOURCES_MAX;i++){
        if(s_sprites_filepath[i][0] == '\0') continue;
        char buf[255];

        snprintf(buf, 255, "%s//%s", s_assets_folder, s_sprites_filepath[i]);

        if(!al_filename_exists(buf)){
            continue;
        }

        ALLEGRO_BITMAP * bmp = al_load_bitmap(buf);

        if(NULL == bmp){
            fprintf(stdout, "%s failed to load\n", buf);
        }

        g_sprite[i] = bmp;

    }

}
