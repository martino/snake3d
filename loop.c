/*
 * Snake 3d -- loop.c
 * 
 * Gestione del loop del programma
 *
 */

#include "loop.h"


/*
 *  qui bisogna gestire tutto quello che riguarda i vari "update" del gioco, quindi
 * movimento del verme(velocita', direzione...) , mappa, collision detection
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

  /* movimento verticale --> freccia up e down */
  if((worldData.kup)||(worldData.kdown)){
    if(worldData.yStatus == worldData.nextYstatus){ /* sono fermo */
      switch(worldData.yStatus){
      case 0: /*sono orizzontale*/
	if(worldData.kup){
	  worldData.nextYstatus = 1;
	  worldData.nextAngleX -= 90.0f;
	  worldData.angleX = worldData.angleMX;
	}else{
	  worldData.nextYstatus = 2;
	  worldData.nextAngleX += 90.0f;
	  worldData.angleX = worldData.angleMX;
	}
	break;
      case 1: /*sono in alto*/
	if(worldData.kdown){
	  worldData.nextYstatus = 0;
	  worldData.nextAngleX += 90.0f;
	  worldData.angleX = worldData.angleMX;
	}
	break;
      case 2: /*sono in basso*/\
	if(worldData.kup){
	  worldData.nextYstatus = 0;
	  worldData.nextAngleX -= 90.0f;
	  worldData.angleX = worldData.angleMX;
	}
	break;
      }
    }else{ /* caso in cui sono in movimento*/
      if(worldData.kup){
	if(worldData.nextYstatus == 2){ /*sto andando verso il basso*/
	  worldData.nextYstatus = 0;
	  worldData.yStatus = 2;
	  worldData.nextAngleX -= 90.0f;
	}
      }else{
	if(worldData.nextYstatus == 1){ /*sto andando verso l'alto*/
	  worldData.nextYstatus = 0;
	  worldData.yStatus = 1;
	  worldData.nextAngleX += 90.0f;
	}
      }
    }
    worldData.kup = 0;
    worldData.kdown = 0;
  }


  /* movimento laterale --> freccia sx e dx */
  if((worldData.kleft)||(worldData.kright)){
    if(worldData.xStatus == 0){ /* sto andando dritto */
      if(worldData.kleft){
	worldData.nextXstatus = 1; /* vado a sinistra */
	worldData.nextAngleY = worldData.angleY - 90.0f ;
	worldData.angleY = worldData.angleMY;
      }
      else{
	worldData.nextXstatus = 2; /* vado a destra */
	worldData.nextAngleY = worldData.angleY + 90.0f ;
	worldData.angleY = worldData.angleMY;
      }
      worldData.xStatus = 1;
    }else{
      switch(worldData.nextXstatus){
      case 1: /* sto andando verso sinistra */
	if(worldData.kright){
	  worldData.nextAngleY = worldData.nextAngleY + 90.0f;
	  worldData.nextXstatus = 2;
	  worldData.angleY = worldData.angleMY;
	}
	break;
      case 2: /* sto andando verso destra */
	if(worldData.kleft){
	  worldData.nextAngleY = worldData.nextAngleY - 90.0f;
	  worldData.nextXstatus = 1;
	  worldData.angleY = worldData.angleMY;
	}
	break;
      }
    }
    worldData.kleft  = 0;
    worldData.kright = 0;
  }
 
  

  /* modifico l'angolo X */
  if(worldData.yStatus != worldData.nextYstatus){
    switch(worldData.nextYstatus){
    case 0:  /* devo arrivare sul piano */
      switch(worldData.yStatus){
      case 1:
	worldData.angleMX += RSPEED;
	if(worldData.angleMX > worldData.nextAngleX){
	  worldData.angleMX = worldData.nextAngleX;
	  worldData.angleX = worldData.nextAngleX;
	  worldData.yStatus = 0;
	}
	break;
      case 2:
	worldData.angleMX -= RSPEED;
	if(worldData.angleMX < worldData.nextAngleX){
	  worldData.angleMX = worldData.nextAngleX;
	  worldData.angleX = worldData.nextAngleX;
	  worldData.yStatus = 0;
	}
	break;
      }
      break;
    case 1:
      worldData.angleMX -= RSPEED;
      if(worldData.angleMX < worldData.nextAngleX){
	worldData.angleMX = worldData.nextAngleX;
	worldData.angleX = worldData.nextAngleX;
	worldData.yStatus = 1;
      }
      break;
    case 2:
      worldData.angleMX += RSPEED;
      if(worldData.angleMX > worldData.nextAngleX){
	worldData.angleMX = worldData.nextAngleX;
	worldData.angleX = worldData.nextAngleX;
	worldData.yStatus = 2;
      }
      break;
    }
  }

  /* modifico l'angolo Y */
  if(worldData.xStatus){ 
    switch(worldData.nextXstatus){
    case 1: /* devo muovermi a sinistra */
      worldData.angleMY -= RSPEED;
      if(worldData.angleMY < worldData.nextAngleY){
	worldData.angleMY = worldData.nextAngleY;
	worldData.angleY = worldData.nextAngleY;
	worldData.xStatus = 0;
	worldData.nextXstatus = 0;
      }
      break;
    case 2: /* devo muovermi a destra */
      worldData.angleMY += RSPEED;
      if(worldData.angleMY > worldData.nextAngleY){
	worldData.angleMY = worldData.nextAngleY;
	worldData.angleY = worldData.nextAngleY;
	worldData.xStatus = 0;
	worldData.nextXstatus = 0;
      }
      break;
    }
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
  }
}
