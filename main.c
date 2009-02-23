/*
 * Snake 3d -- Main.c
 * 
 * Qui c'e' lo scheletro del programma
 *
 */

#include "share.h"



void cleanup(){

}


/*
 * funcizione che funge da "ciclo" principale
 * vengono gestiti gli eventi, gli update ed alla fine viene disegnato tutto
 */
void TimerFunc(int value){
  event();
  loop();
  glutPostRedisplay();
  if(programData.exit){
	cleanup();
	exit(0);
  }
  glutTimerFunc(16, TimerFunc, 0);
}


int initialize(int argc, char *argv[]){
  /*
   * bisogna inizializzare:
   *   mondo
   *   texture
   *   oggetti
   *   displaylist   
   *
   */

  /*
   * Inizializzazione della struttura mondo
   */
  programData.width = winWidth ;
  programData.height = winHeight;

  programData.frame = 0;
  programData.time = 0;
  programData.timebase = 0;
  programData.timerender = 0;

  programData.fullscreen = 0;
  programData.menu = 0;
  programData.exit = 0;
  programData.font  = (GLint)GLUT_BITMAP_HELVETICA_18;
  /*
   * Inizializzazione openGL
   */

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(programData.width, programData.height);
  glutCreateWindow("SnakeTreD");

  glutDisplayFunc(render);
  glutReshapeFunc(changeSize);
  

  /*
   * funzione per gestire il movimento del mouse
   */
  glutPassiveMotionFunc(mouseHandler);

  /*
   * funzione per gestire i tasti speciali:
   *  - freccie
   */
  glutSpecialFunc(specialKeyHandler);

  /*
   * funzione per gestire i tasti normali:
   *  - lettere
   */
  glutKeyboardFunc(keyHandler);


  glutTimerFunc(33, TimerFunc, 1);
  glutMainLoop();
  return 1;
}



int main(int argc, char *argv[]){
  
  if(!initialize(argc, argv))
    return 0;

  // questo ciclo va messo in una funzione richamata poi dal timer glut

/*   while(1){ */
/*     event();  */
/*     loop(); */
/*     render(); */
/*   } */

//  cleanup();
  return 1;
}
