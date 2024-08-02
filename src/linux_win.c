#include "linux_win.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "video.h"

#define _CRIT(...) fprintf(stderr, __VA_ARGS__)
#define _LOG(...)  fprintf(stdout, __VA_ARGS__)

struct _gamewindow gamewindow;

void window_init(void){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        _CRIT("Error Trying to Initialize SDL2\n");
        SDL_Quit();
        return;
    }

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;

    if( (IMG_Init(flags) & flags) !=  flags){
            _CRIT("Error Trying to Init SDL_Image Extension");
            SDL_Quit();
            return;
    }

}
void window_display_create(int width, int height, int is_fullscreen, int enable_vsync, const char *window_caption){

    int window_flags = SDL_WINDOW_SHOWN;

    if(is_fullscreen) window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS;

    #define PTR_INVALID(x) (!x || NULL == x)

    gamewindow.window = SDL_CreateWindow(window_caption, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);

    if(PTR_INVALID(gamewindow.window)){
        _CRIT("Invalid Window creation!");
        SDL_Quit();
        return;
    }

    int renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;

    if(enable_vsync){
        renderer_flags |= SDL_RENDERER_PRESENTVSYNC;
    }

    gamewindow.renderer = SDL_CreateRenderer(gamewindow.window, -1, renderer_flags);

    if(PTR_INVALID(gamewindow.renderer)){
        _CRIT("Invalid Renderer creation!");
        SDL_DestroyWindow(gamewindow.window);
        SDL_Quit();
        return;
    }

    gamewindow.screen = SDL_CreateTexture(gamewindow.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width/VIDEO_SCALE, height/VIDEO_SCALE);

    if(PTR_INVALID(gamewindow.renderer)){
        _CRIT("Render Buffer Failed!");
        SDL_DestroyRenderer(gamewindow.renderer);
        SDL_DestroyWindow(gamewindow.window);
        SDL_Quit();
    }

    #undef PTR_INVALID

}
void window_unload(void){

}
