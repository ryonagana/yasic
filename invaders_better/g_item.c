#include "g_item.h"
#include "g_utils.h"
#include "main.h"
#include "g_sprites.h"
#include "g_render.h"
#include "g_sound.h"
#include "g_player.h"
#include "g_config.h"
#include <math.h>


static void S_item_effect_get_double_cannon(ITEM *item, void *args);
static void S_item_effect_get_default_cannon(ITEM *item, void *args);


/*
static ITEM i_default_laser_cannon = {
    ITEM_ID_DEFAULT_CANNON,
    "",
    "",
    0,0,
    0,0,
    ITEMINFO_FLAG_DEFSHOOT | ITEMINFO_FLAG_INFINITE_AMMO,
    FALSE,
    0,
    S_item_effect_get_default_cannon,
    0,
    30,
    1

};
static ITEM i_double_laser_cannon = {
    ITEM_ID_DOUBLE_CANNON,
    "",
    "",
    0,0,
    2,2,
    ITEMINFO_FLAG_DEFSHOOT & ~ITEMINFO_FLAG_INFINITE_AMMO,
    FALSE,
    0,
    S_item_effect_get_double_cannon,
    0,
    60,
    2
};
*/

//static ITEM s_item_index[ITEM_ID_COUNT] = {0};
static ITEM s_spawn_items_list[MAX_ITEM_LIST] = {0};


static void S_item_load_config(ALLEGRO_CONFIG *cfg, ITEM *item){
    const char *name, *sprite;
    int flags, shot_time, shot_count, id;

    id =  config_read_int(cfg,"properties", "id");
    name   = config_read_text(cfg, "properties", "name");
    sprite = config_read_text(cfg, "properties", "sprite");
    shot_time = config_read_int(cfg,"properties", "shot_time");
    shot_count = config_read_int(cfg,"properties", "shot_count");
    flags = config_read_int(cfg,"properties", "flags");

    item->id = id;
    snprintf(item->name, sizeof(item->name), "%s", name);
    snprintf(item->sprite, sizeof(item->sprite), "%s", sprite);
    item->shot_time = shot_time;
    item->shot_num = shot_count;
    item->flags = flags;
    item->shot_delay = 0;

}

static void S_load_items_config_file(void){
    ALLEGRO_FS_ENTRY *entry, *next;
    entry = al_create_fs_entry("assets//cfg");
    const char *name = NULL;
    ALLEGRO_CONFIG *config = NULL;

    if(!al_open_directory(entry)){
        al_destroy_fs_entry(entry);
        return;
    }

    int count = 0;

    do{
        next = al_read_directory(entry);

        if(next == NULL)
            break;

        if(al_get_fs_entry_mode(next) & ALLEGRO_FILEMODE_ISFILE){
            name = al_get_fs_entry_name(next);
            config = al_load_config_file(name);

            if(config == NULL)
                continue;

            S_item_load_config(config, &s_spawn_items_list[count]);
            al_destroy_config(config);
        }

        al_destroy_fs_entry(next);
        count++;
    }while(1);
    al_close_directory(entry);
    al_destroy_fs_entry(entry);

}


ITEM *item_get_free(ITEM *items, int max){

    for(int i = 0; i < max; i++){
        if(!items[i].active){
            return &items[i];
        }
    }

    return NULL;
}

void item_init(void)
{


    //s_item_index[ITEM_ID_DEFAULT_CANNON] = i_default_laser_cannon;
    //s_item_index[ITEM_ID_DOUBLE_CANNON] = i_double_laser_cannon;


    //s_spawn_items_list[0] = s_item_index[ITEM_ID_DEFAULT_CANNON];
    //s_spawn_items_list[1] = s_item_index[ITEM_ID_DOUBLE_CANNON];

    S_load_items_config_file();



}

ITEM* item_spawn_id(ITEM *items, int size, PLAYER *player, float x, float y, int id){
    ITEM *it = NULL;

    it = item_get_free(items, size);
    if(it == NULL)
        return NULL;


    double angle = angle_distance_rad(player->x, player->y, x,y);

    if(angle < 0){
        angle = PI2 + angle;
    }

    int rnd = game_rand(100);
    int direction = 1;
    if(rnd < 50) direction = -1;


    it = item_get_by_id(it, id);
    it->active = TRUE;
    it->x =  cos(angle) + x;
    it->y =  sin(angle) + y;
    it->vx = direction * 2.0f;
    it->vy = direction * 2.0f;

    return it;
}



void S_item_effect_get_double_cannon(ITEM *item, void *args){
    PLAYER *player = args;

    item->flags = ITEMINFO_FLAG_DEFSHOOT | ~ITEMINFO_FLAG_INFINITE_AMMO;
    item->ammo = 10;
    player->ammo =  item->ammo;

}

void S_item_effect_get_default_cannon(ITEM *item, void *args){
    UNUSED(args);
    item->flags = ITEMINFO_FLAG_INFINITE_AMMO | ITEMINFO_FLAG_DEFSHOOT;
    item->ammo = 0;
}


void item_assign_to_player(PLAYER *player, ITEM *item){
    if(player != NULL){
        player->item_use = item;
        return;
    }

    return;
}
void item_assign_to_player_id(PLAYER *player, int id){

    int c = 0;

    for(c = 0; c < MAX_ITEM_LIST;c++){
        if(s_spawn_items_list[c].id == id) break;
    }


   player->item_use = &s_spawn_items_list[c];
   player->shot_time = player->item_use->shot_time;

}

int item2player_collision(PLAYER *player, int pw, int ph, float x, float y , int w, int h){

    if(rect_collision(player->x, player->y, pw,ph, x,y,w,h)){
        return TRUE;
    }

    return FALSE;

}

int  item2item_collision(ITEM *a, ITEM *b){
    if(rect_collision(a->x,a->y, 32,32,b->x,b->y, 32,32)){
        return TRUE;
    }

    return FALSE;
}

void item_update(void){



    PLAYER *player = getPlayer(0);

    for(int i = 0; i < MAX_ITEM_LIST;i++){
        ITEM *it = &s_spawn_items_list[i];
        if(!it->active) continue;


        if(it->x <= 0 || it->x >= al_get_display_width(display) - 1){
            it->vx *= -1;
        }

        if(it->y <= 0){
            it->vy *= -1;
        }

        if(it->y >= al_get_display_height(display) + 32){
            it->active = FALSE;
        }


        it->x += it->vx;
        it->y += it->vy;

        if(it->active && item2item_collision(it, &s_spawn_items_list[i+1])){
            it->vx *= -1;
            s_spawn_items_list[i+1].vx *= -1;
        }

        if(it->active && item2player_collision(player,32,32, it->x, it->y,32,32)){
            it->active = FALSE;
            particle_explosion(particles, player->x, player->y, 90,110,60, al_map_rgb_f(1,1,1));
            item_assign_to_player_id(player, it->id);
            play(SFX_POWERUP);
        }

    }

}
void item_draw(void){

    for(int i = 0; i < MAX_ITEM_LIST;i++){
        ITEM *it = &s_spawn_items_list[i];
        if(!it->active) continue;

        switch(it->id){
            case ITEM_ID_DEFAULT_CANNON:
                break;
            case ITEM_ID_DOUBLE_CANNON:
                al_draw_bitmap(sprites[SPR_DSHOT], it->x, it->y, 0);
            default:
                break;
        }

    }
}

ITEM *item_get_array(void){
    return s_spawn_items_list;
}

ITEM *item_get_by_id(ITEM *item, int id){

    for(int i = 0; i < ITEM_ID_COUNT;i++){
        if(s_spawn_items_list[i].id == id){
            memcpy(item, &s_spawn_items_list[i], sizeof(ITEM));
            return item;
        }
    }

    return NULL;

}

void item_killall(ITEM *item_list, int cols){
    for(int i = 0; i < cols;i++){
       item_list[i].active = FALSE;
    }
}

