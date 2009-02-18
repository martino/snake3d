/*
 * Snake 3d -- Main.c
 * 
 * Qui c'e' lo scheletro del programma
 *
 */

#include "share.h"




int main(int argc, char *argv[]){
  
  if(!initialize())
    return 0;

  while(true){
    event();
    loop();
    render();
  }

  cleanup();
  return 1;
}
