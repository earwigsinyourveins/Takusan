#include  "pragmas.h"
#include "interp.h"

#define MAXINTERPOLATIONS 1024
long numinterpolations = 0, startofdynamicinterpolations = 0;
long oldipos[MAXINTERPOLATIONS];
long bakipos[MAXINTERPOLATIONS];
long *curipos[MAXINTERPOLATIONS];

void setinterpolation(long *posptr)
    {
    long i;

    if (numinterpolations >= MAXINTERPOLATIONS)
        return;
        
    for (i = numinterpolations - 1; i >= 0; i--)
        {
        if (curipos[i] == posptr)
            return;
        }    
            
    curipos[numinterpolations] = posptr;
    oldipos[numinterpolations] = *posptr;
    numinterpolations++;
    }

void stopinterpolation(long *posptr)
    {
    long i;

    for (i = numinterpolations - 1; i >= startofdynamicinterpolations; i--)
        {
        if (curipos[i] == posptr)
            {
            numinterpolations--;
            oldipos[i] = oldipos[numinterpolations];
            bakipos[i] = bakipos[numinterpolations];
            curipos[i] = curipos[numinterpolations];
            }
        }    
    }

void updateinterpolations(void)                  // Stick at beginning of domovethings
    {
    long i;

    for (i = numinterpolations - 1; i >= 0; i--)
        oldipos[i] = *curipos[i];
    }

// must call restore for every do interpolations
// make sure you don't exit
void dointerpolations(long smoothratio)                      // Stick at beginning of drawscreen
    {
    long i, j, odelta, ndelta;

    ndelta = 0;
    j = 0;
    
    for (i = numinterpolations - 1; i >= 0; i--)
        {
        bakipos[i] = *curipos[i];
        
        odelta = ndelta;
        ndelta = (*curipos[i]) - oldipos[i];
        
        if (odelta != ndelta)
            j = mulscale16(ndelta, smoothratio);
            
        *curipos[i] = oldipos[i] + j;
        }
    }

void restoreinterpolations(void)                 // Stick at end of drawscreen
    {
    long i;

    for (i = numinterpolations - 1; i >= 0; i--)
        *curipos[i] = bakipos[i];
    }
