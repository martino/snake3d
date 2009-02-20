/*
 * Snake 3d -- event.h
 * 
 *
 */


#ifndef EVENT_H
#define EVENT_H

#include "share.h"


void event();
void mouseHandler(int, int);
/*
 * entrambi hanno come primo parametro il tasto premuto
 * mentre la X e la Y contengono 
 */
void keyHandler(GLubyte, GLint, GLint); // key x y
void specialKeyHandler(GLint, GLint, GLint); // key x y


#endif
