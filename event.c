/*
 * Snake 3d -- Event.c
 * 
 * Gestione degli eventi
 *
 */

#include "event.h"

/* gestisce il mouse */
void mouseHandler(int x, int y){
  float tmpX = 0.0f, tmpY = 0.0f;
  if((worldData.angleY != worldData.nextAngleY) || (worldData.angleX != worldData.nextAngleX))
    return;

  tmpX = x - (programData.width / 2);
  tmpY = y - (programData.height / 2);

  if((tmpX == 0.0f) && (tmpY == 0.0f))
    return;

  worldData.angleMX += tmpY * 0.25f;
  worldData.angleMY += tmpX * 0.25f;
  
  if( (worldData.angleMY > (worldData.angleY+45.0f)) ||  (worldData.angleMY< (worldData.angleY-45.0f) ))
    worldData.angleMY -= tmpX * 0.25f;

  if( (worldData.angleMX > (worldData.angleX+45.0f)) ||  (worldData.angleMX< (worldData.angleX-45.0f) ))
    worldData.angleMX -= tmpY * 0.25f;

  glutWarpPointer(programData.width/2, programData.height/2);

}

/* gestisce i tasti carattere */ 
void keyHandler(GLubyte key, GLint x, GLint y){
  static int sCount=0;
  if(key== 'q'){
    fprintf(stderr, "Exit\n");
    programData.exit = 1;
  }else{  
    if(key == 'p'){ /* menu */
      programData.menu = !programData.menu;
    }else{
      if(key == 'm'){ /* multisampling */
	programData.multisample = !programData.multisample;
	if(programData.multisample){
	  fprintf(stderr, "MULTISAMPING ON\n");
	  glEnable(GL_MULTISAMPLE);
	}
	else{
	  fprintf(stderr, "MULTISAMPING OFF\n");
	  glDisable(GL_MULTISAMPLE);
	}
      }else{
      	if(key == 'f'){ /* nebbia */
	  programData.fog= !programData.fog;
	  if(programData.fog){
	    fprintf(stderr, "FOG ON\n");
	    fog(1);
	  }
	  else{
	    fprintf(stderr, "FOG OFF\n");
	    fog(0);
	  }
	}else{
	  if(key == 'n'){ /* nuova partita */
	    programData.gameStatus = 1;
	    programData.menu = 0;
	    programData.points= 0;
	    
	    /* Inizializzo la posizione della telecamera */
	    
	    worldData.angleX = 0.0f;
	    worldData.angleY = 0.0f;
	    
	    worldData.angleMX = 0.0f;
	    worldData.angleMY = 0.0f;
	    
	    worldData.nextAngleX = 0.0f;
	    worldData.nextAngleY = 0.0f;
	    
	    worldData.nextAngleMX = 0.0f;
	    worldData.nextAngleMY = 0.0f;
	    
	    /* Inizializzo il verme */
	    initializeWorm(0.0f, 0.0f, 83.5f, 0);
	    srand(time(NULL));
	      
	    /* Inizializzo la palla */
	    initializeBall();
	    
	    /* Inizializzo i dati utilizzati per la telecamera */
	    
	    worldData.kup    = 0;
	    worldData.kdown  = 0;
	    worldData.kleft  = 0;
	    worldData.kright = 0;
	    
	    worldData.xStatus = 0;
	    worldData.yStatus = 0;
	    
	    worldData.nextXstatus = 0;
	    worldData.nextYstatus = 0;
	    programData.velocity = VELOCITY;
	    programData.timeFrame = 1250/programData.velocity;
	    programData.velAngolare = programData.velocity*0.2;
	    
	    programData.timerender = programData.time;
	    programData.timebase = programData.time;
	    programData.frame = 0;
	    
	  }else{
	    if(key == 's'){/* screenshot */
	      char sname[20];
	      sprintf(sname, "screenshot%d.tga",sCount);
	      gltWriteTGA(sname);
	      sCount++;
	    }
	  }	      
	}
      }
    }
  }
}



/* gestisce le frecce */
void specialKeyHandler(GLint key, GLint x, GLint y){
  switch(key){
  case GLUT_KEY_RIGHT:
    if(programData.invert == 0)
      worldData.kright = 1;
    else
      worldData.kleft = 1;
    break;
  case GLUT_KEY_LEFT:
    if(programData.invert == 0)
      worldData.kleft = 1;
    else
      worldData.kright = 1;
    break;
  case GLUT_KEY_UP:
    if(programData.invert == 0)
      worldData.kup = 1;
    else
      worldData.kdown = 1;
    break;
  case GLUT_KEY_DOWN:
    if(programData.invert == 0)
      worldData.kdown = 1;
    else
      worldData.kup = 1;
    break;
  }
  

}

