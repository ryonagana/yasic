#include "texture.h"
#include "display.h"

SDL_Texture *r_create_texture(SDL_Surface *surf){
    if(surf){
        SDL_Texture * tex = SDL_CreateTextureFromSurface(g_display.renderer, surf);
        if(tex){
            return tex;
        }
    }

    return NULL;
}
SDL_Texture *r_create_texture_colormask(SDL_Surface *surf, Uint32 color){
    SDL_SetColorKey(surf, 1, color);
    SDL_Texture *tex = r_create_texture(surf);
    //SDL_SetTextureColorMod(tex, r,g,b);

    return tex;
}

SDL_Texture *r_create_texture_from_path(const char *texturepath){
    SDL_Surface *surf;
    SDL_Texture *tex;
    surf = IMG_Load(texturepath);

    if(!surf){
        return NULL;
    }

    tex = SDL_CreateTextureFromSurface(g_display.renderer, surf);

    if(!tex){
        return tex;
    }

    SDL_FreeSurface(surf);
    return tex;

}
SDL_Texture *r_create_texture_from_path_colormask(const char *texturepath, Uint32 color){
    SDL_Surface *surf;
    SDL_Texture *tex;

    surf = IMG_Load(texturepath);

    if(!surf){
        return NULL;
    }
    SDL_SetColorKey(surf, 1, 0xff00ff);
    tex = r_create_texture(surf);
    return tex;

    /*
    SDL_SetColorKey()
    SDL_Texture *texture = r_create_texture_from_path(texturepath);
    if(!texture){
        return NULL;
    }

    return texture;
    */
}
