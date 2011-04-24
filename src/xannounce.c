/*
	AUTHOR:		Trevor Woerner
	DATE:		05 January 1997
	FILENAME:	xannounce.c
	PURPOSE:	scrolls an announcement until clicked
	MEMORANDUM:
		This program was written to mark the unfortunate passing away of my Aunt Rita Smith
		who, at approximately 9:30 this morning, after a long and painful battle with cancer,
		simply stopped breathing
*/

#include "xannounce.h"

XEvent			thisEvent;
Display			*mainDisplay_ptr;
Window			mainWindow;
GC				textGC;
XFontStruct		*textFont_ptr;
short			textHeight,stringWidth;
unsigned long	fgPixel,bgPixel;
char			*displayName_ptr = NULL,
				*windowNameStr_ptr = "**** Announcing ****",
				stringMessage[300];
int				stringLength;
XTextProperty	textProp;

int main( int argc, char **argv )
{
	int				getNextEvent=1;
	unsigned long	mainWindowEventMask;
	Bool			waitingEvent;
	int				i,j,scrollDelay=3000;
	time_t			endTime,startTime;
	double			deltaTime;

	if( argc != 2 ) {
		fprintf( stderr, "usage: %s \"string\"\n", argv[0] );
		exit( 1 );
	}
	strcpy( stringMessage, argv[1] );

	if( (mainDisplay_ptr = XOpenDisplay( displayName_ptr )) == NULL ) {
		fprintf( stderr, "could not open display\n" );
		exit( 1 );
	}

	fgPixel = BlackPixel( mainDisplay_ptr, DefaultScreen( mainDisplay_ptr ));
	bgPixel = WhitePixel( mainDisplay_ptr, DefaultScreen( mainDisplay_ptr ));

	mainWindow = XCreateSimpleWindow( mainDisplay_ptr, DefaultRootWindow(mainDisplay_ptr), 1, 1,
					mainWinDims.width, mainWinDims.height, 1, fgPixel, bgPixel );
	textGC = XCreateGC( mainDisplay_ptr, mainWindow, 0, NULL );
	if( (textFont_ptr = XLoadQueryFont( mainDisplay_ptr, "12x24" )) == NULL ) {
		fprintf( stderr, "can't load font\n" );
		exit( 1 );
	}
	XSetFont( mainDisplay_ptr, textGC, textFont_ptr->fid );
	textHeight = (short)(textFont_ptr->max_bounds.ascent + textFont_ptr->max_bounds.descent);
	stringLength = strlen( stringMessage );
	stringWidth = (short)(textFont_ptr->max_bounds.width * stringLength);
	textData.y = (mainWinDims.height>>1) + (textHeight>>1);

	XStringListToTextProperty( &windowNameStr_ptr, 1, &textProp );
	XSetWMName( mainDisplay_ptr, mainWindow, &textProp );
	mainWindowEventMask = ExposureMask | ButtonReleaseMask | ResizeRedirectMask;
	XSelectInput( mainDisplay_ptr, mainWindow, mainWindowEventMask );
	XMapWindow( mainDisplay_ptr, mainWindow );

	while( getNextEvent ) {
		if( (waitingEvent = XCheckMaskEvent( mainDisplay_ptr, mainWindowEventMask, &thisEvent )) == True ) {
			if( thisEvent.xany.window == mainWindow ) {
				switch( thisEvent.type ) {
					case Expose:
						if( thisEvent.xexpose.count == 0 )
							DrawText();
						break;
					case ButtonRelease:
						getNextEvent = 0;
						break;
					case ResizeRequest:
						XResizeWindow( mainDisplay_ptr, mainWindow, mainWinDims.width, mainWinDims.height );
						break;
				}
			}
		} else {
			XClearArea( mainDisplay_ptr, mainWindow, textData.x, (int)(textData.y-textFont_ptr->max_bounds.ascent),
							0, (unsigned int)textHeight, False );
			if( textData.x > -(stringWidth) )
				--textData.x;
			if( textData.x == -(stringWidth) )
				textData.x = mainWinDims.width;
			DrawText();
			sleep_us( 250 );
		}
	}

	XFreeGC( mainDisplay_ptr, textGC );
	XCloseDisplay( mainDisplay_ptr );
	return 0;
}

void DrawText( void )
{
	XClearArea( mainDisplay_ptr, mainWindow, textData.x, (int)(textData.y-textFont_ptr->max_bounds.ascent), 0, textHeight, False );
	XDrawString( mainDisplay_ptr, mainWindow, textGC, textData.x, textData.y, stringMessage, stringLength );
}

void sleep_us( unsigned int usecs )
{
	struct timeval tval;

	tval.tv_sec = usecs / 1000000;
	tval.tv_usec = usecs % 1000000;
	select( 0, NULL, NULL, NULL, &tval );
}
