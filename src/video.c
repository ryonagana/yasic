#include "video.h"
int video_width(void){
    int w;
    SDL_GetWindowSize(gamewindow.window,&w,NULL);
    return w;
}
int video_height(void){
    int h;
    SDL_GetWindowSize(gamewindow.window,NULL,&h);
    return h;
}

void video_render(void){
    SDL_RenderCopy(gamewindow.renderer, gamewindow.screen, NULL, NULL);
    SDL_RenderPresent(gamewindow.renderer);
}

void video_display_size(int *w, int *h){
    SDL_QueryTexture(gamewindow.screen, NULL, NULL, w,h);
}
