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
  if(programData.menu == 1)
    return;

  /* movimento della telecamera a scatti */
  if(programData.time - programData.timerender > 500){
    moveFrame = 1;
    programData.timerender = programData.time + ((programData.time - programData.timerender)-500);
  }

  /* controllo l'input dell'utente */
  if(worldData.kup){
    switch(worldData.yStatus){ /* controllo lo status attuale */
    case 0:
      if(worldData.nextYstatus == 2) /* controllo se sto andando verso il basso*/
	worldData.nextYstatus = 0;  /* in caso torno al piano */
      else
	worldData.nextYstatus = 1; /* altrimenti vado in alto*/
      break;
    case 2: /* se sono in basso in ogni caso vado sul piano */
      worldData.nextYstatus = 0;
      break;	
    }
    worldData.kup = 0;
  }

  if(worldData.kdown){
    switch(worldData.yStatus){ /* controllo lo status attuale */
    case 0:
      if(worldData.nextYstatus == 1) /* controllo se sto andando verso l'alto*/
	worldData.nextYstatus = 0;  /* in caso torno al piano */
      else
	worldData.nextYstatus = 2; /* altrimenti vado in basso*/
      break;
    case 1: /* se sono in alto in ogni caso vado sul piano */
      worldData.nextYstatus = 0;
      break;	
    }
    worldData.kdown = 0;
  }

  if(worldData.kleft){
    switch(worldData.xStatus){
    case 0:
      if(worldData.nextXstatus == 2) /* se sto andando verso destra */
	worldData.nextXstatus = 0; /* torno in centro */
      else
	worldData.nextXstatus = 1; /* altrimenti vado a sinistra */
      break;
    case 2: /* se sono a destra torno in centro */
      worldData.nextXstatus = 0;
      break;
    case 1:
      worldData.nextXstatus = 1;
      worldData.change = 1;
      break;
    }
    worldData.kleft = 0;
  }

  if(worldData.kright){
    switch(worldData.xStatus){
    case 0:
      if(worldData.nextXstatus == 1) /* se sto andando verso sinistra */
	worldData.nextXstatus = 0; /* torno in centro */
      else
	worldData.nextXstatus = 2; /* altrimenti vado a destra*/
      break;
    case 1: /* se sono sinistra  torno in centro */
      worldData.nextXstatus = 0;
      break;
    case 2: /* se sono destra giro ancora */
      worldData.nextXstatus = 2;
      worldData.change = 1;
      break;
    }
    worldData.kright = 0;
  }

  /* bisogna controllare gli angoli */

  if((worldData.xStatus != worldData.nextXstatus)||worldData.change){
    switch(worldData.nextXstatus){
    case 0:
      if(worldData.xStatus == 1)
	/* worldData.angleY += 90.0f;*/
	worldData.nextAngleY = worldData.angleY + 90.0f;
      if(worldData.xStatus == 2)
	/* worldData.angleY -= 90.0f;*/
	worldData.nextAngleY = worldData.angleY - 90.0f;
      worldData.xStatus = 0;
      break;
    case 1:
      /* worldData.angleY -= 90.0f;*/
      worldData.nextAngleY = worldData.angleY - 90.0f;
      /* qui devo calcolare l'angolo a cui dovro' arrivare */
      worldData.xStatus = 1;
      break;
    case 2:
      /* worldData.angleY += 90.0f;*/
      worldData.nextAngleY = worldData.angleY + 90.0f;
      worldData.xStatus = 2;
      break;
    }
    worldData.lastAngleY = worldData.angleY;
    worldData.change = 0;
  }
  
  
  if(worldData.yStatus != worldData.nextYstatus){
    switch(worldData.nextYstatus){
    case 0:
      if(worldData.yStatus == 1)
	/* worldData.angleX += 90.0f; */
	worldData.nextAngleX = worldData.angleX + 90.0f;
      if(worldData.yStatus == 2)
	/* worldData.angleX -= 90.0f; */
	worldData.nextAngleX = worldData.angleX - 90.0f;
      worldData.yStatus = 0;
      break;
    case 1:
      /*worldData.angleX -= 90.0f;*/
      worldData.nextAngleX = worldData.angleX - 90.0f;
      worldData.yStatus = 1;
      break;
    case 2:
      /* worldData.angleX += 90.0f; */
      worldData.nextAngleX = worldData.angleX + 90.0f;
      worldData.yStatus = 2;
      break;
    }
    worldData.lastAngleX = worldData.angleX;
  }
 
  /* qui devo vedere se sono nell'angolo corretto altrimenti mi devo "muovere" */

  if(){

  }
  

  /* controllo se sono in posizione tale da muovermi */
  if(moveFrame){
    if(worldData.yStatus == 0){
      /* movimento sul piano X */
      worldData.x -= (float)sin(worldData.angleY * PIOVER180) * 3.5f;
      worldData.z -= (float)cos(worldData.angleY * PIOVER180) * 3.5f;
    }else{
    /* movimento sul piano Y */
      worldData.y += (float)sin(worldData.angleX * PIOVER180) * 3.05f;
      worldData.z += (float)cos(worldData.angleX * PIOVER180) * 3.05f;
    }
    
/*     fprintf(stderr, "%f %f %f %f\n", worldData.x, worldData.y, worldData.z, (float)(cos(0))); */
  }
}
