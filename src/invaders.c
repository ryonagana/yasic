#include "win.h"
#include <SDL2/SDL.h>
#include "video.h"
#include "utils.h"
#include "ship.h"
#include "enemy.h"


int keys[255];

struct enemy_data enemies;

SDL_Texture *enemy_spr;

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
    window_display_create(800,600,FALSE,TRUE, "INVADERS!");

    int close = 0;
    int vsync_on = TRUE;

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double deltaTime = 0;
    char fps_txt[125];
    ship_init();

    enemy_init(&enemies, 8,8);
    SDL_Surface *enemy_spr_surf = NULL;
    enemy_spr_surf = IMG_Load("assets//enemy01.bmp");
    create_colorkey(enemy_spr_surf, SDL_MapRGB(enemy_spr_surf->format, 255,0,255));

    enemy_spr = convert_surface_to_texture(enemy_spr_surf); //load_texture_from_filepath("assets//enemy01.bmp", NULL);
    SDL_FreeSurface(enemy_spr_surf);

    memset(keys, 0x0, sizeof(keys));

    while(!close){

        SDL_Event e;
        struct spaceship *ship = ship_get();
        now = SDL_GetPerformanceCounter();
        deltaTime = (double) ((now-last)*1000) / (double) SDL_GetPerformanceFrequency();

        snprintf(fps_txt, sizeof(fps_txt), "%.2f", deltaTime);
        SDL_SetWindowTitle(gamewindow.window, fps_txt);


        while(SDL_PollEvent(&e) != 0){

            if(e.type == SDL_QUIT){
                close = 1;
                break;
            }

            if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_a:
                    ship->control.left = 1;
                    break;

                    case SDLK_d:
                        ship->control.right = 1;
                        break;
                    case SDLK_SPACE:
                        ship->control.shoot = 1;
                        break;
                }
            }

            if(e.type == SDL_KEYUP){
                switch(e.key.keysym.sym){
                    case SDLK_a:
                        ship->control.left = 0;
                        break;

                    case SDLK_d:
                        ship->control.right = 0;
                        break;

                    case SDLK_SPACE:
                        ship->control.shoot = 0;
                        break;

                }
            }
        }

        fprintf(stderr, "left %d\n", ship->control.left);
        ship_update(deltaTime);
        enemy_update(&enemies, deltaTime);

        SDL_SetRenderTarget(gamewindow.renderer, gamewindow.screen);
        SDL_RenderClear(gamewindow.renderer);
        ship_draw();
        enemy_draw(&enemies, enemy_spr);
        SDL_SetRenderTarget(gamewindow.renderer, NULL);

        video_render();
        last = now;




        if(!vsync_on){
            float elapsed = (last -now) / SDL_GetPerformanceFrequency() * 1000;
            SDL_Delay(floor(1.0/60.0) - elapsed);
        }
    }

    ship_unload();
    window_unload();

    return 0;
}
