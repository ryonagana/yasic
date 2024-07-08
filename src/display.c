#include "display.h"
#include "utils.h"

#define _E(...) do { SDL_Log(__VA_ARGS__); }while(0);

DISPLAY g_display;
static SDL_Texture *tv_surface = NULL;

int Dsp_InitVideo(void){


    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        _E("SDL Failed to Init");
        return 0;
    }

    if(!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)){
        _E("Image Addon Failed to Load!");
        return 0;
    }

    if(TTF_Init() < 0){
        _E("TTF Not Loaded!");
        return 0;
    }



    /*
    if(!al_init()){
        _E("%s:%d: Allegro Can't Initialize", __FILE__, __LINE__);
        return 0;
    }

    if(!al_install_keyboard()){
        _E("%s:%d: Allegro Can't Initialize Keyboard", __FILE__, __LINE__);
        return 0;
    }

    if(!al_install_mouse()){
        _E("%s:%d: Allegro Can't Initialize Mouse", __FILE__, __LINE__);
        return 0;
    }

    #ifdef ENABLE_JOYSTICK
    if(!al_install_joystick()){
        _E("%s:%d: Allegro Can't Initialize Joystick", __FILE__, __LINE__);
        return 0xFA;
    }
    #endif // ENABLE_JOYSTICK

    #ifdef ENABLE_TOUCH
    if(!al_install_touch_input()){
        _E("%s:%d: Allegro Can't Initialize Touch", __FILE__, __LINE__);
        return 0xFA;
    }
    #endif

    if(!al_init_image_addon()){
        _E("%s:%d: Allegro Can't Initialize Image (Addon)", __FILE__, __LINE__);
        return 0;
    }

    if(!al_init_font_addon()){
        _E("%s:%d: Allegro Can't Initialize Font (Addon)", __FILE__, __LINE__);
        return 0;
    }

    if(!al_init_ttf_addon()){
        _E("%s:%d: Allegro Can't Initialize TTF (Addon)", __FILE__, __LINE__);
        return 0;
    }

    if(!al_init_native_dialog_addon()){
        _E("%s:%d: Allegro Can't Initialize Native Dialog (Addon)", __FILE__, __LINE__);
        return 0;
    }

    if(!al_init_primitives_addon()){
        _E("%s:%d: Allegro Can't Initialize Primitives (Addon)", __FILE__, __LINE__);
        return 0;
    }
    */


    return 1;
}

void Dsp_getMonitorInfo(int adapter, int *w, int *h){

    /*
    ALLEGRO_MONITOR_INFO m;
    al_get_monitor_info(adapter, &m);
    *w = m.x2  - m.x1;
    *h = m.y2  - m.y1;
    return;
    */
    return;
}

void Dsp_CapFrameRate(Uint32 deltatime){
    double frametime = 1000.0 / 60.0;
    if(deltatime < frametime){
        SDL_Delay((Uint32)frametime - deltatime);
    }
}

void Dsp_Render(void){
    //SDL_UpdateTexture(g_display.screen, NULL,  )
    SDL_RenderCopy(g_display.renderer, g_display.screen, NULL, NULL );
    SDL_RenderPresent(g_display.renderer);
}

void Dsp_CreateScreenBuffer(int w, int h)
{
    g_display.screen = SDL_CreateTexture(g_display.renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w,h);
}

void Dsp_CreateDisplay(int w, int h, int fullscreen, int enable_vsync, const char *window_title)
{

    g_display.window = SDL_CreateWindow(window_title,
                                            SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            w,h,
                                            SDL_WINDOW_SHOWN
                                        );

    int flags = SDL_RENDERER_ACCELERATED;

    if(enable_vsync){
        flags |= SDL_RENDERER_PRESENTVSYNC;
        g_display.vsync = enable_vsync;
    }

    g_display.renderer = SDL_CreateRenderer(g_display.window, -1, flags);
    g_display.width =  w;
    g_display.height = h;

    tv_surface = SDL_CreateTexture(g_display.renderer, SDL_PIXELFORMAT_RGBA32,  SDL_TEXTUREACCESS_STATIC, w,h);


    return;

}

void Dsp_RegisterEvents(void)
{

    return;
}

void Dsp_ShutdownVideo(void){

     SDL_DestroyTexture(g_display.screen);
     SDL_DestroyRenderer(g_display.renderer);
     SDL_DestroyWindow(g_display.window);
     SDL_DestroyTexture(tv_surface);

}

void Dsp_RenderNoise(void){

          /*
          if( NULL == s_noise_bg){
                s_noise_bg = al_create_bitmap(al_get_display_width(g_display.dsp), al_get_display_height(g_display.dsp) );
          }

          al_clear_to_color(al_map_rgb(0,0,0));
          ALLEGRO_LOCKED_REGION *lr = al_lock_bitmap(s_noise_bg,ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);

          for(int y = 0; y < al_get_bitmap_height(s_noise_bg);y++){

                  uint8_t *ptr = (uint8_t *)lr->data + y * lr->pitch;
                  for(int x = 0;x < al_get_bitmap_width(s_noise_bg);x++){



                         int rnd  = rand()%255-1;

                         int r = ptr[0] + rnd;
                         int g = ptr[1] + rnd;
                         int b = ptr[2] + rnd;
                         int a = ptr[3];
                         *((uint32_t *)ptr) = (r | (g<<8) | (b<<16) | (a<<24));
                         ptr += 4;

                  }
          }
          al_unlock_bitmap(s_noise_bg);
          al_draw_bitmap(s_noise_bg, 0,0,0);
          */

          return;
}

int Dsp_GetWindowWidth(void){
    int w;
    SDL_GetWindowSize(g_display.window, &w, NULL);
    return  w;
}

int Dsp_GetWindowHeight(void){
    int h;
    SDL_GetWindowSize(g_display.window, NULL, &h);
    return  h;
}

int Dsp_GetTimerSecs(void){
    return 0;
}

int Dsp_GetTimerTicks(void){
    return 0;
}

double Dsp_GetTime(void){
    return 0;
}
