CFLAGS = -I. -I/usr/X11R6/include
LDFLAGS = -L/usr/X11R6/lib -lGL -lGLU -lglut -lm
CC = gcc

all: main

main: main.c  share.h event.c  loop.c  render.c  share.c event.h  loop.h  render.h 
