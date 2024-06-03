#include "g_item.h"
#include "g_utils.h"
#include "main.h"
#include "g_sprites.h"
#include "g_render.h"
#include "g_sound.h"
#include "g_player.h"
#include "g_config.h"
#include <math.h>


typedef struct ITEM_PICKUP {
  int id;
  float x;
  float y;
  float vx;
  float vy;
  unsigned active;
}ITEM_PICKUP;


static ITEM_PICKUP s_items_pickup[MAX_ITEM_LIST];


int load_item_config(const char *config, ITEM *item);
  
void item_init(void){

  PLAYER *p = getPlayer(0);
  UNUSED(p);
  for(int i = 0; i < MAX_ITEM_LIST;i++){
    s_items_pickup[i].id = ITEM_ID_DEFAULT_CANNON;
    s_items_pickup[i].x  = 0;
    s_items_pickup[i].y  = 0;
    s_items_pickup[i].vx = 0;
    s_items_pickup[i].vy = 0;
    s_items_pickup[i].active = TRUE;
  }


}


int load_item_config(const char *config, ITEM *item){
    const char *name, *sprite;
    int flags, shot_time, shot_count, id;
    ALLEGRO_CONFIG *cfg = al_load_config_file(config);

    if(NULL == cfg){
      fprintf(stderr,"%s:%d - Error trying to load config %s", __FILE__, __LINE__, config);
      return FALSE;
    }
    
    id =  config_read_int(cfg,"properties", "id");
    name   = config_read_text(cfg, "properties", "name");
    sprite = config_read_text(cfg, "properties", "sprite");
    shot_time = config_read_int(cfg,"properties", "shot_time");
    shot_count = config_read_int(cfg,"properties", "shot_count");
    flags = config_read_int(cfg,"properties", "flags");

    item->id = id;
    snprintf(item->name,50,"%s", name);
    item->sprite = al_load_bitmap(sprite);
    item->flags = flags;
    item->shot_time = shot_time;
    item->shot_count = shot_count;

    return TRUE;
}

ITEM *item_free_slot(ITEM* item_list){
  int c = 0;
  
  while(item_list[c].active && c < MAX_ITEM_LIST) c++;

  if(c == MAX_ITEM_LIST) return NULL;

  
  return &item_list[c];
}

void item_pickup_killall(void){
  for(int i = 0; i < MAX_ITEM_LIST;i++){
    s_items_pickup[i].active = FALSE;
  }
}

ITEM_PICKUP* S_item_pickup_free_slot(int *index){
  for(int i = 0; i < MAX_ITEM_LIST;i++){
    if(s_items_pickup[i].active) continue;
    if(NULL != index){
      *index = i;
    }
    return &s_items_pickup[i];
  }

  if(NULL != index){
    *index = -1;
  }
  return NULL;
}


void item_drop_update(void){

  int w,h;

  w = al_get_display_width(display);
  h = al_get_display_height(display);
  
  for(int i = 0; i < MAX_ITEM_LIST;i++){
    if(s_items_pickup[i].active == FALSE) continue;
    if(s_items_pickup[i].id < 0) continue;

    ITEM_PICKUP *it = &s_items_pickup[i];

    if(NULL == it) continue;

    if(it->x <= 0 || it->x >= w-1){
      it->vx *= -1;
    }

    if(it->y <= 0){
      it->vy *= -1;
    }

    if(it->y >  h+32){
      it->active = FALSE;
    }
    
    it->x += it->vx;
    it->y += it->vy;

    //TO DO THE ITEM COLISION TO THE PLAYER

  }
}


void item_drop_draw(void){

  for(int i = 0; i < MAX_ITEM_LIST;i++){
    ITEM_PICKUP *it = &s_items_pickup[i];
    if(it->active == FALSE) continue;

    switch(it->id){
      default:
      case ITEM_ID_DEFAULT_CANNON:
	break;

      case ITEM_ID_DOUBLE_CANNON:
        al_draw_bitmap(sprites[SPR_DSHOT], it->x, it->y, 0);
        break;
      
    }
    
  }
}

void
item_spawn_by_id(PLAYER *p, float initial_x, float initial_y, int id)
{
  int index = -1;
  ITEM_PICKUP *it = S_item_pickup_free_slot(&index);
  double radians;
  int rnd;
  int dir = 1;
  
  if(NULL == it)
    return;

  if(index < 0){
    fprintf(stderr, "Cannot get Pickup Item Pointer at index: %d", index);
    return;
  }

  radians = angle_distance_rad(p->x, p->y, initial_x, initial_y);
  radians = radians * RAD2DEG;
  
  if(radians < 0){
    radians = PI2 + radians;
  }

  rnd = game_rand(100);
  
  if(rnd < 50)
    dir = -1;

  it->id = id;
  it->active = TRUE;
  it->x =  cos(radians*DEG2RAD) + initial_x;
  it->y =  sin(radians*DEG2RAD) + initial_y;
  it->vx = dir * 2.0f;
  it->vy = dir * 2.0f;

  return;
}

/*
static void S_item_effect_get_double_cannon(ITEM *item, void *args);
static void S_item_effect_get_default_cannon(ITEM *item, void *args);

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


ITEM *item_get_free(ITEM *items, unsigned max){

    for(unsigned i = 0; i < max; i++){
        if(!items[i].active){
            return &items[i];
        }
    }

    return NULL;
}

void item_init(void)
{

    S_load_items_config_file();
}

ITEM* item_spawn_id(ITEM *items, unsigned size, PLAYER *player, float x, float y, int id){
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

    it->id = id;
    //it = item_get_by_id(it, id);

    // if(NULL == it) return NULL;
    
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


static int item_player_exists(PLAYER *player, int id){

  int i;
  for(i = 0; i < ITEM_ID_COUNT;i++){
    if(player->items[i].id == id && player->items[i].active) return TRUE;
  }

  return FALSE;
}



void item_give_to_player_force(PLAYER *player, int pos, int id){

  ITEM *player_item, *item;


  item = item_get_by_id(player->items,id);

  if(NULL == item){
    fprintf(stderr, "%s:%d - item_get_by_id return NULL\n", __FILE__, __LINE__);
    return;
  }
  
  
  player_item = &player->items[pos];
  player_item->id = id;
  memcpy(player_item, item, sizeof(ITEM));
  player_item->active = TRUE;

  return;
}

void item_assign_to_player_id(PLAYER *player, int id){

  ITEM *item, *itemlist;

  


  item = item_get_by_id(player->items, id);
  itemlist = s_spawn_items_list;

  if(NULL == item){
    fprintf(stderr, "%s:%d - item dropped is NULL", __FILE__, __LINE__);
    return;
  }

  for(int i = 0; i < MAX_ITEM_LIST;i++){
    if(itemlist[i].id == item->id){
      player->shot_time = itemlist[i].shot_time;
      player->shot_count  = itemlist[i].shot_num;
      player->ammo      = itemlist[i].ammo;
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
  UNUSED(a); UNUSED(b);
  return FALSE;
}

void item_update(void){



    PLAYER *player = getPlayer(0);

    for(int i = 0; i < MAX_ITEM_LIST;i++){
        ITEM *it = &s_spawn_items_list[i];

	if(NULL == it)
	  continue;
	
	if(!it->active)
	  continue;


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
*/

