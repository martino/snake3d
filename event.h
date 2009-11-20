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
void keyHandler(GLubyte, GLint, GLint); // key x y
void specialKeyHandler(GLint, GLint, GLint); // key x y


#endif
