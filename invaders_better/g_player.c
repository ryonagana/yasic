#include "g_player.h"
#include <math.h>
#include "g_render.h"
#include "g_sprites.h"
#include "g_sound.h"
#include "g_item.h"
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
    memset(p->items,0, sizeof(p->items));
    item_assign_to_player_id(p, ITEM_ID_DEFAULT_CANNON);

}

void player_draw(float x, float y){

    al_draw_filled_triangle(x,y,x+32,y+32,x,y+32, al_map_rgb_f(1.0,0,0));


}

void player_update_shot(PLAYER *player){


    for(int i = 0; i < MAX_BULLETS; i++){
        if(player->bullets[i].alive && player->bullets[i].ttl > 0){


           for(int i = 0; i < ITEM_ID_COUNT; i++){

                if(player->items[i].id == ITEM_ID_DOUBLE_CANNON){

                    for(int bullet_index = 0; bullet_index < player->items[i].shot_num;bullet_index++){
                        player->bullets[bullet_index%1].x += sin(150 * DEG2RAD);
                        player->bullets[bullet_index%2].y   +=  cos(30 * DEG2RAD);
                    }
                }
          }
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


    int bullet_id = player->item_use->id;

    switch(player->item_use->id)
    {
        default:
        case ITEM_ID_DEFAULT_CANNON:
        {
            create_shot(player->bullets, player->x, player->y, 0,-8.0, bullet_id);
            player->shot_time = player->item_use->shot_time;
            play(SFX_LASER);
        }
        break;

        case ITEM_ID_DOUBLE_CANNON:
        {
            for(int i = 0; i < player->item_use->shot_num;i++){
                if(i%2){
                    create_shot_angle(player->bullets, player->x, player->y, 0,-8.0, 45, bullet_id );
                    continue;
                }

                create_shot_angle(player->bullets, player->x, player->y, 0,-8.0, 5, bullet_id);
            }
            player->shot_time = player->item_use->shot_time;
            play(SFX_LASER);
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
