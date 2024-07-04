#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>

SDL_Texture *r_create_texture(SDL_Surface *surf);
SDL_Texture *r_create_texture_colormask(SDL_Surface *surf, Uint32 color);

SDL_Texture *r_create_texture_from_path(const char *texturepath);
SDL_Texture *r_create_texture_from_path_colormask(const char *texturepath, Uint32 color);

#endif // RENDER_H

