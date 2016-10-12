#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <X11/XKBlib.h>
/*X Libraries need to be included in this file otherwise causes error*/

#include "caps_lock_off.c"
#include "global.c"
#include "outputKeystroke.c"

typedef struct unic_struct unicode;

unicode uni;
int enableBaraha = 1;

// File descriptors for the pipe channel
int channel[2];

int main(int argc, char *argv[])
{
    int fd,pid;
    initialise();
    
    if(argc < 3) {
    	printf("usage: %s <mode> <device>\n", argv[0]);
    	return 1;
    }
    
    fd = open(argv[2], O_RDONLY);
    if(fd == -1) {
        printf("\nError in opening the file\n");
        exit(0);
    }      
    pipe(channel);
    pid = fork();
    if (pid == 0) {
    //within child
        outputKeystroke(argv[1][0]-'0');
        exit(0);
    }

    readInput(fd);      

    wait(NULL);
    return 0;
}


void readInput(int fd)
{   
    unicode uni;    
    short int bksp;
    
    struct input_event ev;
    
    close(channel[0]);
    
    while (1)
    {   
        read(fd, &ev, sizeof(struct input_event));
        if(ev.code == KEY_F8 && ev.value > 0)
        {
            enableBaraha *= -1;

            if(enableBaraha == -1)
                printf("\nSamskrita Lekhani Disabled\n");
            else
                printf("\nSamskrita Lekhani Enabled\n");
        }

/*
ev_value = 1 for press and 0 for release   
*/  
/*
    Following both if statements are embedded in the same block.
*/
        if(enableBaraha == 1)
        
        {       //block begins      
        if(ev.value > 0 && ev.type == 1 && isHandled(ev.code))
        {
        	transilerate(ev.code,uni.code,&bksp);
        	         
        	printf("\nUni.code = %s\n",uni.code);
        	uni.evcode = ev.code;

        	if(!(isShift(ev.code)))
        	{
        	    if(bksp)
        	    {
        	        printExChar("bksp",bksp);          	        
        	    }
        	    
        	    if(strcmp(uni.code,"Error"))    //non zero value for unequal
        	    write(channel[1], &uni, sizeof(uni));       	    

        	}

        	strcpy(uni.code,"Error");        	
        }
        
        if(ev.value == 0 && ev.type == 1 && (ev.code == KEY_LEFTSHIFT || ev.code ==KEY_RIGHTSHIFT))
        transilerate(ev.code,uni.code,&bksp);
        }   //enableBaraha block ends

    }   //while ends
}

void outputKeystroke(int mode)
{      
    int caps,charF;    
    Display *display;
    display = XOpenDisplay(NULL);
    close(channel[1]);
    
    printf("mode = %d\n",mode);

    charF = XKeysymToKeycode(display, 'F');
    while(1) {
    
        read(channel[0], &uni, sizeof(uni));        
        if(!strcmp(uni.code,"None"))
            continue;
        
        if (mode == 0 && !isBlacklisted(uni.evcode))
        {            
            uni.keycode[0] = XKeysymToKeycode(display, uni.code[1]);
            uni.keycode[1] = XKeysymToKeycode(display, uni.code[2]);
            uni.keycode[2] = XKeysymToKeycode(display, uni.code[3]);
            uni.keycode[3] = XKeysymToKeycode(display, uni.code[4]);
            printf("\nSending to printChar %d %d %d %d",uni.keycode[0],uni.keycode[1],uni.keycode[2],uni.keycode[3]);          

            if(uni.keycode[0] == charF || uni.keycode[1] == charF || uni.keycode[2] == charF || uni.keycode[3] == charF)
                printCharLong(uni.code,1);   
             else
                printChar(uni.keycode);
            
        }

        else
        {  
            printCharLong(uni.code,1);
        }
    }
}

/*
Difference between Samskrit and Kannada is = 896 or 0x380
*/

void transilerate(int code,char *unic,short int *bksp)
{   
    char *tmp,one_char;  
       
    int fully_matched=0, this_matched;
    int i, j, clear_buffer=0, again;
    int uniIdx;
    
    static char buf[10];
    static int buf_count, clear_count=0, shift=0, printed_count=0;
    static int last_matched=0, bksp_buf=0;
   
    extern char *aksharamAle[88];
    extern char *pureSwara[24];

/*
4 cases for transileration of any character

1. First Character
2. Intermediate Char
3. First Char with Shift on
4. Interm. char with Shift on

*/
    printf("\nWithin transilerate\n");
/*
Two cases for shift press
1. Shift is pressed and immediately released.
2. Shift is pressed, then character is pressed, then shift is released.
*/   
        *bksp = 0;
        
    if(code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT)
    {
        if(shift == 1)
        shift = 0;
        else
        shift = 1;
        return;
    }   

    if(isWhitespace(code) || code == KEY_BACKSPACE || code == KEY_DELETE)
    {
        memset(&buf[0],0,sizeof(buf));
        buf_count = 0;
        shift = 0;
        printed_count = 0;
	last_matched = 0;
        return;
    }

    if(shift == 1)
    {
        if(code == KEY_GRAVE)
        buf[buf_count++] = '~';
        
        else if(code == KEY_6)
        buf[buf_count++] = '^';
        
        else if(code == KEY_DOT)
        buf[buf_count++] = '.';
        
        else if(isalpha(codeTable[code]))
        buf[buf_count++] = toupper(codeTable[code]);        
        
    }
    else
    {       
        buf[buf_count++] = codeTable[code];
    }

/*To print pureSwara. pureSwara means the full swara and not just the symbol*/
    if(isSwara(buf,&uniIdx,buf_count) && printed_count == 0)
    {
        if(isSwara(buf,&uniIdx,9))
        {
            strcpy(unic,pureSwara[uniIdx]);
            *bksp = bksp_buf;
            bksp_buf = 0;
            printf("\nCharacter is : %c",buf[buf_count-1]);
            printf("\npureSwara sending");
            printed_count++;
	    last_matched = 1;
        }
        else
       {
            bksp_buf++;   
        }
                     
        clear_count++;       
        return;           
    }
    
/*check for longest matching substring

1. Complete String can match
2. Complete mis-match
3. Partial match
*/ 


//AGAIN:
    do{
    
    /*this_matched keeps track of whether there are more than one matches*/
    again = 0;
    this_matched = 0;
    for(i=0;i<86;i++)
    {
        printf("\nbuf = %s, akshara = %s",buf,aksharamAle[i]);
        if(!strncmp(buf,aksharamAle[i],buf_count))
        {
            tmp = unicode_aksharamAle[i];
            this_matched = 1;
            if(!strcmp(buf,aksharamAle[i]))
            {
                fully_matched = 1;
                uniIdx = i;
                break;
            }
        }    
        
    }

    if((last_matched == 1 && this_matched == 0) || buf_count > MAXBUF)
    /*means that last time fully matched but now complete mismatch*/
    {
        clear_buffer = 1;
    }

    if(clear_buffer == 1)
    {   
        one_char = buf[buf_count-1];
        
        memset(&buf[0],0,sizeof(buf));
        buf[0] = one_char;
        buf_count=1;
        clear_buffer = 0;
        last_matched = 0;
        again = 1;
        //goto AGAIN;
    }
    
    }while(again == 1);        
        
    if(isSwara(buf,&i,9) && printed_count > 0)    
    {   
 
        if(!strcmp(buf,"a"))
            *bksp += 1;
            
/*Special handling for LLi and LLI*/
        if(!strcmp(buf,"LLi") || !strcmp(buf,"L^i"))
        {
            strcpy(unic,tmp);
            *bksp += 2;
            printed_count++;
            memset(&buf[0],0,sizeof(buf));
            buf_count = 0;
            return;
        }
        else if(!strcmp(buf,"LLI") || !strcmp(buf,"L^I"))
        {
            strcpy(unic,tmp);
            *bksp += 2;
            printed_count++;
            memset(&buf[0],0,sizeof(buf));
            buf_count = 0;
            return;
        }   
/*to delete the first character in a swara having length = 2*/
        if(buf_count == 2 && buf[buf_count-2] != 'a')
            *bksp += 1; 
    }
    
    if(fully_matched == 1)
    {
        strcpy(unic,tmp);
        printf("\nComplete match\n");
        printed_count++;    
        if(last_matched == 1 && buf[buf_count-1] == 'h' && buf_count>1)
        /*condition to delete the first character in case of 'ch' etc.*/
        {
            *bksp +=1;
        }
  
        *bksp += bksp_buf;
        bksp_buf = 0;                           
    }
    
    else if(this_matched == 1)
    {        
        strcpy(unic,"Error");
        printf("\nIncomplete match\n");
        
        bksp_buf++;
    }
    
    else
    {
        strcpy(unic,"Error");
        printf("\nError: Token not recognised\n");        
    }
    
    last_matched = fully_matched;
    clear_count++;
    printf("\nbksp = %d",*bksp);
}

