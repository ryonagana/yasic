#include "display.h"
#include "invaders.h"

int main(void){
    if(!Dsp_InitVideo()){
        Dsp_ShutdownVideo();
        exit(1);
    }


    Dsp_CreateDisplay(800,600,0,1,"Invaders 2");
    Dsp_RegisterEvents();

    Invaders_Start();
    Invaders_Loop();
    Invaders_Shutdown();

    Dsp_ShutdownVideo();

    return 0;
}
