#ifndef ENEMY_HEADER
#define ENEMY_HEADER
#include <SDL2/SDL.h>
#include "convert.h"
#include "ship.h"
#include "bullet.h"
#define ENEMY_ROW (8)
#define ENEMY_COL (8)

#define ENEMY_FLAG_BONUS_MODE 0x2



#define ENEMY_MAX_BULLETS 10

struct enemy_t {
    SDL_FRect pos;
    SDL_FRect vel;
    int alive;
    int shoot_time;
    int direction;
    int line;
    int flags;
    struct bullet_t bullets[ENEMY_MAX_BULLETS];

};

struct enemy_data {
    struct enemy_t enemies[ENEMY_ROW][ENEMY_COL];
    int rows;
    int cols;

};



void enemy_init(struct enemy_data *en, int max_col, int max_row);
void enemy_update(struct enemy_data *en, double delta);
void enemy_draw(struct enemy_data *en, SDL_Texture *tex);
int enemy_alive_count(struct enemy_data *en);
struct enemy_t *enemy_get_free(struct enemy_data *en);
void enemy_init_array(struct enemy_t *e_list, int max);
void enemy_player_bullet_collision(struct enemy_t *e , struct spaceship *ship);
struct enemy_t* enemy_select_random_enemy(struct enemy_data *d);
#endif // ENEMY_HEADER
