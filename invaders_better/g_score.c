#include "g_score.h"
#include "main.h"
#include "g_utils.h"

TEXT* g_score_list = NULL;
static TEXT s_score_list[MAX_SCORE] = {0};

void
score_init(void)
{
  g_score_list = s_score_list;
}
  
void
score_draw_text(void)
{

    for (int i= 0; i < 10; i++){
        if(s_score_list[i].ttl > 0){
            unsigned char a,r,g,b;
            al_unmap_rgba(s_score_list[i].color,&r,&g,&b,&a);
            int ttl = s_score_list[i].ttl;
            ALLEGRO_COLOR c = al_premul_rgba(r*ttl,g*ttl,b*ttl,a*ttl);
            al_draw_textf(font_list[FONT_PIXEL_BIG], c, s_score_list[i].x, s_score_list[i].y,0, "%s", s_score_list[i].text);
        }
    }
}

TEXT* score_free(TEXT score[MAX_SCORE]){
    int c = 0;
    for(c = 0;c < 10; c++){
        if(score[c].ttl <= 0){
            break;
        }
    }
    return &score[c];
}

void score_add(TEXT *score, const int num, const float x, const float y, ALLEGRO_COLOR color){

    TEXT *s = score_free(score);

    
    if(NULL == s){
      fprintf(stderr, "%s:%d - score_free returned NULL", __FILE__, __LINE__ );
      return;
    }

    s->x = x;
    s->y = y;
    s->ttl = 80;
    s->shadow = 0;
    s->color = color;

    if(num < 0){
         snprintf(s->text, 255, "%02d", num);
         return;
    }

    snprintf(s->text, 255, "+%02d", num);
    return;
}

void score_update_text(void){
    for (int i= 0; i < 10; i++){
        if(s_score_list[i].ttl > 0){
	  s_score_list[i].y += -2.0;
            s_score_list[i].ttl--;
        }

    }
}
