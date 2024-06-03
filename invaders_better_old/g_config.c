#include "g_config.h"
#include <stdlib.h>
#include "main.h"
#include "g_utils.h"

int config_read_int  (ALLEGRO_CONFIG *cfg, const char *section, const char *key){
    const char *value = NULL;
    value = al_get_config_value(cfg, section, key);
    return atoi(value);

}
float config_read_float(ALLEGRO_CONFIG *cfg, const char *section, const char *key){
    const char *v = al_get_config_value(cfg,section,key);
    return atof(v);
}
const char* config_read_text (ALLEGRO_CONFIG *cfg, const char *section, const char *key){

    return al_get_config_value(cfg,section,key);
}
