/*------------------------------------------------------------------------------
 * File: graphics_win32.c
 * Created: September 30, 2015
 * Last changed: September 30, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Linux-implementation av grafikbiblioteket.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifdef __linux__

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"
#include "core/debug.h"

#include <limits.h>

#include <X11/X.h>
#include <X11/Xlib.h>

#include <GL/gl.h>
#include <GL/glx.h>

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

static Atom wm_delete_msg;
static Display* display;
static Window* root;
static XVisualInfo* xvi;
static Colormap colormap;
static Window* window;
static GLXContext glx;

/*--------------------------------------
 * Variable: microSecsPerFrame
 *
 * Description:
 *   Antal mikrosekunder som varje bildruta ska visas.
 *------------------------------------*/
static int microSecsPerFrame;

/*--------------------------------------
 * Variable: initialized
 *
 * Description:
 *   Indikerar om grafiksystemet är initierat.
 *------------------------------------*/
static bool initialized;

/*--------------------------------------
 * Variable: keyPressCB
 *
 * Description:
 *   Callback-funktioner för olika knapptryck.
 *------------------------------------*/
static actionT keyPressCB[UCHAR_MAX];

/*--------------------------------------
 * Variable: keyPressArg
 *
 * Description:
 *   Argumentet till de olika callback-funktionerna.
 *------------------------------------*/
static void *keyPressArg[UCHAR_MAX];

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: InitGraphicsImpl()
 * Parameters:
 *
 * Description:
 *   Initierar grafik-läget.
 *------------------------------------*/
void InitGraphicsImpl() {
    display = XOpenDisplay(0);
    if (!display)
        Error("Could not connect to X server.");

    root = DefaultRootWindow(display);
    if (!root)
        Error("Could not get root window.");

    GLint attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    xvi = glXChooseVisual(display, 0, attr);
    if (!xvi)
        Error("Could not create visual.");

    colormap = XCreateColormap(display, root, xvi->visual, AllocNone);

    XSetWindowAttributes xswa;
    xswa.colormap   = colormap;
    xswa.event_mask = KeyPressMask;

    window = XCreateWindow(display, root, 0, 0, 600, 600, 0, xvi->depth, InputOutput, xvi->visual, CWColormap | CWEventMask, &xswa);

    XMapWindow(display, window);
    XStoreName(display, window, "Quickhull Sandbox");

    glx = glXCreateContext(display, xvi, NULL, GL_TRUE);
    glXMakeCurrent(display, window, glx);

    wm_delete_msg = XInternAtom(display, "WM_DELETE_WINDOW", False);
}

/*--------------------------------------
 * Function: WindowIsOpenImpl()
 * Parameters:
 *
 * Description:
 *   Returnerar sant om grafikfönstret är öppet.
 *------------------------------------*/
bool WindowIsOpenImpl() {
    return (window != NULL);
}

/*----------------------------------------------------------------------------*/
/* Ritfunktioner för att rensa ritytan, rita punkter, linjer, trianglar m.m.  */
/*----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: UpdateDisplayImpl()
 * Parameters:
 *
 * Description:
 *   Uppdaterar ritytan.
 *------------------------------------*/
void UpdateDisplayImpl() {
    glXSwapBuffers(display, window);

    XEvent xe;
    //XNextEvent(display, &xe);
    if (XCheckWindowEvent(display, window, KeyPressMask, &xe)) {
        if (xe.type == ClientMessage) {
            if (xe.xclient.data.l[0] == wm_delete_msg) {
                // Easy h4x, lol.
                window = NULL;
            }
        }
        else if (xe.type == MappingNotify) {
            printf("broder dang\n");
            XRefreshKeyboardMapping((XMappingEvent*)&xe);
        }
        else {
            char buf[16] = {0};
            KeySym keysym;
            XLookupString((XKeyEvent*)&xe, buf, sizeof(buf), &keysym, NULL);
            char c = buf[0];
            actionT fn = keyPressCB[c];
            if (fn) fn(keyPressArg[c]);
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Funktioner för input, t ex associera knappar med callback-funktioner m.m.  */
/*----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: OnKeyPressImpl()
 * Parameters:
 *   c    Tecknet som ska associeras till en funktion.
 *   cb   Callback-funktionen som ska anropas när knappen med det specificerade
 *        tecknet trycks ned.
 *   arg  Det argument som ska skickas till callback-funktionen.
 *
 * Description:
 *   Registerar en callback-funktion för en viss knapp.
 *------------------------------------*/
void OnKeyPressImpl(char c, actionT cb, void *arg) {
    keyPressCB [c] = cb;
    keyPressArg[c] = arg;
}

#endif // __linux__
