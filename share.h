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

#ifdef __APPLE__                // Mac OS X
#include <OpenGL/gl.h>		// Apple OpenGL haders (version depends on OS X SDK version)
#include <OpenGL/glu.h>		// OpenGL Utilities
#include <Glut/glut.h>		// Apples Implementation of GLUT
#else                           // Linux
#include <GL/glut.h>
#endif

/*
 * include comuni
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

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
#define TR  0
#define TG  1
#define TB  2
#define TMY 3
#define TW  4
#define TP  5
#define TO  6
#define TM  7

#define NTEX  8
#define WORLDIM 200.0f
#define PIOVER180 0.0174532925f
#define WORMDIA 3.5f
#define DIA     7.0f
#define VELOCITY 15


/*
 * strutture dati
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
  GLfloat x, y,z; /* centro */
  GLint   timer;  /* timer */
  GLint   texture;/* texture utilizzata */
} Sphere;

struct WSphereStruct;

typedef struct WSphereStruct{
  struct WSphereStruct *next;
  struct WSphereStruct *prev;

  /* data */
  GLfloat x, y, z;

} WSphere;

typedef struct{
  WSphere *head;/*testa*/
  WSphere *tail;/*coda*/
  
  // velocita
  GLfloat vel;
  // dimensione
  GLfloat dim;
  GLfloat dia;
} Worm;


typedef struct{
  /* texture */
  GLuint texObj[NTEX];
  GLuint wall, wallsd, ground, sky, ball, worm;
  
  GLuint texWall, texWallSd;

  /* luci */
  GLfloat ambientLight[4];
  GLfloat diffuseLight[4];
  GLfloat specularLight[4];
  GLfloat positionA[4];

  /* materiali */
  GLfloat specref[4];

  /* telecamera */
  GLfloat angleX, angleY;
  GLfloat angleMX, angleMY;
  GLfloat nextAngleX, nextAngleY;
  GLfloat nextAngleMX, nextAngleMY;
  
  GLuint kup, kdown, kleft, kright;
  GLuint xStatus, yStatus;
  GLuint nextXstatus, nextYstatus;

  
  /* quadric object*/
  GLUquadricObj *q;

} World;

typedef struct{
  /* gestione della finestra*/
  GLfloat width, height;
  /* gestione dei frame */
  GLint frame, time, timebase, timerender;
  /* velocita' di gioco */
  GLint velocity;
  GLint timeFrame;
  GLfloat velAngolare;
  /* font utilizzato nel programma */
  GLint font;
  
  /* variabili utilizzate per l'OSD */
  GLchar fps[10];
  GLchar pointsOSD[100];
  
  /* gestione flusso del programma */
  GLint exit;
  GLint menu;
  GLint fullscreen; 
  GLint gameStatus; /* flag per la gestione della sconfitta*/
  GLint points;
  GLint invert;


  /* effetti grafici */
  GLint multisample;
  GLint fog;

} Data;


/*
 * variabili extern
 */

extern Data  programData;
extern World worldData;
extern Worm  myWorm;
extern Sphere ball;

/*
 * prototipi funzioni
 */

/* esempio, meglio non mettere i nomi delle variabili -- int initMesh(Mesh *, char *);*/
GLbyte *gltLoadTGA(const char*, GLint*, GLint*, GLint*, GLenum*);
GLint gltWriteTGA(const char *szFileName);
/* gestione verme */
int initializeWorm(GLfloat, GLfloat, GLfloat, GLfloat);
int increaseWorm(GLfloat, GLfloat, GLuint);
void destroyWorm();
int moveWorm(GLfloat, GLfloat, GLuint);
void printWorm();
/* gestione sfere */
int initializeBall();
void generateBall();
void destroyBall();
GLdouble dist2Point(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
void increaseVel(GLint);
int randomNBall();
#endif
