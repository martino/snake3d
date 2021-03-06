/*
 * Snake 3d -- Main.c
 * 
 * Qui c'e' lo scheletro del programma
 *
 */

#include "share.h"


/*
 * funcizione che funge da ciclo principale
 * vengono gestiti gli eventi, gli update ed alla fine viene disegnato tutto
 */
void TimerFunc(int value){
  loop();
  glutPostRedisplay();
  if(programData.exit){
    exit(0);
  }

  glutTimerFunc(16, TimerFunc, 0);
}


int initialize(int argc, char *argv[]){
  /*
   * Inizializzazione della struttura mondo
   */
  GLint i = 0;

  const char *texFiles[] = {"texture/texr.tga", "texture/texg.tga", 
			    "texture/texb.tga", "texture/texmy.tga",
			    "texture/worm.tga", "texture/metal.tga",
			    "texture/menu.tga", "texture/ground.tga"};

  programData.width = winWidth ;
  programData.height = winHeight;

  programData.frame = 0;
  programData.time = 0;
  programData.timebase = 0;
  programData.timerender = 0;

  programData.fullscreen = 0;
  programData.menu = 1;
  programData.exit = 0;
  programData.gameStatus = 0;
  programData.font  = (GLint)GLUT_BITMAP_HELVETICA_18;
  programData.points= 0;
  programData.invert = 0;
  sprintf(programData.fps, "FPS: --");
  sprintf(programData.pointsOSD, "PUNTI: %d", programData.points);
  

  /* Effetti grafici */
  programData.multisample = 1;
  programData.fog = 0;

  /* Inizializzazione openGL */

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(programData.width, programData.height);
  glutCreateWindow("Snake3di");

  /* Carico le texutre */
  glEnable(GL_TEXTURE_2D);
  glGenTextures(NTEX, worldData.texObj);
    
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
    /*
     *  gluBuild2DMipmaps -> carica una texture 2d in memoria creando tutte le immagini scalate per il mipmap
     *  parametri: uguali a glTexImage2D senza i livelli(dato che li crea) ed i bordi
     *
     */
    
    gluBuild2DMipmaps(GL_TEXTURE_2D, components, width, height, format, GL_UNSIGNED_BYTE, pImage);   
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
     *                  --> GL_NEAREST_MIPMAP_NEAREST  sceglie il livello di mipmap piu` vicino e poi applica un GL_NEAREST
     *                  --> GL_NEAREST_MIPMAP_LINEAR   fa un'interpolazione lineare tra i livelli di mipmap e poi applica un GL_NEAREST
     *                  --> GL_LINEAR_MIPMAP_NEAREST   sceglie il livello di mipmap piu` vicino e poi fa un GL_LINEAR
     *                  --> GL_LINEAR_MIPMAP_LINEAR    fa un'interpolazione lineare ed applica un GL_LINEAR, viene chiamato trilinear mipmapping
     *                                               [quest'ultimo da i risultati migliori]
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
    free(pImage);

  }

  /* imposto  Sphere Mapping */
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

  worldData.q = gluNewQuadric();
  /* specifico le normali
   *   GL_SMOOTH specifica di calcolare la normale per ogni vertice
   */
  gluQuadricNormals(worldData.q, GL_SMOOTH);
  /* genera le coordinate texture */
  gluQuadricTexture(worldData.q, GL_TRUE);

   
  /* Creo le display list */
  worldData.wall      = glGenLists(6);
  worldData.ground    = worldData.wall + 1;
  worldData.sky       = worldData.wall + 2;
  worldData.ball      = worldData.wall + 3;
  worldData.wallsd    = worldData.wall + 4;
  worldData.worm      = worldData.wall + 5;
  
  worldData.texWall   = TB;
  worldData.texWallSd = TO;
    
  createWorld();
  
  
  /* Gestione luci */

  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, worldData.ambientLight);
  /*
   * imposta i parametri per le luci
   * 
   * il primo parametro è il nome della luce:
   * GL_LIGHT n  -> 0 <= n <= maxlight
   * 
   * il secondo parametro sono i parametri della luce che si imposteranno:
   *  * GL_AMBIENT
   *  * GL_DIFFUSE
   *  * GL_SPECULAR
   *  * GL_POSITION
   *  * GL_SPOT_CUTOFF
   *  * GL_SPOT_DIRECTION
   *  * GL_SPOT_EXPONENT
   *  * GL_CONSTANT_ATTENUATION
   *  * GL_LINEAR_ATTENUATION
   *  * GL_QUADRATIC_ATTENUATION
   * 
   * l'ultimo parametro sono i valori che si impostano
   */

  worldData.ambientLight[0] = 0.6f;
  worldData.ambientLight[1] = 0.6f;
  worldData.ambientLight[2] = 0.6f;
  worldData.ambientLight[3] = 1.0f;
  glLightfv(GL_LIGHT0, GL_AMBIENT,  worldData.ambientLight);

  worldData.diffuseLight[0] = 0.4f;
  worldData.diffuseLight[1] = 0.4f;
  worldData.diffuseLight[2] = 0.4f;
  worldData.diffuseLight[3] = 1.0f;
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  worldData.diffuseLight);	

  worldData.specularLight[0] = 1.0f;
  worldData.specularLight[1] = 1.0f;
  worldData.specularLight[2] = 1.0f;
  worldData.specularLight[3] = 1.0f;
  glLightfv(GL_LIGHT0, GL_SPECULAR, worldData.specularLight);

  worldData.positionA[0] = 0.0f;
  worldData.positionA[1] = 0.0f;
  worldData.positionA[2] = 0.0f;
  worldData.positionA[3] = 0.0f;
  glLightfv(GL_LIGHT0, GL_POSITION, worldData.positionA);

  /* abilito la luce */
  glEnable(GL_LIGHT0);


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
    
  /* sistemo  i materiali con il metodo più utilizzato, cioè lasciando la colorazione a glcolor */
  glEnable(GL_COLOR_MATERIAL);
	
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  
  /* imposto la componente speculare sul materiale */
  /*  NB: fino a che non richiamo ancora glMaterialfv tutte le superfici sono trasparenti */
  worldData.specref[0] = 1.0f;
  worldData.specref[1] = 1.0f;
  worldData.specref[2] = 1.0f;
  worldData.specref[3] = 1.0f;

  glMaterialfv(GL_FRONT, GL_SPECULAR, worldData.specref);
  
  /* ora però devo specificare l'esponente di brilantezza (0 è come non impostare nulla) */
  glMateriali(GL_FRONT, GL_SHININESS, 128);

  
  glutDisplayFunc(render);
  glutReshapeFunc(changeSize);
  

  /* funzione per gestire il movimento del mouse */
  glutPassiveMotionFunc(mouseHandler);

  /*
   * funzione per gestire i tasti speciali:
   *  - frecce
   */
  glutSpecialFunc(specialKeyHandler);

  /*
   * funzione per gestire i tasti normali:
   *  - caratteri
   */
  glutKeyboardFunc(keyHandler);


  glutTimerFunc(33, TimerFunc, 1);
  glutMainLoop();
  return 1;
}



int main(int argc, char *argv[]){
  
  if(!initialize(argc, argv))
    return 0;

  return 1;
}
