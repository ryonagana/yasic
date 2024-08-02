#ifndef VIDEO_HEADER
#define VIDEO_HEADER
#include <SDL2/SDL.h>

#define VIDEO_SCALE 2.0

struct _gamewindow {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_Texture  *screen;
};

extern struct _gamewindow gamewindow;

int video_width(void);
int video_height(void);
void video_render(void);
void video_display_size(int *w, int *h);

#endif // VIDEO_HEADER
