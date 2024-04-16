#include "g_player.h"
#include <math.h>
#include "g_render.h"
#include "g_sprites.h"
#include "g_sound.h"
#include "g_item.h"
#include "main.h"

static ITEM s_default_cannon;
static ITEM s_double_cannon;



static void s_player_init_items(ITEM *itemlist){

  for(int i = 0; i < ITEM_ID_COUNT; i++){
    itemlist[i].id = i;
    itemlist[i].active = FALSE;
    itemlist[i].shot_time  = 60;
    itemlist[i].shot_count = 1;
  }
  
}


void player_init(PLAYER *p){


    load_item_config("assets//cfg/default_cannon.cfg", &s_default_cannon);
    load_item_config("assets//cfg/double_cannon.cfg", &s_double_cannon);
  
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
    s_player_init_items(p->items);
    s_player_init_items(p->weapons);
    p->item_index = 0;
    //item_give_to_player_force(p,0,ITEM_ID_DEFAULT_CANNON);
    //item_assign_to_player_id(p, ITEM_ID_DEFAULT_CANNON);
    
    memcpy(&p->items[0], &s_default_cannon, sizeof(ITEM));
    memcpy(&p->items[1], &s_double_cannon, sizeof(ITEM));
    p->items[0].active = TRUE;
    p->items[1].active = TRUE;
     
}

void player_draw(float x, float y){

    al_draw_filled_triangle(x,y,x+32,y+32,x,y+32, al_map_rgb_f(1.0,0,0));


}

void player_update_shot(PLAYER *player){

  for(int i = 0; i < MAX_BULLETS;i++){
    if(!player->bullets[i].alive || player->bullets[i].ttl > 0){

      
      player->bullets[i].x += cos(player->bullets[i].angle * DEG2RAD) +  player->bullets[i].vx;
	player->bullets[i].y += sin(player->bullets[i].angle * DEG2RAD) + player->bullets[i].vy;
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
#ifdef DEBUG
    player_show_debug(player);
#endif
}


void player_shoot(PLAYER *player){

  ITEM* weapon = &player->items[player->item_index];

  

  if(!weapon->active) return;
    /* if(!(weapon->flags & ITEMINFO_FLAG_WEAP)) continue; */

  player->shot_time = weapon->shot_time;
  player->shot_count = weapon->shot_count;
  player->ammo = weapon->ammo;
  
  switch(weapon->id){
      case ITEM_ID_DEFAULT_CANNON:
	{
	    create_shot(player->bullets, player->x , player->y, 0, -4.0f, weapon->id);

	}
	break;

      case ITEM_ID_DOUBLE_CANNON:
        {
	 
	  for(int i = 0; i < weapon->shot_count; i++){
	    float ang = i==0 ? 50.0f : 140.0;
	    create_shot_angle(player->bullets, player->x , player->y, 0, -2.0f, ang, weapon->id);
	  }
        }
        break;
    }
  
}

int player_select_item_slot(PLAYER *p, int slot){

  UNUSED(p); UNUSED(slot);
  return 0;
  /* ITEM *item, *itemlist; */

  /* item = &p->items[slot]; */
  /* itemlist = item_get_free_slot(); */
  
  /* if(NULL == item){ */
  /*   fprintf(stderr,"%s:%d - Invalid Slot %d (returned NULL)", __FILE__, __LINE__, slot); */
  /*   return -1; */
  /* } */

  /* for(int i = 0; i < MAX_ITEM_LIST;i++){ */
  /*   if(itemlist[i].id == item->id){ */
  /*     p->shot_time = itemlist[i].shot_time; */
  /*     p->shot_count  = itemlist[i].shot_num; */
  /*     p->ammo      = itemlist[i].ammo; */
  /*   } */
  /* } */
  
  /* item->active = TRUE; */

  /* return 0; */
 
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

#ifdef DEBUG
    if(player_keys[ALLEGRO_KEY_1]){
      player->item_index = 0;
    }

    if(player_keys[ALLEGRO_KEY_2]){
      player->item_index = 1;
    }
#endif


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


void player_show_debug(PLAYER *p){


  

   al_draw_multiline_textf(debug_font, al_map_rgb_f(1.0,0,0), 0,450, 300, 20,0,
			   
			   "ITEM 0: %s\n"
			   "ITEM 1: %s\n"
			   "ITEM 2: %s\n"
			   "Bullets Alive: %d\n"
			   "Bullets Dead: %d\n",
			   p->items[0].active ? "ENABLED":"DISABLED",
			   p->items[1].active ? "ENABLED":"DISABLED",
			   p->items[2].active ? "ENABLED":"DISABLED",
			   getCountBulletsAlive(p->bullets),
			   MAX_BULLETS - getCountBulletsAlive(p->bullets)
			   
			   
			 
   );
   return;
}
