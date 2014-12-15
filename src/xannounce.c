/*
 * Copyright (C) 2011  Trevor Woerner
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

//******************************************************//
// structures
//******************************************************//
struct {
	unsigned width, height;
} mainWinDims = {
	350, 50
};

struct {
	int x, y;
} textData;

//******************************************************//
// prototypes
//******************************************************//
static void draw_text (void);

//******************************************************//
// globals
//******************************************************//
Display *dpy_p;
Window mainWin;
GC textGC;
XFontStruct *textFont_p = NULL;
unsigned textHeight;
char *stringMessage_p;
int stringLength;
static char *fontName_pG = NULL;

//******************************************************//
// meat and potatoes
//******************************************************//
int
main (int argc, char *argv[])
{
	int getNextEvent = 1;
	long mainWindowEventMask;
	Bool waitingEvent;
	XEvent thisEvent;
	short stringWidth;
	unsigned long fgPixel, bgPixel;
	char *windowNameStr_p = "**** Announcing ****";
	XTextProperty textProp;

	if (argc == 3) {
		fontName_pG = argv[2];
	}
	else {
		if (argc != 2) {
			fprintf (stderr, "usage: %s \"string\"\n", argv[0]);
			exit (1);
		}
	}
	stringMessage_p = strdup (argv[1]);

	dpy_p = XOpenDisplay (NULL);
	if (dpy_p == NULL) {
		fprintf (stderr, "could not open display\n");
		exit (1);
	}

	fgPixel = BlackPixel (dpy_p, DefaultScreen (dpy_p));
	bgPixel = WhitePixel (dpy_p, DefaultScreen (dpy_p));

	mainWin = XCreateSimpleWindow (dpy_p, DefaultRootWindow (dpy_p), 1, 1,
			mainWinDims.width, mainWinDims.height, 1, fgPixel, bgPixel);
	textGC = XCreateGC (dpy_p, mainWin, 0, NULL);
	if (fontName_pG != NULL) {
		textFont_p = XLoadQueryFont (dpy_p, fontName_pG);
		fprintf (stderr, "%s font: \"%s\"\n", textFont_p == NULL? "can't load" : "using", fontName_pG);
	}
	if (textFont_p == NULL) {
		textFont_p = XLoadQueryFont (dpy_p, "12x24");
		fprintf (stderr, "%s font: \"12x24\"\n", textFont_p == NULL? "can't load" : "using");
	}
	if (textFont_p == NULL) {
		textFont_p = XLoadQueryFont (dpy_p, "lucidasans-18");
		fprintf (stderr, "%s font: \"lucindasans-18\"\n", textFont_p == NULL? "can't load" : "using");
	}
	if (textFont_p == NULL) {
		textFont_p = XLoadQueryFont (dpy_p, "fixed");
		fprintf (stderr, "%s font: \"fixed\"\n", textFont_p == NULL? "can't load" : "using");
	}
	if (textFont_p == NULL)
		exit (1);
	XSetFont (dpy_p, textGC, textFont_p->fid);
	textHeight = (unsigned)(textFont_p->max_bounds.ascent + textFont_p->max_bounds.descent);
	stringLength = (int)strlen (stringMessage_p);
	stringWidth = (short)(textFont_p->max_bounds.width * stringLength);
	textData.y = (int)(mainWinDims.height >> 1) + (int)(textHeight >> 1);

	XStringListToTextProperty (&windowNameStr_p, 1, &textProp);
	XSetWMName (dpy_p, mainWin, &textProp);
	mainWindowEventMask = ExposureMask | ButtonReleaseMask | ButtonPressMask | KeyPressMask | ResizeRedirectMask;
	XSelectInput (dpy_p, mainWin, mainWindowEventMask);
	XMapWindow (dpy_p, mainWin);

	while (getNextEvent) {
		waitingEvent = XCheckMaskEvent (dpy_p, mainWindowEventMask, &thisEvent);
		if (waitingEvent == True) {
			if (thisEvent.xany.window == mainWin) {
				switch (thisEvent.type) {
					case Expose:
						if (thisEvent.xexpose.count == 0)
							draw_text ();
						break;

					case ButtonRelease:
					case ButtonPress:
					case KeyPress:
						getNextEvent = 0;
						break;

					case ResizeRequest:
						XResizeWindow (dpy_p, mainWin,
								mainWinDims.width,
								mainWinDims.height);
						break;
				}
			}
		}
		else {
			XClearArea (dpy_p, mainWin, textData.x,
					(int)(textData.y - textFont_p->max_bounds.ascent),
					0, textHeight, False);
			if (textData.x > -(stringWidth))
				--textData.x;
			if (textData.x == -(stringWidth))
				textData.x = (int)mainWinDims.width;
			draw_text ();
			usleep (8500);
		}
	}

	free (stringMessage_p);
	XFree (textProp.value);
	XFreeFont (dpy_p, textFont_p);
	XFreeGC (dpy_p, textGC);
	XCloseDisplay (dpy_p);
	return 0;
}

static void
draw_text (void)
{
	XClearArea (dpy_p, mainWin, textData.x, (int)(textData.y-textFont_p->max_bounds.ascent),
			0, textHeight, False);
	XDrawString (dpy_p, mainWin, textGC, textData.x, textData.y, stringMessage_p, stringLength);
}
