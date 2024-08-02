#ifndef LINUX_WIN_HEADER
#define LINUX_WIN_HEADER


void window_init(void);
void window_display_create(int width, int height, int is_fullscreen, int enable_vsync, const char *window_caption);
void window_unload(void);

#endif // LINUX_WIN_HEADER
