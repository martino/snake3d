/*
 * Snake 3d -- share.c
 * 
 * Funzioni utili
 *
 */

#include "share.h"


Data  programData;
World worldData;
Worm  myWorm;
Sphere ball;


/* Gestione del verme */

/*
 * Inizializzazione del verme
 */
int initializeWorm(GLfloat x, GLfloat y, GLfloat z, GLfloat dia){
  WSphere *tmp;

  if(dia == 0 ) dia = WORMDIA;

  tmp = (WSphere *)calloc(1, sizeof(WSphere));
  if(tmp == NULL) return 0;
  
  tmp->x = x;
  tmp->y = y;
  tmp->z = z;

  myWorm.tail = tmp;
  myWorm.head = tmp;
  myWorm.dim = 1;
  myWorm.dia = dia;
  return 1;
}

/*
 * Deallocazione della memoria utilizzata per il verme
 */
void destroyWorm(){
  WSphere *ite = myWorm.head;
  WSphere *tmp;

  while(ite != NULL){
    tmp = ite;
    ite = ite->prev;
    if(tmp != NULL) free(tmp);
  }
}

/*
 * Funzione che fa crescere il verme
 */
int increaseWorm(GLfloat xy, GLfloat z, GLuint dir){
  WSphere *tmp = (WSphere *)calloc(1, sizeof(WSphere));
  if(tmp == NULL) return 0;

  /* xz o yz si capisce come si capisce il movimento */
  /* il piu' o meno si capisce uguale in base al caso xz o yz */

  /* secondo me non e' sempre uguale il segno */

  if(dir == 0 ){
    /* sposto il verme sul piano X */
    tmp->x = (myWorm.tail)->x + (xy*myWorm.dia);
    tmp->y = (myWorm.tail)->y;
    tmp->z = (myWorm.tail)->z + (z*myWorm.dia);

  }else{
    /* sposto il verme sul piano Y */
    tmp->x = (myWorm.tail)->x;
    tmp->y = (myWorm.tail)->y - (xy*myWorm.dia);
    tmp->z = (myWorm.tail)->z - (z*myWorm.dia);
  }

  tmp->prev = NULL;
  tmp->next = myWorm.tail;
  (myWorm.tail)->prev = tmp;

  myWorm.tail = tmp;
  myWorm.dim++;
  return 1;
}

/*
 * Funzione che muove il verme in avanti
 *  dir -> indica il piano in cui mi muovo 0=xz 1=yz
 *  xy e z -> sono i "segni"
 */
int moveWorm(GLfloat xy, GLfloat z, GLuint dir){
  WSphere *oldHead = myWorm.head;
  WSphere *oldTail = myWorm.tail;
  WSphere *tmp;

  if(myWorm.dim > 1){
  /* sistemo la coda */
  (oldTail->next)->prev = NULL;
  myWorm.tail = oldTail->next;

  /* sistemo la testa */
  oldHead->next = oldTail;
  oldTail->prev = oldHead;
  oldTail->next = NULL;
  myWorm.head = oldTail;
  }

  if(dir == 0 ){
    /* sposto il verme sul piano X */
    (myWorm.head)->x = oldHead->x - (xy*myWorm.dia);
    (myWorm.head)->y = oldHead->y;
    (myWorm.head)->z = oldHead->z - (z*myWorm.dia);
  }else{
    /* sposto il verme sul piano Y */
    (myWorm.head)->x = oldHead->x;
    (myWorm.head)->y = oldHead->y + (xy*myWorm.dia);
    (myWorm.head)->z = oldHead->z + (z*myWorm.dia);
  }
  

  tmp = (myWorm.head)->prev;
  /* collision detection tra le sfere del verme*/
  
  while(tmp!=NULL){
    if(sCollisionDetection(((myWorm.head)->x), ((myWorm.head)->y), (myWorm.head)->z, tmp->x, tmp->y, tmp->z, WORMDIA, WORMDIA)){
      fprintf(stderr, "gnam gnam verme... \n");
      fprintf(stderr, "%f %f %f %f %f %f\n", (myWorm.head)->x, (myWorm.head)->y, (myWorm.head)->z, tmp->x, tmp->y, tmp->z);
      return 0;
    }
    tmp = tmp->prev;
  }
  //  fprintf(stderr, "ok ... \n");
  //  printf("%f %f %f \n", (myWorm.head)->y,(myWorm.head)->x,(myWorm.head)->z);
  return 1;
}

/* Funzione di DEBUG che visualizza il verme*/
void printWorm(){
  WSphere *ite = myWorm.head;
  fprintf(stderr,"--- BEGIN WORM ---\n");
  while(ite != NULL){
    fprintf(stderr,"Palla X %f  Y %f  Z %f \n", ite->x, ite->y, ite->z);
    ite = ite->prev;
  }
  fprintf(stderr,"--- END WORM ---\n");
}



/* Gestione delle sfere */

/* Inizializzazione palla */
int initializeBall(){
  ball.x = 0.0f;
  ball.y = 0.0f;
  ball.z = -50.0f;
  ball.timer = 10000;

  ball.texture = randomNBall(); 
  /* generateBall() */
  return 1;
}

/* genera una nuova palla */
void generateBall(){
  float randf;
  randf = ((float)random())/RAND_MAX;
  ball.x =  (int)(randf*((WORLDIM-DIA)/2));

  randf = ((float)random())/RAND_MAX;
  if(((int)(randf*10))%2)
    ball.x = ball.x * -1;
 
  randf = ((float)random())/RAND_MAX;
  ball.y =  (int)(randf*((WORLDIM-DIA)/2));
  randf = ((float)random())/RAND_MAX;
  if(((int)(randf*10))%2)
    ball.y = ball.y * -1;


  randf = ((float)random())/RAND_MAX;
  ball.z =  (int)(randf*((WORLDIM-DIA)/2));
  randf = ((float)random())/RAND_MAX;
  if(((int)(randf*10))%2)
    ball.z = ball.z * -1;
  

  if(ball.texture == 3)
    programData.invert = 1;
  else
    programData.invert = 0;

  worldData.texWall = ball.texture;
  ball.texture = randomNBall();

  fprintf(stderr, "x %f y %f z %f tex %d\n", ball.x, ball.y, ball.z, ball.texture);
}

/* distrugge una palla */
void destroyBall(){

}



/*
 * Funzione che carica le tga (senza RLE encoding!!)
 *
 *  restituisce l'immagine caricata
 *  parametri:
 *   -> fileName    nome del file da caricare
 *   -> iWidth	    restituisce la larghezza
 *   -> iHeight	    restituisce l'altezza
 *   -> iComponent  resituisce i componenti dell immagine 1, 3, 4 (luminance, RGB RGBA)
 *   -> eFormat	    restituisce il formato dell'immagine
 *
 */
GLbyte *gltLoadTGA(const char *fileName, GLint *iWidth, GLint *iHeight, GLint *iComponent, GLenum *eFormat){

  FILE *pFile;	           // file pointer
  TGAHEADER tgaHeader;	   // header del TGA
  unsigned long lImageSize;// dimensione in byte dell'immagine
  short sDepth;		   // pixel depth
  GLbyte *pBits = NULL;	   // puntatore ai bit
	
  // carico dei valori de default
  *iWidth = 0;
  *iHeight = 0;
  *eFormat = GL_BGR_EXT;
  *iComponent = GL_RGB8;
	
  // provo ad aprire il file
  pFile = fopen(fileName, "rb");  // da provare ad eliminare la b
  if(pFile == NULL){
    fprintf(stderr, "Non riesco a caricare il file\n");
    return NULL;
  }
		
  // leggo l'header (binario)
  fread(&tgaHeader, 18, 1, pFile); // 18 --> sizeof(TGAHEADER)	
  // ottengo i parametri base dell'immagine
  *iWidth = tgaHeader.width; 
  *iHeight = tgaHeader.height;
  sDepth = tgaHeader.bits/8;	
	
  // accetto targa solo con 8, 24, 32 bit
  if(tgaHeader.bits != 8 && tgaHeader.bits != 24 &&tgaHeader.bits != 32){
    fprintf(stderr, "Il file ha una profondita' di colore sbagliata\n");
    return NULL;
  }
		
  // calcolo le dimensioni dell'image buffer
  lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
	
  // alloco la menoria neccessaria per memorizzare la tga
  pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
  if(pBits == NULL){
    fprintf(stderr, "Non riesco ad allocare la memoria neccessaria\n");
    return NULL;
  }
	
  // ora posso finalmente caricare i bits
  // capisco se è rle dato che le dimensioni sono diverse
  if(fread(pBits, lImageSize, 1, pFile) != 1){
    free(pBits);
    fprintf(stderr, "Errore nella lettura dell'immagine");
    return NULL;
  }
	
  // posso impostare il formato che si aspetta OpenGL
  switch(sDepth){
  case 3:
    *eFormat = GL_BGR_EXT;
    *iComponent = GL_RGB8;
    break;
  case 4:
    *eFormat = GL_BGRA_EXT;
    *iComponent = GL_RGBA8;
    break;
  case 1:
    *eFormat = GL_LUMINANCE;
    *iComponent = GL_LUMINANCE8;
    break;
  };
	
  fclose(pFile);
	
  return pBits;
}
        
/*
 * Funzione per scrivere il contenuto del front buffer in un file TGA
 * senza RLE encoding
 * parametri:
 *   - nome del file TGA
 */

GLint gltWriteTGA(const char *szFileName){
  FILE *pFile;
  TGAHEADER tgaHeader;
  unsigned long lImageSize;
  GLbyte *pBits = NULL;
  GLint iViewport[4];
  GLenum lastBuffer;


  // ottengo la dimensione del viewport
  glGetIntegerv(GL_VIEWPORT, iViewport);
  
  // otengo la dimensione dell'immagine
  lImageSize = iViewport[2] * 3 * iViewport[3];

  //allogo i blocchi
  pBits = (GLbyte *)malloc(lImageSize);
  if(pBits == NULL)
    return 0;

  // leggo i bit dal color bufer
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ROW_LENGTH, 0);
  glPixelStorei(GL_PACK_SKIP_ROWS, 0);
  glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

  
  
  // Ottengo il read buffer e lo salvo
  // Leggo il front buffer e lo leggo
  // Alla fine ripristino il readBuffer
  glGetIntegerv(GL_READ_BUFFER, &lastBuffer);
  glReadBuffer(GL_FRONT);
  glReadPixels(0, 0, iViewport[2], iViewport[3], GL_BGR, GL_UNSIGNED_BYTE, pBits);
  glReadBuffer(lastBuffer);

  // Inizializzo l'header TGA
  tgaHeader.identsize = 0;
  tgaHeader.colorMapType = 0;
  tgaHeader.imageType = 2;
  tgaHeader.colorMapStart = 0;
  tgaHeader.colorMapLength = 0;
  tgaHeader.colorMapBits = 0;
  tgaHeader.xstart = 0;
  tgaHeader.ystart = 0;
  tgaHeader.width = iViewport[2];
  tgaHeader.height = iViewport[3];
  tgaHeader.bits = 24;
  tgaHeader.descriptor = 0;

  
  //apro il file
  pFile = fopen(szFileName, "wb");

  if(pFile == NULL){
    free(pBits);
    return 0;
  }

  //scrivo l'header
  fwrite(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

  //scrivo l'immagine
  fwrite(pBits, lImageSize, 1, pFile);

  //libero la memoria e chiudo il file
  free(pBits);
  fclose(pFile);

  return 1;
}





/*
 * Funzione che calcola la distanza euclidea
 */

GLdouble dist2Point(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2){
  return ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)) + ((z1 - z2) * (z1 - z2));
}


/*
 * Incremento della velocita'
 */
void increaseVel(GLint vel){

  programData.velocity += vel;
  programData.timeFrame = 1250/programData.velocity;
  programData.velAngolare = programData.velocity*0.2;
    
  fprintf(stderr, " velocity %d timeframe %d velangolare %f \n", programData.velocity, programData.timeFrame, programData.velAngolare);

}

/*
 * Genera un numero casuale per la texture della palla
 */
int randomNBall(){
  int res = 0;
  float randf;
  randf = ((float)random())/RAND_MAX;
  res =  (int)(randf*4);
  
  return res;
}
