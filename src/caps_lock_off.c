/*
#include <stdlib.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>
*/
int xtest_get_capslock_state()
{
    Display* dpy;
    dpy = XOpenDisplay( NULL );
    
    int i;
    int capslock_mask = 0;
    Window dummy1, dummy2;
    int dummy3, dummy4, dummy5, dummy6;
    unsigned int mask;
    XModifierKeymap* map = XGetModifierMapping( dpy );
    KeyCode capslock_keycode = XKeysymToKeycode( dpy, XK_Caps_Lock );
    if( capslock_keycode == NoSymbol )
        return 0;
    for( i = 0;
         i < 8;
         ++i )
        {
	if( map->modifiermap[ map->max_keypermod * i ] == capslock_keycode )
		capslock_mask = 1 << i;
	}
    XQueryPointer( dpy, DefaultRootWindow( dpy ), &dummy1, &dummy2,
        &dummy3, &dummy4, &dummy5, &dummy6, &mask );
    XFreeModifiermap( map );
    return mask & capslock_mask;
}
