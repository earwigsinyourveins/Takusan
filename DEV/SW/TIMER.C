#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "tags.h"
#include "sector.h"
#include "sprite.h"
#include "weapon.h"

void (__interrupt __far *oldkeyhandler)();
void __interrupt __far keyhandler(void);
volatile char keystatus[256], keyfifo[KEYFIFOSIZ], keyfifoplc, keyfifoend;
volatile char readch, oldreadch, extended, keytemp;

////////////////////////


// Apogee Sound Library
#define ASL 1

#if ASL

#include "task_man.h"
#include "fx_man.h"
#include "music.h"
#include "audiofx.h"

void TimerFunc(task * Task);
task *Task1 = NULL;
int Timer1;

#if 0
void 
TimerFunc(task * Task)
    {
    totalclock++;

    if (KEY_PRESSED(KEYSC_PERIOD))
        {
        extern unsigned long MoveThingsCount;
        MoveThingsCount++;
        MoveThingsCount--;
        return;
        }
    //keytimerstuff();
    }
#endif    

inittimer()
    {
    Timer1 = 0;
    Task1 = TS_ScheduleTask(&TimerFunc, 120, 1, &Timer1);
    TS_Dispatch();
    }

uninittimer()
    {
    //TS_Terminate(Task1);
    //TS_Terminate(0);
    //before you shutdown your memory system
    TS_Shutdown();
    }

#else
void (__interrupt __far * oldtimerhandler) ();
void __interrupt __far timerhandler(void);

inittimer()
    {
    outp(0x43, 54);
    outp(0x40, 9942 & 255);
    outp(0x40, 9942 >> 8);              // 120 times/sec

    oldtimerhandler = _dos_getvect(0x8);

    _disable();
    _dos_setvect(0x8, timerhandler);
    _enable();
    }

uninittimer()
    {
    outp(0x43, 54);
    outp(0x40, 255);
    outp(0x40, 255);                    // 18.2 times/sec

    _disable();
    _dos_setvect(0x8, oldtimerhandler);
    _enable();
    }

void __interrupt __far 
timerhandler()
    {
    totalclock++;
    //keytimerstuff();
    outp(0x20, 0x20);
    }

#endif

///////////////////////////////


