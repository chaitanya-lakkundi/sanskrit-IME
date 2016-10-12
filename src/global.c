#include <linux/input.h>    //struct input_event
#include <sys/types.h>      //u_short u_long
#include <sys/wait.h>       //wait()
#include <fcntl.h>          // O_RDONLY
#include <unistd.h>         //for read() 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXBUF 3
/*MAXBUF is the max number of chars in buf[] after which buf is cleared*/

#define between(a,x,y) ((a>=x)?((a<=y)?1:0):0)
#define isShift(x) ((x==KEY_LEFTSHIFT || x==KEY_RIGHTSHIFT)?1:0)
#define isWhitespace(x) ((x == KEY_SPACE || x == KEY_TAB || x == KEY_KPENTER || x == KEY_ENTER)?1:0)

#define isBlacklisted(c) ((c == KEY_U || c == KEY_F || c == KEY_J \
|| c == KEY_Q || c == KEY_6 || c == KEY_9 || isWhitespace(c) )?1:0)
/*if isBlacklisted(c)==True then mode 1 is compulsarily used*/

#define isHandled(c) (between(c,KEY_Q,KEY_P) || between(c,KEY_A,KEY_L) || between(c,KEY_Z,KEY_M) || isShift(c) || isWhitespace(c) || c == KEY_GRAVE || c == KEY_DOT || between(c,KEY_1,KEY_0) || c == KEY_BACKSPACE) || c == KEY_DELETE
/*isHandled takes care of which characters are allowed into transilerate*/



/*
#define O_RDONLY	0x0000
#define O_WRONLY	0x0001
#define O_RDWR		0x0002

struct timeval {
	long	tv_sec;		// seconds
	long	tv_usec;	// and microseconds
};

struct input_event {
	struct timeval time;
	unsigned short type;
	unsigned short code;
	unsigned int value;
};
*/

void printChar(int *);
void printCharLong(char *code,int bksp);
void outputKeystroke(int mode);     //child process
void readInput(int fd);             //master process
void printExChar(char *whichChar,int count);
void initialise(void);
void transilerate(int code,char *unic, short int *bksp);
int isSwara(char *akshara, int *idx, int buf_count);

struct unic_struct{
	char code[40];
	int keycode[4];
	int evcode;
};


char codeTable[60];


void initialise(void)
{
codeTable[2]='1';
codeTable[3]='2';
codeTable[4]='3';
codeTable[5]='4';
codeTable[6]='5';
codeTable[7]='6';
codeTable[8]='7';
codeTable[9]='8';
codeTable[10]='9';
codeTable[11]='0';

codeTable[16]='q';
codeTable[17]='w';
codeTable[18]='e';
codeTable[19]='r';
codeTable[20]='t';
codeTable[21]='y';
codeTable[22]='u';
codeTable[23]='i';
codeTable[24]='o';
codeTable[25]='p';

codeTable[30]='a';
codeTable[31]='s';
codeTable[32]='d';
codeTable[33]='f';
codeTable[34]='g';
codeTable[35]='h';
codeTable[36]='j';
codeTable[37]='k';
codeTable[38]='l';

codeTable[41]='~';

codeTable[44]='z';
codeTable[45]='x';
codeTable[46]='c';
codeTable[47]='v';
codeTable[48]='b';
codeTable[49]='n';
codeTable[50]='m';

codeTable[52]='.';
}
/*
aksharamAle contains all ITRANS characters. 
My own characters are added at the end of the array.
*/
char * aksharamAle[88] = {"a","aa","A","i","ii","I","u","uu","U","RRi","R^i",
"RRI","R^I","LLi","L^i","LLI","L^I","e","ai","o","au","ou","aM","aH","k","kh","g",
"gh","~N","ch","Ch","j","jh","~n","T","Th","D","Dh","N","t","th","d","dh","n",
"p","ph","b","bh","m","y","r","l","L","v","w","sh","Sh","s","h","xkSh","x","xGY",
"xj~n","xdny","xshr","M","H",".n",".m",".a",".c",".N",".h","OM","AUM","0","1","2",
"3","4","5","6","7","8","9","f"};

/*
By default array contains swara signs instead os swaras themselves because
frequency of signs is more than the swaras themselves.
*/

char * unicode_aksharamAle[100] = {"None","U093E","U093E","U093F","U0940",
"U0940","U0941","U0942","U0942","U0943","U0943","U0944","U0944","U0962",
"U0962","U0963","U0963","U0947","U0948","U094B","U094C","U094C","U0902","U0903",
"U0915","U0916","U0917","U0918","U0919",
"U091A","U091B","U091C","U091D","U091E",
"U091F","U0920","U0921","U0922","U0923",
"U0924","U0925","U0926","U0927","U0928",
"U092A","U092B","U092C","U092D","U092E",
"U092F","U0930","U0932","U0933","U0935","U0935",
"U0936","U0937","U0938","U0939",
"UFFFD","UFFFD","UFFFD","UFFFD","UFFFD","UFFFD",
"U0902","U0903","U0902","U0902","U093D","U0945","U0901","U094D","U0950",
"U0950","U0966","U0967","U0968","U0969","U096A","U096B","U096C","U096D",
"U096E","U096F","U094D"
};

char * pureSwara[24] = {"U0905","U0906","U0906","U0907","U0908",
"U0908","U0909","U090A","U090A","U090B","U090B","U0960","U0960","U090C",
"U090C","U0961","U0961","U090F","U0910","U0913","U0914","U0914"};


int isSwara(char *akshara,int *idx,int buf_count)
{
    char * swara[] = {"a","aa","A","i","ii","I","u","uu","U","RRi","R^i",
"RRI","R^I","LLi","L^i","LLI","L^I","e","ai","o","au","ou"};
    int i=0,flag = 0;
    
    for(i=0;i<22;i++)
    {
        if(buf_count == 9)
        {
           if(!strcmp(swara[i],akshara))
           {
                printf("\nFound a pure swara\n");
                *idx = i;                            
                flag = 1;
                break;
           }
         
        }
        
        else if(!strncmp(swara[i],akshara,buf_count))
        {
            printf("\nFound a partial swara\n");
            *idx = i;            
            flag = 1;
            break;
        }
    }
    
    return flag;
    
}
