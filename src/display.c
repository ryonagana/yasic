#include "display.h"
#include "utils.h"

#define _E(...) do { fprintf(stderr,__VA_ARGS__); }while(0);

DISPLAY g_display;
static ALLEGRO_BITMAP *s_noise_bg = NULL;



int Dsp_InitVideo(void){

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


    return 1;
}

void Dsp_getMonitorInfo(int adapter, int *w, int *h){
    ALLEGRO_MONITOR_INFO m;
    al_get_monitor_info(adapter, &m);
    *w = m.x2  - m.x1;
    *h = m.y2  - m.y1;
    return;
}

void Dsp_CreateDisplay(int w, int h, int fullscreen, int enable_vsync, const char *window_title)
{
    int flags = 0;
    char title[127];

    if(w <= 1 || h <= 1){
        Dsp_getMonitorInfo(0,&w, &h);
    }

    if(w <= 640) w = 640;
    if(h <= 480) h = 480;

    if(enable_vsync > 0){
        al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
    }

    if(fullscreen > 0){
        flags = ALLEGRO_FULLSCREEN_WINDOW;
    }else {
        flags = ALLEGRO_WINDOWED;
    }

    flags |=  ALLEGRO_OPENGL|ALLEGRO_OPENGL_3_0|ALLEGRO_PROGRAMMABLE_PIPELINE|ALLEGRO_GENERATE_EXPOSE_EVENTS;
    al_set_new_display_flags(flags);

    snprintf(title, sizeof(title), "%s", window_title);
    al_set_new_window_title(title);

    #define INVALID_PTR(ptr) (ptr == NULL || !ptr)

    g_display.dsp = al_create_display(w,h);

    if(INVALID_PTR(g_display.dsp)){
        _E("%s:%d: Invalid Display - %d", __FILE__, __LINE__, al_get_errno());
        exit(1);
    }

    g_display.queue = al_create_event_queue();
    if(INVALID_PTR(g_display.queue)){
        _E("%s:%d: Invalid Queue", __FILE__, __LINE__);
        exit(1);
    }

    g_display.timer = al_create_timer(1.0/60.0);
    if(INVALID_PTR(g_display.timer)){
        _E("%s:%d: Invalid Timer", __FILE__, __LINE__);
        exit(1);
    }

    g_display.screen = al_create_bitmap(w,h);
    if(INVALID_PTR(g_display.screen)){
        _E("%s:%d: Invalid Screen Buffer", __FILE__, __LINE__);
        exit(1);
    }


    #undef INVALID_PTR

    return;

}

void Dsp_RegisterEvents(void){
    al_register_event_source(g_display.queue, al_get_keyboard_event_source());
    al_register_event_source(g_display.queue, al_get_mouse_event_source());
    al_register_event_source(g_display.queue, al_get_timer_event_source(g_display.timer));
    al_register_event_source(g_display.queue, al_get_display_event_source(g_display.dsp));

    al_start_timer(g_display.timer);

    #ifdef ENABLE_JOYSTICK
    al_register_event_source(g_display.queue, al_get_joystick_event_source());
    #endif // ENABLE_JOYSTICK

    #ifdef ENABLE_TOUCH
    al_register_event_source(g_display.queue, al_get_touch_input_event_source());
    #endif // ENABLE_TOUCH
}

void Dsp_ShutdownVideo(void){
    if(g_display.screen){
        al_destroy_bitmap(g_display.screen);
        g_display.screen = NULL;
    }

    if(g_display.queue){
        al_destroy_event_queue(g_display.queue);
    }

    if(g_display.timer){
        al_stop_timer(g_display.timer);
        al_destroy_timer(g_display.timer);
    }

    if(g_display.dsp){
        al_destroy_display(g_display.dsp);
    }

    if(s_noise_bg){
        al_destroy_bitmap(s_noise_bg);
    }
}

void Dsp_RenderNoise(void){
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
}

int Dsp_GetWindowWidth(void){
    return al_get_display_width(g_display.dsp);
}

int Dsp_GetWindowHeight(void){
    return al_get_display_height(g_display.dsp);
}

int Dsp_GetTimerSecs(void){
    return al_get_timer_count(g_display.timer)/60;
}

int Dsp_GetTimerTicks(void){
    return al_get_timer_count(g_display.timer);
}

double Dsp_GetTime(void){
    return al_get_time();
}
