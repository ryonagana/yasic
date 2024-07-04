#include "sound.h"
#include <stdio.h>
#include "utils.h"



void Snd_Init(int channels){
    UNUSED(channels);
    /*
    if(!al_is_audio_installed()){
        if(!al_install_audio()){
            fprintf(stderr, "Audio Error! - No Sound");
            return;
        }

        if(!al_init_acodec_addon()){
            fprintf(stderr, "Audio Codec Addon Error! - No Sound");
            return;
        }
    }

    voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
    mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

    if(!al_attach_mixer_to_voice(mixer, voice)){
            fprintf(stderr, "Attach mixer to voice error");
            return;
    }

    if(!al_set_default_mixer(mixer)){
        fprintf(stderr, "default mixer to voice error");
        return;
    }

    al_reserve_samples(channels);
    */
    return;


}
void Snd_End(void){

    /*
    if(mixer){
        al_destroy_mixer(mixer);
        mixer = NULL;
    }
    if(voice){
        al_destroy_voice(voice);
        voice = NULL;
    }

    if(al_is_audio_installed()){
        al_uninstall_audio();
    }*/
    return;
}
