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
  GLuint moveFrame = 0;

  /* calcolo del framerate */
  programData.frame++;
  programData.time = glutGet(GLUT_ELAPSED_TIME);
  if(programData.time - programData.timebase > 1000){
    sprintf(programData.fps, "FPS: %.0f", programData.frame*1000.0/(programData.time-programData.timebase));
    programData.timebase = programData.time;
    programData.frame = 0;
  }

  /* se sono nel menu' non devo muovere nulla */
  /* movimento della telecamera a scatti */
  if(programData.time - programData.timerender > 500){
    moveFrame = 1;
    programData.timerender = programData.time + ((programData.time - programData.timerender)-500);
  }

  if(moveFrame){
    /* movimento sul piano Y */
    /*    worldData.y += (float)sin(worldData.angleX * PIOVER180) * 1.05f;
    worldData.z += (float)cos(worldData.angleX * PIOVER180) * 1.05f;
    */
    /* movimento sul piano X */
    
    worldData.x -= (float)sin(worldData.angleY * PIOVER180) * 1.05f;
    worldData.z -= (float)cos(worldData.angleY * PIOVER180) * 1.05f;
    
/*     fprintf(stderr, "%f %f %f %f\n", worldData.x, worldData.y, worldData.z, (float)(cos(0))); */
  }
}
