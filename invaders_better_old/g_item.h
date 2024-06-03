#ifndef GAME_ITEM_HEADER
#define GAME_ITEM_HEADER
#include <stdio.h>
#include <stdint.h>
#include <allegro5/allegro.h>

#define MAX_ITEM_LIST 20

enum ITEMINFO_FLAG {

    ITEMINFO_FLAG_DEFSHOOT = (1<<1),
    ITEMINFO_FLAG_PIERCE   = (1<<2),
    ITEMINFO_FLAG_RICOCHET = (1<<3),
    ITEMINFO_FLAG_INFINITE_AMMO = (1<<4),
    ITEMINFO_FLAG_WEAP      = (1<<5),
    ITEMINFO_FLAG_PICKUP    = (1<<6),
    ITEMINFO_FLAG_INSTAKILL     = (1<<16)

};

enum ITEM_ID {
    ITEM_ID_DEFAULT_CANNON,
    ITEM_ID_DOUBLE_CANNON,
    ITEM_ID_COUNT
};


typedef struct ITEM {
  int id;
  char name[50];
  ALLEGRO_BITMAP *sprite;
  uint32_t flags;
  int shot_time;
  int shot_count;
  int ammo;
  unsigned char active;
}ITEM;

typedef struct PLAYER PLAYER;

void item_init(void);
void item_drop_update(void);
void item_drop_draw(void);
void item_spawn_by_id(PLAYER *p, float initial_x, float initial_y, int id);
ITEM *item_free_slot(ITEM* item_list);
void item_pickup_killall(void);
int load_item_config(const char *config, ITEM *item);
/*
#define MAX_ITEM_LIST 8
enum ITEMINFO_FLAG {

    ITEMINFO_FLAG_DEFSHOOT = (1<<1),
    ITEMINFO_FLAG_PIERCE   = (1<<2),
    ITEMINFO_FLAG_RICOCHET = (1<<3),
    ITEMINFO_FLAG_INFINITE_AMMO = (1<<4),
    ITEMINFO_FLAG_WEAP      = (1<<5),
    ITEMINFO_FLAG_PICKUP    = (1<<6),
    ITEMINFO_FLAG_INSTAKILL     = (1<<16)

};

enum ITEM_ID {
    ITEM_ID_DEFAULT_CANNON,
    ITEM_ID_DOUBLE_CANNON,
    ITEM_ID_COUNT
};


typedef struct ITEM {
    int id;
    char name[50];
    char  sprite[255];
    float x;
    float y;
    float vx;
    float vy;
    uint32_t flags;
    unsigned char active;
    int ammo;
    void (*effect_callback)(struct ITEM *item, void *args);
    int ticks;
    int shot_time;
    int shot_num;
    int shot_delay;
}ITEM;

typedef struct PLAYER PLAYER;


void item_init(void);
ITEM *item_get_free(ITEM *items, unsigned  max);
ITEM *item_spawn_id(ITEM *items, unsigned size, PLAYER *player, float x, float y, int id);
ITEM *item_get_by_id(ITEM *item, int id);
void item_give_to_player_force(PLAYER *player, int pos, int id);
//void item_assign_to_player(PLAYER *player, ITEM *item);
void item_assign_to_player_id(PLAYER *player, int id);
int  item2player_collision(PLAYER *player, int pw, int ph, float x, float y, int w, int h);
int  item2item_collision(ITEM *a, ITEM *b);
void item_killall(ITEM *item_list, int cols);


ITEM *item_get_array(void);
void item_update(void);
void item_draw(void);
*/




#endif // GAME_ITEM_HEADER
