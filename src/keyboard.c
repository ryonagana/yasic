#include "keyboard.h"

static int game_keys[255];



void Keyboard_PollKeyUp(SDL_Event *e){
    game_keys[e->key.keysym.scancode] = 1;
}

void Keyboard_PollKeyDown(SDL_Event *e){
    game_keys[e->key.keysym.scancode] = 0;
}

void Keyboard_Update(void){
    memset(game_keys, 0x0, sizeof(game_keys));
}

int *Keyboard_getKeys(void){
    return game_keys;
}

int Keyboard_isPressed(int key){
    return game_keys[key];
}
