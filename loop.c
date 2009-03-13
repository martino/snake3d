/*
 * Snake 3d -- loop.c
 * 
 * Gestione del loop del programma
 *
 */

#include "loop.h"

/*
 * Funzione che controlla se il verme sta uscendo dai confini di gioco
 */
int checkWall(float xy, float z, int piano){
  switch(piano){
  case 1: /* controllo il piano X */
    if((xy >=  WORLDIM)) return 0;
    if((xy <= -WORLDIM)) return 0;
    break;
  case 2: /* controllo il piano Y */
    if((xy >=  WORLDIM)) return 0;
    if((xy <= -WORLDIM)) return 0;
    break;
  default:
    break;
  }

  /* controllo il piano Z */
  if( z <= -WORLDIM) return 0;
  if( z >=  WORLDIM) return 0;
  return 1;
}




/*
 * Collision detection tra due sfere
 *   x-y-z1 e' l'oggetto in movimento
 *
 */
int sCollisionDetection(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2){
  GLuint x=0,y=0,z=0;
  GLfloat dim = DIA;
  if((x1+dim > x2-dim)&&(x1-dim < x2+dim)) x=1; 
  if((y1+dim > y2-dim)&&(y1-dim < y2+dim)) y=1; 
  if((z1-dim < z2+dim)&&(z1+dim > z2-dim)) z=1; 
  
  if(x&&y&&z)
    return 1;
  else 
    return 0;
}

/*
 *  qui bisogna gestire tutto quello che riguarda i vari "update" del gioco, quindi
 * movimento del verme(velocita', direzione...) , mappa, collision detection
 */
void loop(){
  GLuint moveFrame = 0;
  GLuint delay = 0;

  /* calcolo del framerate */
  programData.frame++;
  programData.time = glutGet(GLUT_ELAPSED_TIME);
  if(programData.timerender == 0) programData.timerender = programData.time;
  if(programData.time - programData.timebase > 1000){
    sprintf(programData.fps, "FPS: %.0f", programData.frame*1000.0/(programData.time-programData.timebase));
    programData.timebase = programData.time;
    programData.frame = 0;
  }

  /* controllo lo stato della partita */
  switch(programData.gameStatus){
  case 0: /* ho perso */
    return;
    break;
  case 1: /* sto giocando */
    /* continuo*/
    break;
  case 2: /* ho vinto */
    return;
    break;
  default:
    break;
  }

  /* se sono nel menu' non devo muovere nulla */
  if(programData.menu == 1)
    return;
  

  /* movimento della telecamera a scatti */
  if(programData.time - programData.timerender > programData.timeFrame){
    moveFrame = 1;
    delay = programData.time - (programData.timerender + programData.timeFrame);    
    programData.timerender = programData.time + delay ;
  }

  /* controllo l'input dell'utente */

  /* movimento verticale --> freccia up e down */
  if((worldData.kup)||(worldData.kdown)){
    if(worldData.yStatus == worldData.nextYstatus){ /* sono fermo */
      switch(worldData.yStatus){
      case 0: /*sono orizzontale*/
	if(worldData.kup){
	  worldData.nextYstatus = 1;
	  worldData.nextAngleX  = worldData.angleX - 90.0f;
	  worldData.nextAngleMX = worldData.angleMX -90.0f;
	  worldData.angleX = worldData.angleMX;
	}else{
	  worldData.nextYstatus = 2;
	  worldData.nextAngleX = worldData.angleX + 90.0f;
	  worldData.nextAngleMX = worldData.angleMX + 90.0f;
	  worldData.angleX = worldData.angleMX;
	}
	break;
      case 1: /*sono in alto*/
	if(worldData.kdown){
	  worldData.nextYstatus = 0;
	  worldData.nextAngleX = worldData.angleX + 90.0f;
	  worldData.nextAngleMX = worldData.angleMX + 90.0f;
	  worldData.angleX = worldData.angleMX;
	}
	break;
      case 2: /*sono in basso*/\
	if(worldData.kup){
	  worldData.nextYstatus = 0;
	  worldData.nextAngleX = worldData.angleX - 90.0f;
	  worldData.nextAngleMX = worldData.angleMX -90.0f;
	  worldData.angleX = worldData.angleMX;
	}
	break;
      default:
	break;
      }
    }else{ /* caso in cui sono in movimento*/

      if(worldData.kup){
	if(worldData.nextYstatus == 2){ /*sto andando verso il basso*/
	  worldData.nextYstatus = 0;
	  worldData.yStatus = 2;
	  worldData.angleX = worldData.nextAngleX;
	  /* qui bisogna muovere il verme */
	  worldData.nextAngleX = worldData.angleX - 90.0f;
	  worldData.nextAngleMX = worldData.angleMX - 90.0f;

	}
      }else{
	if(worldData.nextYstatus == 1){ /*sto andando verso l'alto*/
	  worldData.nextYstatus = 0;
	  worldData.yStatus = 1;
 	  worldData.angleX = worldData.nextAngleX;
	  /* qui bisogna muovere il verme */
	  worldData.nextAngleX = worldData.angleX  + 90.0f;
	  worldData.nextAngleMX = worldData.angleMX + 90.0f;
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
	/*-----*/
	worldData.nextAngleY = worldData.angleY - 90.0f ;
	worldData.nextAngleMY = worldData.angleMY - 90.0f ;
	worldData.angleY = worldData.angleMY;
      }
      else{
	worldData.nextXstatus = 2; /* vado a destra */
	/*-----*/
	worldData.nextAngleY = worldData.angleY + 90.0f ;
	worldData.nextAngleMY = worldData.angleMY + 90.0f ;
	worldData.angleY = worldData.angleMY;
      }
      worldData.xStatus = 1;
    }else{
      switch(worldData.nextXstatus){
      case 1: /* sto andando verso sinistra */
	if(worldData.kright){
	  worldData.nextAngleY = worldData.nextAngleY + 90.0f;
	  worldData.nextAngleMY = worldData.nextAngleMY + 90.0f;
	  worldData.nextXstatus = 2;
	  worldData.angleY = worldData.angleMY;
	}
	break;
      case 2: /* sto andando verso destra */
	if(worldData.kleft){
	  worldData.nextAngleY = worldData.nextAngleY - 90.0f;
	  worldData.nextAngleMY = worldData.nextAngleMY - 90.0f;
	  worldData.nextXstatus = 1;
	  worldData.angleY = worldData.angleMY;
	}
	break;
      default:
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
	worldData.angleMX += programData.velAngolare;
	moveFrame = 0;
	if(worldData.angleMX > worldData.nextAngleMX){
	  worldData.angleMX = worldData.nextAngleMX;
	  worldData.angleX = worldData.nextAngleX;
	  worldData.yStatus = 0;
	}
	break;
      case 2:
	worldData.angleMX -= programData.velAngolare;
	moveFrame = 0;
	if(worldData.angleMX < worldData.nextAngleMX){
	  worldData.angleMX = worldData.nextAngleMX;
	  worldData.angleX = worldData.nextAngleX;
	  worldData.yStatus = 0;
	}
	break;
      default:
	break;
      }
      break;
    case 1:
      worldData.angleMX -= programData.velAngolare;
      moveFrame = 0;
      if(worldData.angleMX < worldData.nextAngleMX){
	worldData.angleMX = worldData.nextAngleMX;
	worldData.angleX = worldData.nextAngleX;
	worldData.yStatus = 1;
      }
      break;
    case 2:
      worldData.angleMX += programData.velAngolare;
      moveFrame = 0;
      if(worldData.angleMX > worldData.nextAngleMX){
	worldData.angleMX = worldData.nextAngleMX;
	worldData.angleX = worldData.nextAngleX;
	worldData.yStatus = 2;
      }
      break;
    default:
      break;
    }
  }

  /* modifico l'angolo Y */
  if(worldData.xStatus){ 
    switch(worldData.nextXstatus){
    case 1: /* devo muovermi a sinistra */
      worldData.angleMY -= programData.velAngolare;
      moveFrame = 0;
      if(worldData.angleMY < worldData.nextAngleMY){
	worldData.angleMY = worldData.nextAngleMY;
	worldData.angleY = worldData.nextAngleY;
	worldData.xStatus = 0;
	worldData.nextXstatus = 0;
      }
      break;
    case 2: /* devo muovermi a destra */
      worldData.angleMY += programData.velAngolare;
      moveFrame = 0;
      if(worldData.angleMY > worldData.nextAngleMY){
	worldData.angleMY = worldData.nextAngleMY;
	worldData.angleY = worldData.nextAngleY;
	worldData.xStatus = 0;
	worldData.nextXstatus = 0;
      }
      break;
    default:
      break;
    }
  }

  
  /* controllo se sono in posizione tale da muovermi */
  if(moveFrame){
    float offSetX = 0.0f, offSetY = 0.0f, offSetZ = 0.0f;
    if(worldData.yStatus == 0){
      /* movimento sul piano X */
      offSetX = (float)sin(worldData.angleY * PIOVER180) * 3.5f; 
      offSetZ = (float)cos(worldData.angleY * PIOVER180) * 3.5f;
      moveWorm((float)sin(worldData.angleY * PIOVER180), (float)cos(worldData.angleY * PIOVER180), 0);
      if(checkWall((myWorm.head)->x+myWorm.dia, (myWorm.head)->z +myWorm.dia, 1)){
	/*collision testa del verme*/
	if(sCollisionDetection((myWorm.head)->x, (myWorm.head)->y, (myWorm.head)->z, 0, 0, -50.0f))
	  fprintf(stderr, " presa!\n");
      }else{
	/* e' uscito dai muri */
	programData.gameStatus = 0;
	fprintf(stderr, "sono uscito\n");
      }
    }else{
      /* movimento sul piano Y */
      offSetY = (float)sin(worldData.angleX * PIOVER180) * 3.5f;
      offSetZ = (float)cos(worldData.angleX * PIOVER180) * 3.5f; 
      moveWorm((float)sin(worldData.angleX * PIOVER180), (float)cos(worldData.angleX * PIOVER180), 1);
      if(checkWall((myWorm.head)->x+myWorm.dia, (myWorm.head)->z +myWorm.dia, 2)){
	/*collision testa del verme*/
	if(sCollisionDetection((myWorm.head)->x, (myWorm.head)->y, (myWorm.head)->z, 0, 0, -50.0f))
	  fprintf(stderr, " presa!\n");
      }else{
	/* e' uscito dai muri */
	programData.gameStatus = 0;
	fprintf(stderr, "sono uscito\n");
      }
    }

  }
}
