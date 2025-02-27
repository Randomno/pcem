#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

void leave_fullscreen();
int getfile(void* hwnd, char *f, char *fn);
int getsfile(void* hwnd, char *f, char *fn, char *dir, char *ext);
int getfilewithcaption(void* hwnd, char *f, char *fn, char *caption);
void screenshot_taken(unsigned char* rgb, int width, int height);

#ifdef __cplusplus
}
#endif

extern char openfilestring[260];

extern int pause;

extern int take_screenshot;

extern SDL_mutex* syncMutex;
extern SDL_cond* syncCond;
extern int syncRender;
