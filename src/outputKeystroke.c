void printChar(int keycode[4])
{   
    Display *display;
    int bksp,ctrl,shift,charU,caps;
    
    display = XOpenDisplay(NULL);
     
    bksp  = XKeysymToKeycode(display, XK_BackSpace);
    ctrl  = XKeysymToKeycode(display, XK_Control_L);
    shift = XKeysymToKeycode(display, XK_Shift_L);
    charU = XKeysymToKeycode(display, XK_U);
    caps  = XKeysymToKeycode(display,XK_Caps_Lock);
 
    XFlush( display );

    printf("\nPrinting devanagari");           

/*To turn off caps lock if ON */
    if(xtest_get_capslock_state())
    {   
           
        XTestFakeKeyEvent(display, caps, True, 0);
        XTestFakeKeyEvent(display, caps, False, 0);
        printf("\nCaps Lock OFF");
    }
    
    XTestFakeKeyEvent(display, bksp, True, 0);
    XTestFakeKeyEvent(display, bksp, False,0);
    
    XTestFakeKeyEvent(display, ctrl, True, 0);        
    XTestFakeKeyEvent(display, shift, True, 0);
    
    XTestFakeKeyEvent(display, charU, True, 0);
    XTestFakeKeyEvent(display, charU, False, 0);
    
    XTestFakeKeyEvent(display, keycode[0], True, 0);
    XTestFakeKeyEvent(display, keycode[0], False, 0);

    XTestFakeKeyEvent(display, keycode[1], True, 0);
    XTestFakeKeyEvent(display, keycode[1], False,0);

    XTestFakeKeyEvent(display, keycode[2], True,0);
    XTestFakeKeyEvent(display, keycode[2], False,0);

    XTestFakeKeyEvent(display, keycode[3], True, 0);
    XTestFakeKeyEvent(display, keycode[3], False, 0);    

    XTestFakeKeyEvent(display, shift, False, 0);
    XTestFakeKeyEvent(display, ctrl, False,0);
    
    XFlush( display );
    XCloseDisplay( display );
/*if this option is not enabled then Maximum no. of clients reached error*/
}

void printCharLong(char *arg,int bksp)
{   
    Display *display;
    display = XOpenDisplay(NULL);
    KeySym sym = XStringToKeysym(arg);

    int min, max, numcodes;
    XDisplayKeycodes(display,&min,&max);
    KeySym *keysym;

    keysym = XGetKeyboardMapping(display,min,max-min+1,&numcodes);
    keysym[(max-min-1)*numcodes]=sym;
    XChangeKeyboardMapping(display,min,numcodes,keysym,(max-min));
    XFree(keysym);
    XFlush( display );

    KeyCode code = XKeysymToKeycode(display,sym);
    
    if(bksp)
    {
    XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_BackSpace), True, 0);
    XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_BackSpace), False, 0);
    }
    
    XTestFakeKeyEvent(display, code, True, 0);
    XTestFakeKeyEvent(display, code, False, 0);

    XFlush( display );
    XCloseDisplay( display );
}


void printExChar(char *whichKey,int count)
{
    Display *display;
    int bksp,ctrl,shift,charU,charV;
    
    display = XOpenDisplay(NULL);
    
    bksp  = XKeysymToKeycode(display, XK_BackSpace);

    while(count)
    {
        if(!strcmp(whichKey,"bksp"))
        {
            printf("\nPrint %d backspace(s)",count);
            
            XTestFakeKeyEvent(display, bksp, True, 0);
            XTestFakeKeyEvent(display, bksp, False,0);
            
            XFlush( display );
            XCloseDisplay( display );
        }
        
        count--;
    }    
}
