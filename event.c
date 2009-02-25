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
