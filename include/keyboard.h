#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL.h>

#define KEY_MOVE_LEFT  0
#define KEY_MOVE_RIGHT 1
#define KEY_MOVE_SHOOT 2

void Keyboard_PollKeyUp(SDL_Event *e);
void Keyboard_PollKeyDown(SDL_Event *e);
int *Keyboard_getKeys(void);
int Keyboard_isPressed(int key);
#endif // KEYBOARD_H
