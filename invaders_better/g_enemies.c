#include <assert.h>
#include "g_sound.h"
#include "g_enemies.h"
#include "g_utils.h"
#include "main.h"
#include "g_sprites.h"
#include "g_render.h"
#include "g_particles.h"


#include "math.h"
#include "g_player.h"

static int enemy_direction = 1;
static int walk_time = WALK_TIME_DELAY_PHASE1;

const float explosion_speed[4] = {
     .5f,
     1.0f,
     1.2f,
     2.0f,
};



ENEMY* getFreeEnemy(ENEMY (*enemies)[ENEMY_COLS]){
    for(int i = 0; i < ENEMY_ROWS; i++){
        for(int j = 0; j < ENEMY_COLS;j++){

            if(!enemies[i][j].alive){
                return &enemies[i][j];
            }
        }
    }
    return NULL;
}



void enemies_init(ENEMY (*enemies)[ENEMY_COLS]) {

        for(int y = 0; y < ENEMY_ROWS;y++){
            for(int x = 0; x < ENEMY_COLS;x++){

                int rand_type = game_rand(100);

                enemies[y][y].type =   rand_type > 50 ? 1 : 2;
                enemies[y][x].x = x * TILE * 1.5;
                enemies[y][x].y = y * TILE * 1.5;
                enemies[y][x].alive = 1;
                enemies[y][x].type = 1;
                enemies[y][x].life = 1;
                memset(enemies[y][x].bullets, 0, sizeof(enemies[y][x].bullets));

            }

        }
}

void enemies_killall(ENEMY (*enemies)[ENEMY_COLS]){
    for(int i = 0; i < ENEMY_ROWS;i++){
        for(int j = 0; j < ENEMY_COLS;j++){
            if(!enemies[i][j].alive) continue;
            enemies[i][j].alive = FALSE;
        }
    }
}
void enemies_reset(ENEMY  (*enemies)[ENEMY_COLS]){

    for(int i = 0; i < ENEMY_ROWS;i++){
        for(int j = 0; j < ENEMY_COLS;j++){
            enemies[i][j].alive = TRUE;
            enemies[i][j].x  = 0;
             enemies[i][j].y = 0;
        }
    }

    enemies_init(enemies);
}


void draw_enemies(ENEMY enemy_list[ENEMY_ROWS][ENEMY_COLS], float offset_x, float offset_y){

    (void)offset_x; (void)offset_y;

    if(!gameover){
        for(int y = 0; y < ENEMY_ROWS;y++){
            for(int x = 0; x < ENEMY_COLS;x++){
                if( enemy_list[y][x].alive){
                    int type = enemy_list[y][x].type == 1 ? SPR_ENEMY01 : SPR_ENEMY02;
                    ALLEGRO_BITMAP *enemy_spr = sprites[type];

                    al_draw_bitmap(enemy_spr, enemy_list[y][x].x,  enemy_list[y][x].y, 0);
                }

            }
        }

    }

    if(gameover > 0){
        al_draw_textf(debug_font, al_map_rgb_f(1.0,0,0), (int)al_get_display_width(display)/ 2,
         (int)al_get_display_height(display) / 2.0,0, "GAME OVER BABY!"
        );
    }
}

void enemies_draw_bullets(ENEMY enemies[ENEMY_ROWS][ENEMY_COLS]){

    for(int y = 0; y < ENEMY_ROWS;y++){
        for(int x = 0; x < ENEMY_COLS;x++){
            if(!enemies[y][x].alive) continue;

            for(int i = 0; i < MAX_BULLETS; i++){
                if(!enemies[y][x].bullets[i].alive) continue;

                BULLET *b = &enemies[y][x].bullets[i];
                //al_draw_filled_rectangle(b->x,b->y,b->x+8, b->y+8, al_map_rgb(255, 159, 67));
                al_draw_bitmap(sprites[SPR_ENEMYSHOT],b->x, b->y,0);
            }
        }
    }
}

void enemies_update_bullet(PLAYER *player, ENEMY (*enemies)[ENEMY_COLS]){


        for(int y = 0; y < ENEMY_ROWS; y++){
            for(int x = 0;x < ENEMY_COLS;x++){

                ENEMY *enemy = &enemies[y][x];

                    for(int i = 0; i < MAX_BULLETS-1;i++){
                        BULLET *bullet = &enemy->bullets[i];

                        if(bullet->alive){
                           int total = getCountBulletsAlive(enemy->bullets);

                           for(int i = 0; i < total; i++){
                               bullet->x += cos(bullet->angle * DEG2RAD) * difficulty[game_difficulty].shot_speed;
                               bullet->y += sin(bullet->angle * DEG2RAD) * difficulty[game_difficulty].shot_speed;
                           }

                           if(!g_demo_start && rect_collision(player->x, player->y,32,32, bullet->x, bullet->y, 8,8) && player->alive){

                                if(player->life > 0){

                                    switch(difficulty[game_difficulty].type){
                                        default:
                                        case DIFF_NOVICE:
                                            player->life -= 40;
                                            break;

                                        case DIFF_EASY:
                                            player->life -= 40;
                                            break;

                                        case DIFF_NORMAL:
                                            player->life -= 25;
                                            break;

                                        case DIFF_HARD:
                                            player->life -= 25;
                                            break;

                                        case DIFF_NIGHTMARE:
                                            player->life -= 25;
                                            break;
                                    }

                                    bullet->alive = FALSE;
                                    //particle_explosion(particles, (int)bullet->x, (int)bullet->y, 200,100, 200);

                                    play(SFX_EXPLOSION4);
                                }

                           }

                           if(bullet->x >= al_get_display_width(display)){
                               bullet->alive = FALSE;
                           }

                          if(bullet->x < 0){
                               bullet->alive = false;
                           }

                            if(bullet->y >= al_get_display_height(display)){
                               bullet->alive = false;
                           }

                          if(bullet->y < 0){
                               bullet->alive = false;
                           }


                        }
                    }
            }
        }


        if(enemy_shoot_time > 0){
            enemy_shoot_time--;
        }else {

            if(g_game_paused)
                return;

            int enemy_index_x =  game_rand_range(0, ENEMY_COLS-1);
            int enemy_index_y =  game_rand_range(0, ENEMY_ROWS-1);

            ENEMY *e = &enemies[enemy_index_y][enemy_index_x];

            if(!e->alive){
                return;
            }

                for(int i = 0; i < difficulty[game_difficulty].enemies_num_shoot; i++){
                    BULLET *b = create_shot(e->bullets, e->x, e->y, 1.0, 1.0, ITEM_ID_DEFAULT_CANNON);

                    if(b == NULL){
                        return;
                    }

                    b->color = al_map_rgb(255,0, 0);


                   double player_angle = angle_distance_rad(b->x, b->y, player->x, player->y); //atan2(dy, dx);
                   b->angle = player_angle * RAD2DEG;

                   b->x = e->x + cos(player_angle) * difficulty[game_difficulty].speed_multiplier;
                   b->y = e->y + sin(player_angle) * difficulty[game_difficulty].speed_multiplier;
                   play(SFX_ENEMY_SHOOT);

                }

                enemy_shoot_time =  difficulty[game_difficulty].enemy_shot_time;
        }


}


void enemies_set_down(ENEMY (*enemies)[ENEMY_COLS]){
        for(int y = 0; y < ENEMY_ROWS; y++){
            for(int x = 0;x < ENEMY_COLS;x++){
                enemies[y][x].y +=  TILE;
                line++;
            }
        }
}


void enemies_update(PLAYER *player,  ITEM *item_list, ENEMY (*enemies)[ENEMY_COLS]){


    int phase = WALK_TIME_DELAY_PHASE1;
    for(int y = 0; y < ENEMY_ROWS; y++){
        for(int x = 0;x < ENEMY_COLS;x++){
             if(enemies[y][x].alive) {
                 enemies[y][x].x +=  difficulty[game_difficulty].speed_multiplier * enemy_direction;
                 walk_time--;

                 if(walk_time <= 0){
                    play(SFX_WALK);


                    if(line > 100){
                        phase = WALK_TIME_DELAY_PHASE2;
                    }else if(line > 150){
                        phase = WALK_TIME_DELAY_PHASE3;
                    }else if(line > 200){
                        phase = WALK_TIME_DELAY_PHASE4;
                    }else if(line > 300){
                        phase = WALK_TIME_DELAY_PHASE5;
                    }else {
                        phase = WALK_TIME_DELAY_PHASE1;
                    }


                    walk_time = phase;
                 }

                 if(enemies[y][x].x + TILE > (al_get_display_width(display) - TILE - 1)){

                     enemies_set_down(enemies);
                     enemy_direction = -1;
                 }

                 if(enemies[y][x].x  < 0 ){
                     enemies_set_down(enemies);
                     enemy_direction = 1;
                 }


                 if(enemies[y][x].y > al_get_display_height(display) - TILE - 1){
                            do_gameover();
                            return;
                 }

                 if(enemies_bullet_collision(&enemies[y][x], player->bullets)){
                        enemies[y][x].alive = FALSE;
                        player->score += 100;
                        //score_add(score_list, 100, enemies[y][x].x, enemies[y][x].y, COLOR_RED);
                        int rand = game_rand_range(0,2);
                        int speed_index = 0;
                        speed_index = game_rand_range(0,3);
                        play_sound(rand,1.0,0.0, explosion_speed[speed_index], ALLEGRO_PLAYMODE_ONCE);


                        int drop_chance = game_rand_range(1,100);

                        if(drop_chance <= 99){


                            int item_drop_item = ITEM_ID_DOUBLE_CANNON;


                            int sounds[3] = {
                                SFX_POWERUP,
                                SFX_POWERUP2,
                                SFX_POWERUP3
                            };

                            //score_add(score_list, 150,  enemies[y][x].x,enemies[y][x].y-50, COLOR_WHITE);
                            player->score += 150;
                            //item_spawn(item_list, enemies[y][x].x,enemies[y][x].y, item_drop_item);
                            item_spawn_id(item_list, MAX_ITEM_LIST, player,   enemies[y][x].x,enemies[y][x].y, item_drop_item);
                            //assert(it == NULL);
                            //it->id = item_drop_item;
                            int rand_snd = game_rand_range(0,2);
                            play(sounds[rand_snd]);

                        }


                        particle_explosion(particles, enemies[y][x].x, enemies[y][x].y, 30, 50, 10, COLOR_ORANGE);
                        return;
                 }


                /*
                if(rect_collision(player.x,player.y, 32,32, enemies[y][x].x,enemies[y][x].y,32,32)){
                        if(!g_demo_start){
                            do_gameover();

                        }else{
                            new_game(FALSE);
                        }
                        return;
                }*/


             }
        }
    }

    return;
}


int enemies_count(ENEMY (*enemies)[ENEMY_COLS], int rows, int cols){
    int count = 0;
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols;x++){
            if(enemies[y][x].alive) count++;
        }
    }

    return count;
}

int  enemies_bullet_collision(ENEMY *enemy, BULLET *bullet){
    for(int i = 0; i < MAX_BULLETS; i++){

        if(rect_collision(enemy->x,enemy->y,32,32, bullet[i].x,bullet[i].y, 8,8) && bullet[i].alive){
            bullet[i].alive = FALSE;
            return TRUE;
        }
    }

    return FALSE;
}
