/*
 * Copyright (C) 2011  Trevor Woerner
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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
static bool find_font (void);
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
	if (find_font () == false)
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

static bool
find_font (void)
{
	const char *fontNameStrings_p[] = { fontName_pG, "12x24", "lucidasans-18", "fixed" };
	unsigned fontNameStringsCnt = sizeof (fontNameStrings_p) / sizeof (fontNameStrings_p[0]);
	unsigned i;

	for (i=0; i<fontNameStringsCnt; ++i) {
		if (fontNameStrings_p[i] == NULL)
				continue;

		textFont_p = XLoadQueryFont (dpy_p, fontNameStrings_p[i]);
		if (textFont_p == NULL)
			fprintf (stderr, "can't load font: %s\n", fontNameStrings_p[i]);
		else {
			printf ("using font: %s\n", fontNameStrings_p[i]);
			break;
		}
	}

	if (textFont_p == NULL)
		return false;
	return true;
}

static void
draw_text (void)
{
	XClearArea (dpy_p, mainWin, textData.x, (int)(textData.y-textFont_p->max_bounds.ascent),
			0, textHeight, False);
	XDrawString (dpy_p, mainWin, textGC, textData.x, textData.y, stringMessage_p, stringLength);
}
