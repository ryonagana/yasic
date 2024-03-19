#ifndef GAME_ITEM_HEADER
#define GAME_ITEM_HEADER

#define MAX_ITEM_LIST 8


enum ITEMINFO_FLAG {

    ITEMINFO_FLAG_DEFSHOOT = (1<<1),
    ITEMINFO_FLAG_PIERCE   = (1<<2),
    ITEMINFO_FLAG_RICOCHET = (1<<3),
    ITEMINFO_FLAG_INFINITE_AMMO = (1<<4),
    ITEMINFO_FLAG_INSTAKILL     = (1<<16)

};

enum ITEM_ID {
    ITEM_ID_DEFAULT_CANNON,
    ITEM_ID_DOUBLE_CANNON,
    ITEM_ID_COUNT
};

struct ITEM;

typedef struct ITEMINFO {
    int id;
    int flags;
    char name[25];
    int delay;
    int ammo;
    int shots;
    int effect_time;
    void (*effect_callback)(struct ITEM *item, void *args);

}ITEMINFO;


typedef struct ITEM {
    ITEMINFO info;
    float x;
    float y;
    float vx;
    float vy;
    int active;
    int ttl;


}ITEM;

typedef struct PLAYER PLAYER;



ITEM *item_get_free(ITEM list[MAX_ITEM_LIST]);
void item_spawn(ITEM *items_list, float x, float y, int id);
void item_add_player(PLAYER *p, int id);
void item_init(ITEM *item);
void item_update(PLAYER *player, ITEM *item_list);
void item_draw(ITEM *item_list);






#endif // GAME_ITEM_HEADER
