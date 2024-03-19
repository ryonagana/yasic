#include "g_item.h"
#include "g_utils.h"
#include "main.h"
#include "g_sprites.h"
#include "g_render.h"
#include "g_sound.h"
#include "g_player.h"

static void S_item_effect_get_double_cannon(ITEM *item, void *args){
    PLAYER *player = args;

    item->info.flags = ITEMINFO_FLAG_DEFSHOOT | ~ITEMINFO_FLAG_INFINITE_AMMO;
    item->info.ammo = 10;
    player->ammo =  item->info.ammo;

}
static void S_item_effect_get_default_cannon(ITEM *item, void *args){
    UNUSED(args);
    item->info.flags = ITEMINFO_FLAG_INFINITE_AMMO | ITEMINFO_FLAG_DEFSHOOT;
    item->info.ammo = 0;

}



static ITEMINFO item_info_list[ITEM_ID_COUNT] = {
    {
       ITEM_ID_DEFAULT_CANNON,
       ITEMINFO_FLAG_DEFSHOOT | ITEMINFO_FLAG_INFINITE_AMMO,
       "Default Laser Cannon",
       40,
       0,
       1,
       0,
       S_item_effect_get_default_cannon
    },

    {
       ITEM_ID_DOUBLE_CANNON,
       ITEMINFO_FLAG_DEFSHOOT,
       "Double Laser Cannon",
       80,
       0,
       2,
       0,
       S_item_effect_get_double_cannon
    }

};


ITEM *item_get_free(ITEM list[MAX_ITEM_LIST]){
    int i;
    for(i = 0; i < MAX_ITEM_LIST;i++){
        if(list[i].active) continue;
        break;
    }

    if(i == MAX_ITEM_LIST) return NULL;

    return &list[i];
}


void item_init(ITEM *items){

    memset(items, 0, sizeof(*items));

    for(int i = 0; i < ITEM_ID_COUNT;i++){
        items[i].info = item_info_list[i];
        items[i].active = TRUE;
        items[i].x = 0;
        items[i].y = 0;
        items[i].vx = 0;
        items[i].vy = 0;
        items[i].ttl = game_rand_range(50,250);
        items[i].info.flags = 0;
    }

}

void item_spawn(ITEM *item_list, float x, float y, int id){
    ITEM *item = item_get_free(item_list);

    if(item == NULL)
        return;

    item->x = x;
    item->y = y;
    item->info = item_info_list[id];
    item->vx = 2.0;
    item->vy = 2.0;
    item->active = TRUE;
    item->ttl = game_rand_range(60,150);
}


void item_update(PLAYER *player, ITEM *item_list){



    for(int i = 0; i < MAX_ITEM_LIST;i++){
        if(!item_list[i].active && item_list[i].ttl <= 0) continue;

        ITEM *item = &item_list[i];

        if(item->x < -10 || item->x >= al_get_display_width(display)  + 10  ||
           item->y < -10 || item->y >= al_get_display_height(display) + 10
          ){
             item->active = FALSE;
        }



        item->x += item->vx;
        item->y += item->vy;


        if(rect_collision(player->x,player->y,32,32, item->x,item->y,32,32)){
            play(SFX_POWERUP2);
            item->active = FALSE;
            item_add_player(player, item->info.id);

            /*

            if(item->info.effect_callback){
                item->info.effect_callback(item, player);
            }
            item->active = FALSE;
            */

            break;

        }

        if(item->ttl > 0){
            item->ttl--;
        }


    }
}

void item_draw(ITEM *item_list){


    for(int i = 0; i < MAX_ITEM_LIST;i++){
        if(!item_list[i].active && item_list[i].ttl <= 0) continue;

        ITEM *item = &item_list[i];

        switch(item->info.id){
        default:
            case ITEM_ID_DEFAULT_CANNON:
            break;

            case ITEM_ID_DOUBLE_CANNON:
                {
                    al_draw_bitmap(sprites[SPR_DSHOT], item->x, item->y, 0);
                }
            break;
        }
    }
}


void item_add_player(PLAYER *p, int id){

    for(int i = 0; i < ITEM_ID_COUNT;i++){
        if(p->items[i].active) continue;

        p->items[i].active = TRUE;
        p->items[i].info = item_info_list[id%ITEM_ID_COUNT];
        p->items[i].info.effect_callback(&p->items[i], p);
        p->item_use = &p->items[i];
        break;
    }


}


