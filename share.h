/*
 * Snake 3d -- share.h
 * 
 * Definiamo qui tutte le strutture del programma e le 
 * funzioni "utili"
 *
 */


#ifndef SHARE_H
#define SHARE_H

/**
 * costanti
 */

#define winHeight 600
#define winWidth  800


/*
 * include OpenGL
 */
// Mac OS X
#ifdef __APPLE__
#include <OpenGL/gl.h>		// Apple OpenGL haders (version depends on OS X SDK version)
#include <OpenGL/glu.h>		// OpenGL Utilities
#include <Glut/glut.h>		// Apples Implementation of GLUT
#elif
#include <GL/glut.h>
#endif

/*
 * include comuni
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * include degli altri file
 */

#include "render.h"
#include "event.h"
#include "loop.h"


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

} World;

typedef struct{
  /* gestione della finestra*/
  GLfloat width, height;
  /* gestione dei frame */
  GLint frame, time, timebase, timerender;
  /* font utilizzato nel programma */
  GLint font;
  
  /* variabili utilizzate per l'OSD */
	
  /* gestione flusso del programma */
  GLint exit;
  GLint menu;
  GLint fullscreen; 
} Data;


/*
 * variabili extern
 */

extern Data programData;

/*
 * prototipi funzioni
 */

/* esempio, meglio non mettere i nomi delle variabili -- int initMesh(Mesh *, char *);*/


#endif
