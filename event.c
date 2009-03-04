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
  float tmpX, tmpY;

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

/*
 * gestisce i tasti "normali"
 */ 
void keyHandler(GLubyte key, GLint x, GLint y){
	if(key== 'q'){
		fprintf(stderr, "Exit\n");
		programData.exit = 1;
	}
	
	/* gestione dei vari effetti, compreso menù */
	if(key == 'm'){ // menu
		programData.menu = !programData.menu;
	}
	
	if(key == 'f'){ // fullscreen
		
	}

	if(key == 'r'){ 
	  worldData.cColor = 1;
	}

	if(key == 't'){ 
	  worldData.cColor = 2;
	}

	if(key == 'y'){ 
	  worldData.cColor = 3;
	}
	/*
	if(key == 'u'){ 
	  worldData.position[0] += 0.1f;
	  fprintf(stderr, "light pos %f %f %f\n", worldData.position[0], worldData.position[1], worldData.position[2]);
	}

	if(key == 'j'){ 
	  worldData.position[0] -= 0.1f;
	  fprintf(stderr, "light pos %f %f %f\n", worldData.position[0], worldData.position[1], worldData.position[2]);
	}

	if(key == 'i'){ 
	  worldData.position[1] += 0.1f;
	  fprintf(stderr, "light pos %f %f %f\n", worldData.position[0], worldData.position[1], worldData.position[2]);
	}

	if(key == 'k'){ 
	  worldData.position[1] -= 0.1f;
	  fprintf(stderr, "light pos %f %f %f\n", worldData.position[0], worldData.position[1], worldData.position[2]);
	}

	if(key == 'o'){ 
	  worldData.position[2] += 0.1f;
	  fprintf(stderr, "light pos %f %f %f\n", worldData.position[0], worldData.position[1], worldData.position[2]);
	}

	if(key == 'l'){ 
	  worldData.position[2] -= 0.1f;
	  fprintf(stderr, "light pos %f %f %f\n", worldData.position[0], worldData.position[1], worldData.position[2]);
	}


*/
}

/*  
 * gestisce le freccie
 */
void specialKeyHandler(GLint key, GLint x, GLint y){
  if(key == GLUT_KEY_RIGHT){
    /*     worldData.angleY += 90.0f; */
    worldData.kright = 1;
  }

  if(key == GLUT_KEY_LEFT){
    /*     worldData.angleY -= 90.0f; */
    worldData.kleft = 1;
  }
  

  if(key == GLUT_KEY_UP){
/*     worldData.angleY += 90.0f; */
    worldData.kup = 1;

  }

  if(key == GLUT_KEY_DOWN){
/*     worldData.angleY -= 90.0f; */
    worldData.kdown = 1;

  }
  

}


/**
   La funzione event ha il compito di gestire gli eventi del mouse e della tastiera

   Ci saranno due grandi categorie da gestire:
   - eventi del menu'
   - eventi di gioco
	
   - coda movimenti (freccie tastiera + mouse)                              --> MovEvent
   - coda eventi    (le lettere che abilitano/disabilitano le varie cose)   --> ComEvent


 */
void event(){

  return;
}
