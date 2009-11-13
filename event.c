/*
 * Snake 3d -- Event.c
 * 
 * Gestione degli eventi
 *
 */

#include "event.h"

/*
 * gestisce il mouse
 */
void mouseHandler(int x, int y){
  float tmpX = 0.0f, tmpY = 0.0f;
  static int pippo = 0;
  if((worldData.angleY != worldData.nextAngleY) || (worldData.angleX != worldData.nextAngleX))
    return;

  tmpX = x - (programData.width / 2);
  tmpY = y - (programData.height / 2);

  if((tmpX == 0.0f) && (tmpY == 0.0f))
    return;

/*   if(tmpY > 8.0f) tmpY = 8.0; */
/*   if(tmpX > 8.0f) tmpX = 8.0; */

/*   if(tmpY < -8.0f) tmpYsi = -8.0; */
/*   if(tmpX < -8.0f) tmpX = -8.0; */

  worldData.angleMX += tmpY * 0.25f;
  worldData.angleMY += tmpX * 0.25f;
  
/*   fprintf(stderr, "MX %f  MY %f ", worldData.angleMX, worldData.angleMY); */
/*   fprintf(stderr, "tmp x %f  y %f ", tmpX, tmpY); */
/*   fprintf(stderr, "x %d  y %d \n", x, y); */
  
  if( (worldData.angleMY > (worldData.angleY+45.0f)) ||  (worldData.angleMY< (worldData.angleY-45.0f) ))
    worldData.angleMY -= tmpX * 0.25f;

  if( (worldData.angleMX > (worldData.angleX+45.0f)) ||  (worldData.angleMX< (worldData.angleX-45.0f) ))
    worldData.angleMX -= tmpY * 0.25f;


  glutWarpPointer(programData.width/2, programData.height/2);
  pippo++;
  fprintf(stderr, "%d\n", pippo);


}

/*
 * gestisce i tasti "normali"
 */ 
void keyHandler(GLubyte key, GLint x, GLint y){
  if(key== 'q'){
    fprintf(stderr, "Exit\n");
    programData.exit = 1;
  }else{  
    if(key == 'p'){ // menu
      programData.menu = !programData.menu;
    }else{
      if(key == 'm'){ // multisample
	programData.multisample = !programData.multisample;
	if(programData.multisample){
	  fprintf(stderr, "MULTISAMPLE ON\n");
	  glEnable(GL_MULTISAMPLE);
	}
	else{
	  fprintf(stderr, "MULTISAMPLE OFF\n");
	  glDisable(GL_MULTISAMPLE);
	}
      }else{
      	if(key == 'f'){ // nebbia
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
	  if(key == 'n'){ // nuova partita
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
	    if(key == 'a'){
	      if(worldData.yStatus == 0)
		increaseWorm((float)sin(worldData.angleY * PIOVER180), (float)cos(worldData.angleY * PIOVER180), 0);
	      else
		increaseWorm((float)sin(worldData.angleX * PIOVER180), (float)cos(worldData.angleX * PIOVER180), 1);
	    }else{
	      if(key == 's'){//screenshot
		gltWriteTGA("screenshot.tga");
	      }
	    }	      
	  }
	}
      }
    }
  }
}


/*  
 * gestisce le freccie
 */
void specialKeyHandler(GLint key, GLint x, GLint y){
  switch(key){
  case GLUT_KEY_RIGHT:
    /*     worldData.angleY += 90.0f; */
    if(programData.invert == 0)
      worldData.kright = 1;
    else
      worldData.kleft = 1;
    break;
  case GLUT_KEY_LEFT:
    /*     worldData.angleY -= 90.0f; */
    if(programData.invert == 0)
      worldData.kleft = 1;
    else
      worldData.kright = 1;
    break;
  case GLUT_KEY_UP:
    /*     worldData.angleY += 90.0f; */
    if(programData.invert == 0)
      worldData.kup = 1;
    else
      worldData.kdown = 1;
    break;
  case GLUT_KEY_DOWN:
    /*     worldData.angleY -= 90.0f; */
    if(programData.invert == 0)
      worldData.kdown = 1;
    else
      worldData.kup = 1;
    break;
  }
  

}

