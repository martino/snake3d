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
 * parametri:
 * -> fileName	 nome del file da caricare
 * -> tex        texture caricata
 *
 */
        
GLint gltLoadTGA(const char *fileName, Tex *texture){

  FILE *pFile;		    // file pointer
  TGAHEADER tgaHeader;	    // header del TGA
  unsigned long lImageSize; // dimensione in byte dell'immagine
  short sDepth;		    // pixel depth
  GLbyte *pBits = NULL;	    // puntatore ai bit

	
  // carico dei valori de default
  texture->width = 0;
  texture->height = 0;
  texture->format = GL_BGR_EXT;
  texture->component = GL_RGB8;
	
  // provo ad aprire il file
  pFile = fopen(fileName, "rb"); // da provare ad eliminare la b
  if(pFile == NULL){
    fprintf(stderr, "Non riesco a caricare il file\n");
    return 0;
  }
		
  // leggo l'header (binario)
  fread(&tgaHeader, 18, 1, pFile); // 18 --> sizeof(TGAHEADER)	
  // ottengo i parametri base dell'immagine
  texture->width = tgaHeader.width; 
  texture->height = tgaHeader.height;
  sDepth = tgaHeader.bits/8;	
	
  // controllo idiota che mi permette solo di accettare targa solo con 8, 24, 32 bit
  if(tgaHeader.bits != 8 && tgaHeader.bits != 24 &&tgaHeader.bits != 32){
    fprintf(stderr, "Il file ha una profondita' di colore sbagliata\n");
    return 1;
  }
		
  // calcolo le dimensioni dell'image buffer
  lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
	
  // alloco la menoria neccessaria per memorizzare la tga
  pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
  if(pBits == NULL){
    fprintf(stderr, "Non riesco ad allocare la memoria neccessaria\n");
    return 0;
  }
	
  // ora posso finalmente caricare i bits
  // capisco se è rle dato che le dimensioni sono diverse
  if(fread(pBits, lImageSize, 1, pFile) != 1){
    free(pBits);
    fprintf(stderr, "Errore nella lettura dell'immagine");
    return 0;
  }
	
  // posso impostare il formato che si aspetta OpenGL
  switch(sDepth){
  case 3:
    texture->format = GL_BGR_EXT;
    texture->component = GL_RGB8;
    break;
  case 4:
    texture->format = GL_BGRA_EXT;
    texture->component = GL_RGBA8;
    break;
  case 1:
    texture->format = GL_LUMINANCE;
    texture->component = GL_LUMINANCE8;
    break;
  };
	
  fclose(pFile);
  texture->image = pBits;
  return 1;
}
