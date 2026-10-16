#include  "pragmas.h"
#include "interp.h"

#define SHORT_MAXINTERPOLATIONS 256
short short_numinterpolations = 0, short_startofdynamicinterpolations = 0;
short short_oldipos[SHORT_MAXINTERPOLATIONS];
short short_bakipos[SHORT_MAXINTERPOLATIONS];
short *short_curipos[SHORT_MAXINTERPOLATIONS];

void short_setinterpolation(short *posptr)
    {
    long i;

    if (short_numinterpolations >= SHORT_MAXINTERPOLATIONS)
        return;
        
    for (i = short_numinterpolations - 1; i >= 0; i--)
        {
        if (short_curipos[i] == posptr)
            return;
        }    
            
    short_curipos[short_numinterpolations] = posptr;
    short_oldipos[short_numinterpolations] = *posptr;
    short_numinterpolations++;
    }

void short_stopinterpolation(short *posptr)
    {
    long i;

    for (i = short_numinterpolations - 1; i >= short_startofdynamicinterpolations; i--)
        {
        if (short_curipos[i] == posptr)
            {
            short_numinterpolations--;
            short_oldipos[i] = short_oldipos[short_numinterpolations];
            short_bakipos[i] = short_bakipos[short_numinterpolations];
            short_curipos[i] = short_curipos[short_numinterpolations];
            }
        }    
    }

void short_updateinterpolations(void)                  // Stick at beginning of domovethings
    {
    long i;

    for (i = short_numinterpolations - 1; i >= 0; i--)
        short_oldipos[i] = *short_curipos[i];
    }

// must call restore for every do interpolations
// make sure you don't exit
void short_dointerpolations(long smoothratio)                      // Stick at beginning of drawscreen
    {
    long i, j, odelta, ndelta;

    ndelta = 0;
    j = 0;
    
    for (i = short_numinterpolations - 1; i >= 0; i--)
        {
        short_bakipos[i] = *short_curipos[i];
        
        odelta = ndelta;
        ndelta = (*short_curipos[i]) - short_oldipos[i];
        
        if (odelta != ndelta)
            j = mulscale16(ndelta, smoothratio);
            
        *short_curipos[i] = short_oldipos[i] + j;
        }
    }

void short_restoreinterpolations(void)                 // Stick at end of drawscreen
    {
    long i;

    for (i = short_numinterpolations - 1; i >= 0; i--)
        *short_curipos[i] = short_bakipos[i];
    }
