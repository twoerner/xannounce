/*
	AUTHOR:		Trevor Woerner
	DATE:		05 January 97
	FILENAME:	xannounce.h
	PURPOSE:	to make an announcement
*/

#ifndef X_ANNOUNCE_H
#define X_ANNOUNCE_H

#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct {
	unsigned int	width, height;
} mainWinDims = { 350, 50 };

struct {
	int				x,y;
} textData;

void DrawText( void );
void sleep_us( unsigned int );

#endif
