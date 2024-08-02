#ifndef UTILS_HEADER
#define UTILS_HEADER


#define UNUSED(x) ((void)x)

#define TRUE (1ul)
#define FALSE (0ul)


#define KEY_PRESSED  0x02
#define KEY_RELEASED 0x04


#define RAD2DEG 57.29578049
#define DEG2RAD 0.017453292


#define GRAVITY 0.8

extern int keys[255];
int KeyPressed(int key);
int KeyReleased(int key);

#endif // UTILS_HEADER
