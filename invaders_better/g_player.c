#include "g_player.h"
#include <math.h>
#include "g_render.h"
#include "g_sprites.h"
#include "g_sound.h"
#include "main.h"
void player_init(PLAYER *p){

    p->x = al_get_display_width(display) / 2 - 32;
    p->y = al_get_display_height(display) / 2 - 32;

    p->x = al_get_display_width(display) / 2 - 32;
    p->y = al_get_display_height(display) - 50;
    p->direction = 1;
    p->alive = TRUE;
    p->shot_time = 0;
    p->life = 100.0;
    p->lives = 1;
    p->shot_time = 25;

    for(int i = 0; i < ITEM_ID_COUNT;i++){
        p->items[i].active = false;
    }

    item_add_player(p, ITEM_ID_DEFAULT_CANNON);

}

void player_draw(float x, float y){

    al_draw_filled_triangle(x,y,x+32,y+32,x,y+32, al_map_rgb_f(1.0,0,0));


}

void player_update_shot(PLAYER *player){
    for(int i = 0; i < MAX_BULLETS; i++){
        if(player->bullets[i].alive && player->bullets[i].ttl > 0){
           player->bullets[i].y -= 8.0;

           if( player->bullets[i].y < -16 || player->bullets[i].y > al_get_display_height(display) + 16 ||
               player->bullets[i].x < -16 || player->bullets[i].x > al_get_display_width(display) + 16){
                player->bullets[i].alive = false;
           }

            if(player->item_use->info.id == ITEM_ID_DOUBLE_CANNON){

                for(int bullet_index = 0; bullet_index < player->item_use->info.shots;bullet_index++){
                    player->bullets[bullet_index%1].x += sin(150 * DEG2RAD);
                    player->bullets[bullet_index%2].y   +=  cos(30 * DEG2RAD);
                }
            }

        /*
           if(g_spaceship_entity->alive && g_spaceship_entity && rect_collision(player.bullets[i].x, player.bullets[i].y, 8,8, g_spaceship_entity->x, g_spaceship_entity->y, 32,32)){
                g_spaceship_entity->alive = FALSE;
                g_ship_active = FALSE;
                score_add(score_list, 1000,  g_spaceship_entity->x, g_spaceship_entity->y, COLOR_WHITE);
                player->score += 1000;
                al_stop_samples();
           }*/

        }
    }

    if(player->shot_time > 0){
        player->shoot = TRUE;
    }else if(player->shot_time < 0){
        return;
    }else {
      player->shoot = FALSE;
    }

    player->shot_time--;
    return;

}

void player_draw_shot(PLAYER *player){

    for(int i = 0; i < MAX_BULLETS; i++){
        if(player->bullets[i].alive){

            al_draw_line( player->bullets[i].x+4, player->y+4, player->bullets[i].x + 4, player->bullets[i].y, al_map_rgba_f(0.5,0.5,0.5,0.2),1);
            al_draw_bitmap(sprites[SPR_PLAYERSHOT], player->bullets[i].x, player->bullets[i].y,0);
        }
    }
}


void player_shoot(PLAYER *player){
    ITEM *item = player->item_use;

    if(!(item->info.flags & ITEMINFO_FLAG_INFINITE_AMMO)){
        if(player->ammo > 0){
            player->ammo--;
        }else {
            player->item_use = &player->items[0];
        }
    }

    switch(item->info.id){
    default:
    case ITEM_ID_DEFAULT_CANNON:
    {
        create_shot(player->bullets, player->x, player->y, 0,1.0);
        play(SFX_LASER);
        player->shot_time = item->info.delay;
    }
        break;

    case ITEM_ID_DOUBLE_CANNON:
        {
            for(int i = 0; i < 2; i++){
                create_shot(player->bullets, player->x + cos(45.0 * DEG2RAD) , player->y + cos(-45.0 * DEG2RAD),0.0,1.0);
            }

            play(SFX_LASER);
            player->shot_time = item->info.delay;
        }
        break;
    }
}

void player_update(PLAYER *player){


    player->x = 2.0 * -g_mouse.x;
    player->x = 2.0 * g_mouse.y;

     if(player->x > 0 && player->x < al_get_display_width(display)-1){
                    player->x += 2.0;
    }

    if(player->score <= 0 )
        player->score = 0;


    if((g_mouse.buttons & 1) && !player->shoot && !gameover  ){
        player_shoot(player);
    }


    if(player->alive){
        if(player_keys[ALLEGRO_KEY_A] || player_keys[ALLEGRO_KEY_LEFT]){
            player->x -= 2.0;
            player->keypressed[0] = TRUE;
        }else {
            player->keypressed[0] = FALSE;
        }

        if(player_keys[ALLEGRO_KEY_D] || player_keys[ALLEGRO_KEY_RIGHT]){
            player->x +=  2.0;
            player->keypressed[1] = TRUE;
        }else {
            player->keypressed[1] = FALSE;
        }

        if(player_keys[ALLEGRO_KEY_SPACE] && !player->shoot && !gameover){
            player_shoot(player);
        }

    }

}
