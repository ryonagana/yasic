#include <stdio.h>
#include "resources.h"
#include "display.h"
#include "texture.h"

 SDL_Texture *g_sprite[RESOURCES_MAX];
 int s_sprites_loaded = 0;

const char *s_assets_folder = "assets";
const char s_sprites_filepath[RESOURCES_MAX][255] = {
    {"pshot.bmp\0"},
    {"enemy01.bmp\0"},
    {"enemy02.bmp\0"},
    {0}

};


#include <sys/stat.h>
#include <sys/types.h>


static int s_resource_exists(const char *filepath, uint64_t *size){

    struct stat s;

    if(stat(filepath, &s) != 0){
        if(size != NULL){
            *size = s.st_size;
            return 1;
        }
    }

    return 0;
}

void resources_init(void){

    for(int i = 0; i < RESOURCES_MAX; i++){

            if(s_sprites_filepath[i][0] == '\0') return;
            char buf[255];
            snprintf(buf, 255, "%s//%s", s_assets_folder, s_sprites_filepath[i]);


            SDL_Surface *surf = SDL_LoadBMP(buf);


            if(surf == NULL){
                fprintf(stdout, "%s failed to load\n", buf);
                return;
            }

            g_sprite[i] = r_create_texture_colormask(surf, 0xff00ff); //r_create_texture(surf);  //SDL_CreateTextureFromSurface(g_display.renderer, surf);
            s_sprites_loaded++;
            SDL_FreeSurface(surf);
    }

    return;

}

 SDL_Texture *resources_get_sprite(int index){
    return g_sprite[index];
 }
