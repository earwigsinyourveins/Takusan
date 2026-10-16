#include <string.h>
#include "mytypes.h"
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
//#include "panel.h"
#include "game.h"

// macros this function needs
//#define MK_FP(seg,ofs)       ((void _seg *)(seg)+(void near *)(ofs))
//#define PokeByte(a,b,c)      (*((char far *)MK_FP((a),(b)))=(char)(c))

//#if DEBUG
VOID 
PokeStringMono(BYTE Attr, BYTEp String)
    {
    // Scrolls the monochrome display up one line, then prints the
    // string with the desired attribute on the bottom line.

    // EXAMPLE: PokeStringMono(MDA_NORMAL, "Hello, world.");

    BYTEp Src, Dest;
    LONGp s,d;
    static char MonoBuf[4000];
    
    #define BASE (MonoBuf)
    #define LINE_SIZE 160
    #define LINE_OFFSET(num) ((80*2)*(num))
    
    // First scroll the screen up one line.
    // copy lines 1-24 up to 0
    s = (LONGp)(BASE + LINE_SIZE);
    d = (LONGp)BASE;
    memmove(d,s,LINE_OFFSET(24));

    // clear bottom line
    Dest = BASE + LINE_OFFSET(24);
    //memset(Dest,0,LINE_SIZE);
    memset(Dest,'.',LINE_SIZE);

    // Now print the string on the bottom line.
    Src = String;
    Dest = BASE + LINE_OFFSET(24);
    
    while (*Src)
        {
        *Dest++ = *Src++;
        *Dest++ = Attr;
        }
    
    memcpy((char *)0xB0000, MonoBuf, sizeof(MonoBuf));    
    }
//#endif
    
