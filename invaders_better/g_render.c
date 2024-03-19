#include "g_render.h"
#include <stdio.h>
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *screen = NULL;
ALLEGRO_BITMAP *buffer  = NULL;
ALLEGRO_FONT *debug_font = NULL;


int allegro_init(char *error_message) {

    int error = 0;

    if(!al_init()){
        error++;
        sprintf(error_message, "Allegro Cant Initilialized\n");
    }

    if(!al_install_keyboard()){
        error++;
        sprintf(error_message, "Keyboard not Installed\n");
    }

    if(!al_install_mouse()){
        error++;
        sprintf(error_message, "Mouse not Installed\n");
    }


    if(!al_install_audio()){
        error++;
        sprintf(error_message, "Audio Error!\n");
    }

    if(!(al_init_font_addon() && al_init_ttf_addon())){
                error++;
        sprintf(error_message, "TTF Error!\n");
    }

    if(!al_init_acodec_addon()){
        error++;
        sprintf(error_message, "Audio Codecs Errors!\n");
    }

    al_reserve_samples(16);

    if(!al_init_native_dialog_addon()){
        error++;
        sprintf(error_message, "Native Dialog Errors!\n");
    }

    if(!al_init_primitives_addon()){
        error++;
        sprintf(error_message, "Primitives Errors!\n");
    }

    if(!al_init_image_addon()){
        error++;
        sprintf(error_message, "Image Not Loaded!\n");
    }

    return error;
}

