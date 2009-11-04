/*
 * Snake 3d -- loop.h
 * 
 *
 */


#ifndef LOOP_H
#define LOOP_H

#include "share.h"


void loop();
int checkWall(float, float, int);
int sCollisionDetection(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

#endif
