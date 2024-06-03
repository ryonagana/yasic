#ifndef GAME_SOUND_HEADER
#define GAME_SOUND_HEADER
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#define MAX_SFX 20

extern ALLEGRO_MIXER *mixer;
extern ALLEGRO_VOICE *voice;
extern ALLEGRO_SAMPLE *sfx_list[MAX_SFX];

enum {
        SFX_EXPLOSION1,
        SFX_EXPLOSION2,
        SFX_EXPLOSION3,
        SFX_LASER,
        SFX_WALK,
        SFX_HIT,
        SFX_POWERUP,
        SFX_POWERUP2,
        SFX_POWERUP3,
        SFX_SHIP,
        SFX_ENEMY_SHOOT,
        SFX_EXPLOSION4,
        SFX_COLLISION,
        SFX_TOTAL
};



int init_audio(void);
void play_sound(int id, float volume, float pan, float speed, ALLEGRO_PLAYMODE mode);
ALLEGRO_SAMPLE_ID play(int id);
void destroy_audio(void);


#endif // GAME_SOUND_HEADER
