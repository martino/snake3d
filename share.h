/*
 * Snake 3d -- share.h
 * 
 * Definiamo qui tutte le strutture del programma e le 
 * funzioni "utili"
 *
 */


#ifndef SHARE_H
#define SHARE_H


/*
 * include OpenGL
 */
// Mac OS X
#ifdef __APPLE__
#include <OpenGL/gl.h>		// Apple OpenGL haders (version depends on OS X SDK version)
#include <OpenGL/glu.h>		// OpenGL Utilities
#include <Glut/glut.h>		// Apples Implementation of GLUT
#else  // conto di essere in linux
#include <GL/glut.h>
#endif

/*
 * include comuni
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * include degli altri file
 */

#include "render.h"
#include "event.h"
#include "loop.h"

/**
 * costanti
 */

#define winHeight 600
#define winWidth  800
//texWallR, texWallG, texWallB, texUp, texDown;
#define TWR   0
#define TWG   1
#define TWB   2
#define TG    3
#define TS    4
#define NTEX  5
#define WORLDIM 100.0f
#define PIOVER180 0.0174532925f

/*
 * strutture dati
 */

/* esempio

typedef struct{
	int nvert;
	int nedge;
	int nface;
	
	float x[MAXVERT], y[MAXVERT], z[MAXVERT];
	int edge[MAXEDGE][2];
	int face[MAXFACE][3];
} Mesh;

*/


/* struttura per la gestione delle TGA */
#pragma pack(1)// da capire bene
typedef struct{
  GLbyte identsize;		// Dimensione del campo ID che segue l'header
  GLbyte colorMapType;		// 0 = none, 1 = paletted
  GLbyte imageType;		// 0 = none, 1 = indexed, 2 = rgb, 3 = gray, +8 = rle
  unsigned short colorMapStart;	// prima entry color map
  unsigned short colorMapLength;// numero di colori
  unsigned char  colorMapBits;	// bit per ogni palette entry
  unsigned short xstart;	// origine x dell'immagine
  unsigned short ystart;	// origine y dell'immagine
  unsigned short width;		// larghezza dell'immagine
  unsigned short height;	// altezza dell'immagine
  GLbyte bits;			// bitss per pixel (8,16,24,32)
  GLbyte descriptor;		// descrittore dell'immagine
} TGAHEADER;
#pragma pack(8)


typedef struct{

} MovEvent;

typedef struct{

} ComEvent;

typedef struct{

} Sphere;

typedef struct{

} SphereList;

typedef struct{

} Worm;


typedef struct{
  /* texture */
  GLuint texObj[NTEX];
  GLuint wall, ground, sky;
  GLuint cColor;
  
  /* luci */
  GLfloat ambientLight[4];
  GLfloat diffuseLight[4];
  GLfloat specularLight[4];
  GLfloat positionA[4];
  GLfloat spotDirA[3];
  GLfloat positionB[4];
  GLfloat spotDirB[3];
  /* materiali */
  GLfloat specref[4];

  /* telecamera */
  GLfloat angleX, angleY;
  GLfloat nextAngleX, nextAngleY;
  GLfloat lastAngleX, lastAngleY;

  GLfloat x, y, z;
  
  GLuint kup, kdown, kleft, kright;
  GLuint xStatus, yStatus;
  GLuint nextXstatus, nextYstatus;
  GLuint change; /* da capire se lo uso */

} World;

typedef struct{
  /* gestione della finestra*/
  GLfloat width, height;
  /* gestione dei frame */
  GLint frame, time, timebase, timerender;
  /* font utilizzato nel programma */
  GLint font;
  
  /* variabili utilizzate per l'OSD */
  GLchar fps[10];
  
  /* gestione flusso del programma */
  GLint exit;
  GLint menu;
  GLint fullscreen; 
  
} Data;


/*
 * variabili extern
 */

extern Data programData;
extern World worldData;

/*
 * prototipi funzioni
 */

/* esempio, meglio non mettere i nomi delle variabili -- int initMesh(Mesh *, char *);*/
GLbyte *gltLoadTGA(const char*, GLint*, GLint*, GLint*, GLenum*);

#endif
