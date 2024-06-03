#ifndef GAME_SPRITES_HEADER
#define GAME_SPRITES_HEADER
#include "g_utils.h"

#define SPRITE_MAX 12

enum SPRITE_ID {
    SPR_BG01,
    SPR_CANNON,
    SPR_ENEMY01,
    SPR_ENEMY02,
    SPR_ENEMYSHOT,
    SPR_PLAYERSHOT,
    SPR_TITLE,
    SPR_DSHOT

};


extern ALLEGRO_BITMAP *sprites[SPRITE_MAX];
void init_sprites(void);
#endif // GAME_SPRITES_HEADER
