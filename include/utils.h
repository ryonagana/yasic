#ifndef UTILS_HEADER
#define UTILS_HEADER


#define UNUSED(x) ((void)x)

#define TRUE (1ul)
#define FALSE (0ul)


#define KEY_PRESSED  0x02
#define KEY_RELEASED 0x04

extern int keys[255];
int KeyPressed(int key);
int KeyReleased(int key);

#endif // UTILS_HEADER
