#ifndef CONVERT_H
#define CONVERT_H
#include <SDL2/SDL.h>


SDL_Texture*  load_texture_from_filepath(const char *path, Uint32 *color);
SDL_Texture*  convert_surface_to_texture(SDL_Surface *surf);
void          create_colorkey(SDL_Surface *surf, Uint32 color);

#endif // CONVERT_H
