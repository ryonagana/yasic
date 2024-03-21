#ifndef GAME_CONFIG_HEADER
#define GAME_CONFIG_HEADER

#include <allegro5/allegro.h>
#include <stdio.h>


int         config_read_int  (ALLEGRO_CONFIG *cfg, const char *section, const char *key);
float       config_read_float(ALLEGRO_CONFIG *cfg, const char *section, const char *key);
const char* config_read_text (ALLEGRO_CONFIG *cfg, const char *section, const char *key);

#endif // GAME_CONFIG_HEADER
