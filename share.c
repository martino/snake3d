/*
 * Snake 3d -- share.c
 * 
 * Funzioni utili
 *
 */

#include "share.h"


Data programData;
World worldData;

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
        
