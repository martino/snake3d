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
 * include comuni
 */
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

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


/*
 * prototipi funzioni
 */

/* esempio, meglio non mettere i nomi delle variabili -- int initMesh(Mesh *, char *);*/


#endif
