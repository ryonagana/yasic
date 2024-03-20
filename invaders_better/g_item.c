#include "g_item.h"
#include "g_utils.h"
#include "main.h"
#include "g_sprites.h"
#include "g_render.h"
#include "g_sound.h"
#include "g_player.h"
#include <math.h>


static void S_item_effect_get_double_cannon(ITEM *item, void *args);
static void S_item_effect_get_default_cannon(ITEM *item, void *args);


static ITEM i_default_laser_cannon = {
    ITEM_ID_DEFAULT_CANNON,
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
    0,0,
    2,2,
    ITEMINFO_FLAG_DEFSHOOT | ~ITEMINFO_FLAG_INFINITE_AMMO,
    FALSE,
    0,
    S_item_effect_get_double_cannon,
    0,
    45,
    2
};


static ITEM s_item_index[ITEM_ID_COUNT] = {0};
static ITEM s_spawn_items_list[MAX_ITEM_LIST] = {0};


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


    s_item_index[ITEM_ID_DEFAULT_CANNON] = i_default_laser_cannon;
    s_item_index[ITEM_ID_DOUBLE_CANNON] = i_double_laser_cannon;


    s_spawn_items_list[0] = s_item_index[ITEM_ID_DEFAULT_CANNON];
    s_spawn_items_list[1] = s_item_index[ITEM_ID_DOUBLE_CANNON];

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

    it = item_get_by_id(it, id);
    it->active = TRUE;
    it->x =  cos(angle) + x;
    it->y =  sin(angle) + y;

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

    for(size_t i = 0; i < sizeof(s_item_index)/sizeof(s_item_index[0]); i++){
        if(s_item_index[i].id == id){
            player->item_use = &s_item_index[i];
        }
    }

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

    for(int i = 0; i < MAX_ITEM_LIST;i++){
        ITEM *it = &s_spawn_items_list[i];
        if(!it->active) continue;


        if(it->x < -10 || it->x >= al_get_display_width(display)  + 10  ||
           it->y < -10 || it->y >= al_get_display_height(display) + 10)
        {
             it->active = FALSE;
        }


        it->x += it->vx;
        it->y += it->vy;
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
