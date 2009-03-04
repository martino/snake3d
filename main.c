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
   * Inizializzazione della struttura mondo
   */


  GLint i = 0;
  const char *texFiles[] = {"texture/wallr.tga", "texture/wallg.tga", "texture/wallb.tga", "texture/ground.tga","texture/sky.tga" };

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
  
  sprintf(programData.fps, "FPS: --");

  /*
   * Inizializzazione openGL
   */

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(programData.width, programData.height);
  glutCreateWindow("SnakeTreD");


  /*
   * bisogna inizializzare:
   *   mondo
   *   oggetti
   *
   */


  /*
   * Carico le texutre
   */
  glEnable(GL_TEXTURE_2D);
  glGenTextures(NTEX, worldData.texObj);
  //parametri delle texutre
  //  glTexEnvi(GL_TEXTURE....
  for(i=0; i<NTEX; i++){ // carico tutte le texture
    GLbyte *pImage;
    GLint width, height, components;
    GLenum format;
    
    glBindTexture(GL_TEXTURE_2D, worldData.texObj[i]);
    pImage = gltLoadTGA(texFiles[i], &width, &height, &components, &format);
   
    /*
     *	glTexImage2D  -> carica una texture 2d in memoria,
     *	parametri:
     *	  -> target: specifica il tipo di texture puo' essere GL_TEXTURE_2D oppure GL_PROXY_TEXTURE_2D,
     *	  -> level: specifica il numero di livello di dettaglio, il livello 0 e' il base [mipmapping]
     *	  -> internalformat: specifica il numero di colori nella texture, solitamente si usano questi simboli:
     *		GL_ALPHA
     *		GL_LUMINANCE
     *		GL_LUMINANCE_ALPHA
     *		GL_RGB
     *		GL_RGBA
     *	  -> width: larghezza della texture
     *	  -> height:altezza della texture
     *	  -> depth: profondita' di colore della texture
     *		NB: questi tre parametri prima di opengl 2.0 dovevano essere interi potenze di 2
     *	  -> border:	specifica la dimensione del bordo
     *	  -> format: 	speficifo il formato dei dati che andrò a scrivere:
     *		GL_COLOR_INDEX
     *		GL_STENCIL_INDEX
     *		GL_DEPTH_COMPONENT
     *		GL_RGB
     *		GL_BGR
     *		GL_RGBA
     *		GL_BGRA
     *		GL_RED
     *		GL_GREEN
     *		GL_BLUE
     *		GL_ALPHA
     *		GL_LUMINANCE
     *		GL_LUMINANCE_ALPHA
     *	  -> type:   specifica il tipo di dati (es: GL_UNSIGNED_BYTE, etc etc)
     *	  -> pixels: è un puntatore al blocco di pixel 
     */
    glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, pImage);
    free(pImage);
    
    /*
     *	glTexParameteri  -> imposta i parametri della texture
     *	parametri:
     *	  -> target: specifica la texture  GL_TEXTURE_1D, GL_TEXTURE_2D, or GL_TEXTURE_3D
     *	  -> pname:  specifica quale parametro viene impostato
     *		* basic filtering
     *		   il processo di calcolare i "color fragmnet" da una texture map ingrandita o rimpicciolita è detto filtering
     *		   posso agire sul funzionamento di quest'ultimo utilizzando due parametri:
     *			--> GL_TEXTURE_MAG_FILTER magnification
     *			--> GL_TEXTURE_MIN_FILTER minification  (bisogna sempre scegliere uno dei due filtri sottostanti se non utilizzo mipmapping)
     *		   ed i parametri impostabili sono: (manca mipmapping)
     *			--> GL_LINEAR  [linear filtering] è un pochino più esoso del NEAREST(ma con l'hw attuale non rappresenta un problmea)
     *					il valore del pixel viene interpolato dai suoi vicini
     *			--> GL_NEAREST [nearest	neighbor] è il filtro più veloce e semplice, restituisce il valore del texel più vicino al pixel che deve essere disegnato
     *	 		
     *		* texture warp	
     *		   normalmente le coordinate texture vengono specificate tra 0.0 e 1.0, se però le coordinate texture escono da questo range allora OpenGL funziona in base alla warp mode impostata
     *		   il warp si può configurare per ogni coordinata quindi ho a disposizione:
     *			--> GL_TEXTURE_WARP_S
     *			--> GL_TEXTURE_WARP_T
     *			--> GL_TEXTURE_WARP_R
     *		   ed i parametri disponibili sono:
     *			--> GL_REPEAT	semplicemente fa in modo che la texture si ripeta nella direzione in cui la coordinata è maggiore di 1.0
     *					questo metodo è molto comodo per ripetere le texture più volte
     *			--> GL_CLAMP	i texel vengono presi dal bordo della texture oppure da TEXTURE_BORDER_COLOR (impostato dempre con la glTexParameter) 
     *			--> GL_CLAMP_TO_EDGE	i texel fuori dal range vengono presi dall'ultima riga/colonna
     *			--> GL_CLAMP_TO_BORDER	utilizza i border texel 
     *	 -> param:  specifica il valore del parametro
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
    

  }

   
  /*
   * Creo le display list
   */
  worldData.wall   = glGenLists(3);
  worldData.ground = worldData.wall + 1;
  worldData.sky    = worldData.wall + 2;
  createWorld();
  worldData.cColor = 1;
  
  
  /*
   * Gestione luci
   */

  /* luce ambientale bianca */
  worldData.ambientLight[0] = 0.2f;
  worldData.ambientLight[1] = 0.2f;
  worldData.ambientLight[2] = 0.2f;
  worldData.ambientLight[3] = 1.0f;
  
  worldData.diffuseLight[0] = 0.4f;
  worldData.diffuseLight[1] = 0.4f;
  worldData.diffuseLight[2] = 0.4f;
  worldData.diffuseLight[3] = 1.0f;

  worldData.specularLight[0] = 1.0f;
  worldData.specularLight[1] = 1.0f;
  worldData.specularLight[2] = 1.0f;
  worldData.specularLight[3] = 1.0f;

  /* posizioni delle due luci spot in fronte basso che puntano verso l'alto*/
  worldData.positionA[0] =  -(WORLDIM - 1.0f);
  worldData.positionA[1] =  -(WORLDIM - 1.0f);
  worldData.positionA[2] =  -(WORLDIM - 1.0f);
  worldData.positionA[3] =   1.0f;

  worldData.spotDirA[0] = -1.0f;
  worldData.spotDirA[1] = -1.0f;
  worldData.spotDirA[2] = -1.0f;

  worldData.positionB[0] =  WORLDIM - 1.0f;
  worldData.positionB[1] = -(WORLDIM - 1.0f);
  worldData.positionB[2] = -(WORLDIM - 1.0f);
  worldData.positionB[3] =   1.0f;

  worldData.spotDirB[0] =  1.0f;
  worldData.spotDirB[1] = -1.0f;
  worldData.spotDirB[2] = -1.0f;


  /* Inizializzo la posizione della telecamera */
  worldData.x = 0.0f;
  worldData.y = 0.0f;
  worldData.z = 80.0f;
  worldData.angleX = 0.0f;
  worldData.angleY = 0.0f;

  worldData.angleMX = 0.0f;
  worldData.angleMY = 0.0f;

  worldData.nextAngleX = 0.0f;
  worldData.nextAngleY = 0.0f;

  
  /* Inizializzo i dati utilizzati per la telecamera */

  worldData.kup    = 0;
  worldData.kdown  = 0;
  worldData.kleft  = 0;
  worldData.kright = 0;
  
  worldData.xStatus = 0;
  worldData.yStatus = 0;

  worldData.nextXstatus = 0;
  worldData.nextYstatus = 0;

  worldData.change = 0;


  // sistemo  i materiali con il metodo più utilizzato, cioè lasciando la colorazione a glcolor
  glEnable(GL_COLOR_MATERIAL);
	
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  
  // imposto la componente speculare sul materiale
  //		NB: fino a che non richiamo ancora glMaterialfv tutte le superfici sono trasparenti
  worldData.specref[0] = 1.0f;
  worldData.specref[1] = 1.0f;
  worldData.specref[2] = 1.0f;
  worldData.specref[3] = 1.0f;

  glMaterialfv(GL_FRONT, GL_SPECULAR, worldData.specref);
  
  // ora però devo specificare l'esponente di brilantezza (0 è come non impostare nulla)
  glMateriali(GL_FRONT, GL_SHININESS, 128);


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
