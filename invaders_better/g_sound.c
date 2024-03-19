#include "g_sound.h"
#include "g_render.h"
#include <stdio.h>
#include <stdlib.h>

ALLEGRO_MIXER *mixer = NULL;
ALLEGRO_VOICE *voice = NULL;
ALLEGRO_SAMPLE *sfx_list[MAX_SFX];


int init_audio(void){
    int error = 0;

    char error_message[255];

    #define SND_PTR_FAILED(x) (!x||x == NULL)


    voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);

    if(voice == NULL) {
      error++;
      sprintf(error_message, "Failed created voice\n");
    }

    mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    if(mixer == NULL) {
      error++;
      sprintf(error_message, "Failed created mixer\n");
    }


    if(!al_attach_mixer_to_voice(mixer, voice)){
      error++;
      sprintf(error_message, "Failed to attach mixer to voice\n");
    }

    if(!al_set_default_mixer(mixer)) {
      error++;
      sprintf(error_message, "Failed set default mixer\n");
    }

    if(!al_reserve_samples(16)) {
      error++;
      sprintf(error_message, "Failedto reserve samples\n");
    }



    sfx_list[SFX_EXPLOSION1] = al_load_sample("assets//sfx//explosion01.wav");
    sfx_list[SFX_EXPLOSION2] = al_load_sample("assets//sfx//explosion02.wav");
    sfx_list[SFX_EXPLOSION3] = al_load_sample("assets//sfx//explosion03.wav");
    sfx_list[SFX_LASER] = al_load_sample("assets//sfx//laserShoot.wav");
    sfx_list[SFX_WALK] = al_load_sample("assets//sfx//walk.wav");
    sfx_list[SFX_HIT] = al_load_sample("assets//sfx//hit.wav");
    sfx_list[SFX_POWERUP] = al_load_sample("assets//sfx//powerUp.wav");
    sfx_list[SFX_POWERUP2] = al_load_sample("assets//sfx//powerUp2.wav");
    sfx_list[SFX_POWERUP3] = al_load_sample("assets//sfx//powerUp3.wav");
    sfx_list[SFX_SHIP] =  al_load_sample("assets//sfx//ship.wav");
    sfx_list[SFX_EXPLOSION4] = al_load_sample("assets//sfx//big_explosion.ogg");
    sfx_list[SFX_ENEMY_SHOOT] = al_load_sample("assets//sfx//fire_small.ogg");
    sfx_list[SFX_COLLISION] = al_load_sample("assets//sfx//collision.ogg");

    int i;

    for(i = 0; i < SFX_TOTAL-1; i++){
        if(SND_PTR_FAILED(sfx_list[i])){
            error++;
        }
    }

    #undef SND_PTR_FAILED

    if(error){
        al_show_native_message_box(display,"Error Loading Sounds!", "Error:", "Error Loading Sounds: Sounds is Missing!", NULL,0);
    }

    return error;
}

void play_sound(int id, float volume, float pan, float speed, ALLEGRO_PLAYMODE mode){
    if(sfx_list[id]){
        al_play_sample(sfx_list[id], volume, pan, speed, mode, NULL);
        return;
    }

    fprintf(stderr,"sound invalid");
    return;
}

ALLEGRO_SAMPLE_ID play(int id){
    ALLEGRO_SAMPLE_ID sample_id = {0};
    if(sfx_list[id]){
        al_play_sample(sfx_list[id], 1.0,0.0,1.0, ALLEGRO_PLAYMODE_ONCE, &sample_id);
        return sample_id;
    }

    fprintf(stderr,"sound invalid");
    return sample_id;
}

void destroy_audio(void){

    for(int i = 0; i < MAX_SFX;i++){
        if(sfx_list[i]){
            al_destroy_sample(sfx_list[i]);
            sfx_list[i] = NULL;
        }
    }
}
