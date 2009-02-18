/*
 * Snake 3d -- Main.c
 * 
 * Qui c'e' lo scheletro del programma
 *
 */

#include "share.h"


void TimerFunc(int value){
  event();
  loop();
  glutPostRedisplay();
  glutTimerFunc(16, TimerFunc, 0);
}


int initialize(int argc, char *argv[]){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(winWidth, winHeight);
  glutCreateWindow("SnakeTreD");

  glutDisplayFunc(renerScene);
/*   glutReshapeFunc(changeSize);  */

/* devo assegnare le varie funzioni di gestione tasitera - mouse
 
   queste semplicemente gestistcono l'evento aggiungendo un messaggio in coda
 */

  glutTimerFunc(33, TimerFunc, 1);
  glutMainLoop();
  return 1;
}


void cleanup(){


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

  cleanup();
  return 1;
}
