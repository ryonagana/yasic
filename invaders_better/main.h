#ifndef MAIN_HEADER
#define MAIN_HEADER

#include "g_particles.h"

typedef struct DIFFICULTY_PARAMS {
    int type;
    float speed_multiplier;
    int enemy_shot_time;
    float shot_speed;
    int enemies_num_shoot;

}DIFFICULTY_PARAMS;

enum GAME_DIFFICULTY {
    DIFF_NOVICE,
    DIFF_EASY,
    DIFF_NORMAL,
    DIFF_HARD,
    DIFF_NIGHTMARE,

    DIFF_COUNT
};


typedef struct MOUSECOORD {
    int x,y;
    float z;
    unsigned int buttons;
} MOUSECOORD;


typedef struct POINT {
    float x;
    float y;

}POINT;

typedef struct PLAYER PLAYER;

void do_gameover(void);

extern DIFFICULTY_PARAMS difficulty[DIFF_COUNT];
extern PARTICLE particles[MAX_PARTICLES];

extern MOUSECOORD g_mouse;
extern int player_keys[ALLEGRO_KEY_MAX];
extern int g_game_paused;
extern int g_game_started;
extern int g_gamestate;

enum GAMESTATE_TYPE {
        GAMESTATE_TYPE_MENU,
        GAMESTATE_TYPE_GAMEPLAY,
        GAMESTATE_TYPE_HISCORE,
        GAMESTATE_TYPE_OPTIONS,
        GAMESTATE_TYPE_GAMEOVER,
        GAMESTATE_TYPE_USER_HISCORE,
        GAMESTATE_TYPE_INTROS
};


double angle_distance_rad(double x1, double y1, double x2, double y2);


PLAYER* getPlayer(int pos);


#endif // MAIN_HEADER
