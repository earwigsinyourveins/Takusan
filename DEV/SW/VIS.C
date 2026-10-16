#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build.h"
#include "proto.h"
#include "names2.h"
#include "game.h"
#include "tags.h"
#include "mathutil.h"
#include "break.h"
#include "quake.h"
#include "pal.h"

#include "def.h"

extern BOOL GamePaused;

#define VIS_VisCur(sp) (SP_TAG2(sp))
#define VIS_VisDir(sp) (SP_TAG3(sp))
#define VIS_VisGoal(sp) (SP_TAG4(sp))

void ProcessVisOn(void)
    {
    short i, nexti;
    SPRITEp sp;
    
    TRAVERSE_SPRITE_STAT(headspritestat[STAT_VIS_ON], i, nexti)
        {
        sp = &sprite[i];
        
        if (VIS_VisDir(sp))
            {
            // get brighter
            VIS_VisCur(sp) >>= 1;
            //VIS_VisCur(sp) -= 16;
            if (VIS_VisCur(sp) <= VIS_VisGoal(sp))
                {
                VIS_VisCur(sp) = VIS_VisGoal(sp);
                VIS_VisDir(sp) ^= 1;
                }
            }
        else
            {
            // get darker
            VIS_VisCur(sp) <<= 1;
            VIS_VisCur(sp) += 1;
            //VIS_VisCur(sp) += 16;
            if (VIS_VisCur(sp) >= NormalVisibility)
                {
                VIS_VisCur(sp) = NormalVisibility;
                if (sp->owner >= 0)
                    {
                    ASSERT(User[sp->owner]);
                    RESET(User[sp->owner]->Flags2, SPR2_VIS_SHADING);
                    }
                KillSprite(i);
                }
            }    
        }
    }

void VisViewChange(PLAYERp pp, long *vis)
    {
    short i, nexti;
    SPRITEp sp;
    short BrightestVis = NormalVisibility;
    long x,y,z;
    short sectnum;
    
    if (GamePaused)    
        return;
    
    // find the closest quake - should be a strength value
    TRAVERSE_SPRITE_STAT(headspritestat[STAT_VIS_ON], i, nexti)
        {
        sp = &sprite[i];
        
        if (sp->owner >= 0)
            {
            x = sprite[sp->owner].x;    
            y = sprite[sp->owner].y;    
            z = sprite[sp->owner].z;    
            sectnum = sprite[sp->owner].sectnum;
            }
        else
            {    
            x = sp->x;    
            y = sp->y;    
            z = sp->z;    
            sectnum = sp->sectnum;
            }
        
        // save off the brightest vis that you can see
        if (FAFcansee(pp->posx, pp->posy, pp->posz, pp->cursectnum, x, y, z, sectnum))
            {
            if (VIS_VisCur(sp) < BrightestVis)
                BrightestVis = VIS_VisCur(sp);
            }
        }
    
    *vis = BrightestVis;    
    }
    
int SpawnVis(short Parent, short sectnum, long x, long y, long z, long amt)    
    {
    short SpriteNum;
    SPRITEp sp;
    short i,nexti;
    
    if (Parent >= 0)
        {
        if (sector[sprite[Parent].sectnum].floorpal == PALETTE_FOG)
            return(-1);

        if (sector[sprite[Parent].sectnum].floorpal == PALETTE_DIVE_LAVA)
            return(-1);
        
        // kill any others with the same parent
        TRAVERSE_SPRITE_STAT(headspritestat[STAT_VIS_ON], i, nexti)
            {
            sp = &sprite[i];
            if (sp->owner == Parent)
                {
                KillSprite(i);
                }
            }
        
        SpriteNum = COVERinsertsprite(sprite[Parent].sectnum, STAT_VIS_ON);
        sp = &sprite[SpriteNum];
        
        sp->owner = Parent;
        
        ASSERT(User[Parent]);
        SET(User[Parent]->Flags2, SPR2_CHILDREN);
        
        sp->x = sprite[Parent].x;
        sp->y = sprite[Parent].y;
        sp->z = sprite[Parent].z;
        
        SET(User[Parent]->Flags2, SPR2_VIS_SHADING);
        }
    else    
        {
        if (sector[sectnum].floorpal == PALETTE_FOG)
            return(-1);
        
        SpriteNum = COVERinsertsprite(sectnum, STAT_VIS_ON);
        sp = &sprite[SpriteNum];
        
        sp->x = x;
        sp->y = y;
        sp->z = z - Z(20);
        sp->owner = -1;
        }
        
    sp->cstat = 0;
    sp->extra = 0;
    
    VIS_VisDir(sp) = 1;
    VIS_VisCur(sp) = NormalVisibility;
    VIS_VisGoal(sp) = amt;

    return(SpriteNum);
    }
    
