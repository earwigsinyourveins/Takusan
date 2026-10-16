#include <stdlib.h>
#include <stdio.h>

#include "build.h"
#include "keys.h"
#include "mytypes.h"
#include "game.h"
#include "usrhooks.h"
#include "def.h"

void
TimerFunc(task * Task)
    {
    totalclock++;
    }






/*** VERSIONS RESTORATION - RECREATED HOOKS.C CODE ***/






extern char lumplockbyte[];

///
/// Routine called with a sound is finished playing
///

void
SoundCallBack(long num)
    {
    VOC_INFOp vp = &voc[num];
    
    // RTS sounds are negative
    if (num < 0)
        {
        ASSERT(-num < 11);
        lumplockbyte[-num]--;
        ASSERT(lumplockbyte[-num] != 255/*CACHE_LOCK_MAX*/);
        return;
        }

    // Update counter
    //vp->playing--;
    vp->lock--;
    }



/*---------------------------------------------------------------------
   Function: USRHOOKS_GetMem

   Allocates the requested amount of memory and returns a pointer to
   its location, or NULL if an error occurs.  NOTE: pointer is assumed
   to be dword aligned.
---------------------------------------------------------------------*/

int USRHOOKS_GetMem
   (
   void **ptr,
   unsigned long size
   )

   {
   void *memory;

   //memory = AllocMem( size );
   memory = malloc( size );

   //LoadFile routine allocs memory without this routine so
   //cant do AllocMem

   if ( memory == NULL )
      {
      TerminateGame();
      printf("USRHOOKS_GetMem out of memory");
      exit(0);
      }

   *ptr = memory;

    ////DSPRINTF(ds,"User Hooks %d, %ud",size,*ptr);
    //MONO_PRINT(ds);
   

   return( USRHOOKS_Ok );
   }


/*---------------------------------------------------------------------
   Function: USRHOOKS_FreeMem

   Deallocates the memory associated with the specified pointer.
---------------------------------------------------------------------*/

int USRHOOKS_FreeMem
   (
   void *ptr
   )

   {
   if ( ptr == NULL )
      {
      TerminateGame();
      printf("USRHOOKS_FreeMem freed a null pointer");
      exit(0);
      }

    ////DSPRINTF(ds,"Free Hooks %ud",ptr);
    //MONO_PRINT(ds);
      
   //FreeMem( ptr );
   free( ptr );

   return( USRHOOKS_Ok );
   }


