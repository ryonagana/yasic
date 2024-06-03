#include "sound.h"
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

ALLEGRO_MIXER *mixer = NULL;
ALLEGRO_VOICE *voice = NULL;

void Snd_Init(int channels){
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


}
void Snd_End(void){
    if(al_is_audio_installed()){
        al_uninstall_audio();
    }

    if(mixer){
        al_destroy_mixer(mixer);
        mixer = NULL;
    }
    if(voice){
        al_destroy_voice(voice);
        voice = NULL;
    }
}
