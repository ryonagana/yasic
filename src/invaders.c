#include "linux_win.h"
#include <SDL2/SDL.h>
#include "video.h"
#include "utils.h"
#include "ship.h"

int keys[255];

int KeyPressed(int key){
    return (keys[key] & KEY_PRESSED) == 0;
}

int KeyReleased(int key){
    return (keys[key] & KEY_RELEASED) == 0;
}

int main(int argc, char *argv[]){

    UNUSED(argc);
    UNUSED(argv);

    window_init();
    window_display_create(800,600,0,1, "INVADERS!");

    int close = 0;
    int vsync_on = TRUE;

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double deltaTime = 0;
    char fps_txt[125];
    ship_init();

    int first_frame = TRUE;

    memset(keys, 0x0, sizeof(keys));

    while(!close){

        SDL_Event e;
        struct spaceship *ship = ship_get();

        if(!first_frame){
            last = now;
            first_frame = TRUE;
        }

        now = SDL_GetPerformanceCounter();
        deltaTime = (double) ((now-last)*1000) / (double) SDL_GetPerformanceFrequency();

        snprintf(fps_txt, sizeof(fps_txt), "%.6f", deltaTime);
        SDL_SetWindowTitle(gamewindow.window, fps_txt);

        while(SDL_PollEvent(&e) != 0){

            if(e.type == SDL_QUIT){
                close = 1;
                break;
            }

            if(e.type == SDL_KEYDOWN){
                keys[e.key.keysym.scancode] |= KEY_PRESSED;
                keys[e.key.keysym.scancode] &= ~KEY_RELEASED;
            }

            if(e.type == SDL_KEYUP){
                keys[e.key.keysym.scancode] &= ~KEY_PRESSED;
                keys[e.key.keysym.scancode] |= KEY_RELEASED;
            }
        }

        fprintf(stderr, "left %d\n", ship->keys.left);
        ship_update(deltaTime);

        SDL_SetRenderTarget(gamewindow.renderer, gamewindow.screen);
        SDL_RenderClear(gamewindow.renderer);
        ship_draw();
        SDL_SetRenderTarget(gamewindow.renderer, NULL);

        video_render();

        memset(keys, 0x0, sizeof(keys));

        last = now;

        if(!vsync_on){
            float elapsed = (last -now) / SDL_GetPerformanceFrequency() * 1000;
            SDL_Delay(floor(1.0/60.0) - elapsed);
        }
    }

    return 0;
}
