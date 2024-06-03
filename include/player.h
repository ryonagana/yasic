#ifndef PLAYER_H
#define PLAYER_H

typedef struct PLAYER {
    int x;
    int y;
    int vx;
    int vy;
    int shoot_timer;
    unsigned int shoot_count;
}PLAYER;

void Player_Init(PLAYER *p);
void Player_SpawnPos(PLAYER *p, int x, int y);
void Player_SetShootTime(PLAYER *p, int timer);
void Player_SetShootCount(PLAYER *p, int count);

#endif // PLAYER_H
