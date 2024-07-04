#include "display.h"
#include "sound.h"
#include "invaders.h"

int main(int argc, char *argv[]){
    if(!Dsp_InitVideo()){
        Dsp_ShutdownVideo();
        exit(1);
    }

    Snd_Init(8);
    Dsp_CreateDisplay(800,600,0,0,"Invaders 2");
    Dsp_CreateScreenBuffer(g_display.width,g_display.height);
    Dsp_RegisterEvents();

    Invaders_Start();
    Invaders_Loop();
    Invaders_Shutdown();

    Snd_End();
    Dsp_ShutdownVideo();

    return 0;
}
