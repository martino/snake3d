/*
 * Snake 3d -- loop.c
 * 
 * Gestione del loop del programma
 *
 */

#include "loop.h"


/**
   qui bisogna gestire tutto quello che riguarda i vari "update" del gioco, quindi
   movimento del verme(velocita', direzione...) , mappa, collision detection
   

 */
void loop(){
  /* calcolo del framerate */
  programData.frame++;
  programData.time = glutGet(GLUT_ELAPSED_TIME);
  if(programData.time - programData.timebase > 1000){
    sprintf(programData.fps, "FPS: %.0f", programData.frame*1000.0/(programData.time-programData.timebase));
    programData.timebase = programData.time;
    programData.frame = 0;
  }


}
