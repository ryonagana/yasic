#include "bullet.h"
#include <stdio.h>
#include "display.h"

void Bullet_Init(TBULLETS *bullet_root, int size){
    if(!bullet_root)
        return;
    if(size <= 0)
        return;

    bullet_root->length = size > MAX_BULLETS ? MAX_BULLETS : size;
    bullet_root->reset = 0;
    memset((void*)bullet_root->bullets, 0, size);
}

static void S_Bullet_Default_Update(struct BULLET *b, int bullet_length){

    if(Bullet_Collides_Bottom(b) ||
       Bullet_Collides_Top(b) ||
       Bullet_Collides_Left(b) ||
       Bullet_Collides_Right(b)
       ){
            b->alive = 0;
       }


       b->x += b->vx;
       b->y += b->vy;

}

static void S_Bullet_Default_Render(struct BULLET *b, int bullet_length){

}


void Bullet_Update(TBULLETS *bullet_root){

    for(int i = 0; i < bullet_root->length;i++){
        BULLET *b = &bullet_root->bullets[i];

        if(b->alive){

            if(bullet_root->update_callback){
                bullet_root->update_callback(b, bullet_root->length);
                return;
            }

            S_Bullet_Default_Update(b, bullet_root->length);


        }

    }

}

void Bullet_Draw(TBULLETS *bullet_root, ALLEGRO_BITMAP *bmp){
    for(int i = 0; i < bullet_root->length;i++){

            if(bullet_root->bullets[i].alive){
                BULLET *b = &bullet_root->bullets[i];

            if(bullet_root->render_callback){
                bullet_root->render_callback(b, bullet_root->length, bmp);
                return;
            }

            if(bmp){
                al_draw_bitmap(bmp, b->x, b->y,0);
            }else {
                al_draw_filled_circle(b->x,b->y,5,al_map_rgb(255,0,255));
            }
        }
    }
}

BULLET* Bullet_FindFree(TBULLETS *bullet_root){
   BULLET *b = bullet_root->bullets;
   int count = 0;
   while(b[count].alive && count < bullet_root->length){
        count++;
   }
   if(count == bullet_root->length){
        return NULL;
   }

   return &b[count];
}


int Bullet_Collides_Top(BULLET *b){
    if(b->y < 0){
        return 1;
    }

    return 0;
}
int Bullet_Collides_Bottom(BULLET *b){
    if(b->y > Dsp_GetWindowHeight()-1){
        return 1;
    }

    return 0;
}
int Bullet_Collides_Left(BULLET *b){
    if(b->x < 0){
        return 1;
    }

    return 0;
}
int Bullet_Collides_Right(BULLET *b){
    if(b->x > Dsp_GetWindowWidth()-1){
        return 1;
    }
    return 0;
}

void Bullet_SetFlag(BULLET *b, int flags){
    b->flags = flags;
}

int Bullet_GetFlag(BULLET *b) {
    return b->flags;
}

void Bullet_SetGravity(BULLET *b, float gravity){
    b->gravity = gravity;
}

void Bullet_SetUpdateCallback(TBULLETS *bullets, void (*update_callback)(struct BULLET *b, int bullet_length)){
    if(update_callback){
        bullets->update_callback = update_callback;
    }
}
void Bullet_SetDrawCallback(TBULLETS *bullets,   void (*render_callback)(struct BULLET *b, int bullet_length, ALLEGRO_BITMAP *bmp)){
    if(render_callback){
        bullets->render_callback = render_callback;
    }
}
