#include "convert.h"
#include <SDL2/SDL_image.h>
#include "video.h"

SDL_Texture*  load_texture_from_filepath(const char *path){
    SDL_Surface *sf;
    SDL_Texture *tex;

    if(strlen(path) <= 0 || NULL == path ){
        fprintf(stderr, "Error path is Invalid!");
        return NULL;
    }

    sf = IMG_Load(path);

    if(NULL == sf){
        fprintf(stderr, "asset %s invalid ", path);
        return NULL;
    }

    tex = SDL_CreateTextureFromSurface(gamewindow.renderer, sf);

    if(NULL == tex){
        fprintf(stderr, "texture %s invalid ", path);
        SDL_FreeSurface(sf);
        return NULL;
    }

    SDL_FreeSurface(sf);
    return tex;


}
SDL_Texture*  convert_surface_to_texture(SDL_Surface *surf){
    SDL_Texture *tex;

    if(NULL == surf){
        return NULL;
    }

    tex = SDL_CreateTextureFromSurface(gamewindow.renderer, surf);
    return tex;
}

void create_colorkey(SDL_Surface *surf, Uint32 color){
    SDL_SetColorKey(surf, SDL_TRUE, color);
}
