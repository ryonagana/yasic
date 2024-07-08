#include <stdio.h>
#include "resources.h"
#include "display.h"
#include "texture.h"
#include "platform.h"
#include "utils.h"

#include <sys/stat.h>
#include <sys/types.h>


static SDL_Texture *g_sprite[RESOURCES_MAX];
static int s_sprites_loaded = 0;


typedef struct FILERESOURCE {
    char filepath[255];
    int  has_colormask;
    int  color;
    int  end;
}FILERESOURCE;

FILERESOURCE resource[RESOURCES_MAX] = {

    {
        .filepath = "pshot.bmp\0",
        .has_colormask = TRUE,
        .color = 0xff00ff,
        .end = FALSE
    },

    {
        .filepath = "enemy01.bmp\0",
        .has_colormask = TRUE,
        .color = 0xff00ff,
        .end = FALSE
    },

    {
        .filepath = "enemy02.bmp\0",
        .has_colormask = TRUE,
        .color = 0xff00ff,
        .end = FALSE
    },

    {
        .filepath = "cannon.bmp\0",
        .has_colormask = TRUE,
        .color = 0xff00ff,
        .end = FALSE
    },

    {
        .end = TRUE
    }
};


const char s_assets_folder[] = "assets";


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
             if(resource[i].end) break;

             char buffer[2048];
             snprintf(buffer, sizeof(buffer), "%s//%s", s_assets_folder, resource[i].filepath);

             SDL_Surface *surf =  IMG_Load(buffer);


            if(surf == NULL){
                fprintf(stdout, "%s failed to load\n", buffer);
                return;
            }

            if(resource[i].has_colormask){
                g_sprite[i] = r_create_texture_colormask(surf, resource[i].color);
            }

            s_sprites_loaded++;
            SDL_FreeSurface(surf);

    }

    return;

}

 SDL_Texture *resources_get_sprite(int index){
    return g_sprite[index];
 }
