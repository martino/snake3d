/*
 * Snake 3d -- render.h
 * 
 *
 */


#ifndef RENDER_H
#define RENDER_H

#include "share.h"

void render();
void changeSize(GLint, GLint);
void setOrtographicProjection();
void resetPerspectiveProjection();
void drawWorld();
void createWorld();
void renderText(float, float, char *);
#endif
