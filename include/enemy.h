#ifndef ENEMY_HEADER
#define ENEMY_HEADER
#include <SDL2/SDL.h>
#include "convert.h"
#define ENEMY_ROW 8
#define ENEMY_COL 8

struct enemy_t {
    SDL_FRect pos;
    SDL_FRect vel;
    int alive;
    int shoot_time;
    int direction;
    int line;
};

struct enemy_data {
    struct enemy_t enemies[ENEMY_ROW][ENEMY_COL];
    int rows;
    int cols;

};

void enemy_init(struct enemy_data *en, int max_col, int max_row);
void enemy_update(struct enemy_data *en, float delta);
void enemy_draw(struct enemy_data *en, SDL_Texture *tex);
#endif // ENEMY_HEADER
