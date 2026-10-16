#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "build.h"
#include "proto.h"
#include "names2.h"
#include "game.h"
#include "tags.h"
#include "def.h"

void CopySectorWalls(short dest_sectnum, short src_sectnum)
    {
    short dest_wall_num, src_wall_num, start_wall;
    
    dest_wall_num = sector[dest_sectnum].wallptr;
    src_wall_num = sector[src_sectnum].wallptr;

    start_wall = dest_wall_num;
    
    do
        {
        wall[dest_wall_num].picnum = wall[src_wall_num].picnum;
        
        wall[dest_wall_num].xrepeat =       wall[src_wall_num].xrepeat;
        wall[dest_wall_num].yrepeat =       wall[src_wall_num].yrepeat;
        wall[dest_wall_num].overpicnum =    wall[src_wall_num].overpicnum;
        wall[dest_wall_num].pal =           wall[src_wall_num].pal;
        wall[dest_wall_num].cstat =         wall[src_wall_num].cstat;
        wall[dest_wall_num].shade =         wall[src_wall_num].shade;
        wall[dest_wall_num].xpanning =      wall[src_wall_num].xpanning;
        wall[dest_wall_num].ypanning =      wall[src_wall_num].ypanning;
        wall[dest_wall_num].hitag =         wall[src_wall_num].hitag;
        wall[dest_wall_num].lotag =         wall[src_wall_num].lotag;
        wall[dest_wall_num].extra =         wall[src_wall_num].extra;
         
        if (wall[dest_wall_num].nextwall >= 0 && wall[src_wall_num].nextwall >= 0)
            {
            wall[wall[dest_wall_num].nextwall].picnum = wall[wall[src_wall_num].nextwall].picnum;
            wall[wall[dest_wall_num].nextwall].xrepeat = wall[wall[src_wall_num].nextwall].xrepeat;
            wall[wall[dest_wall_num].nextwall].yrepeat = wall[wall[src_wall_num].nextwall].yrepeat;
            wall[wall[dest_wall_num].nextwall].overpicnum = wall[wall[src_wall_num].nextwall].overpicnum;
            wall[wall[dest_wall_num].nextwall].pal = wall[wall[src_wall_num].nextwall].pal;
            wall[wall[dest_wall_num].nextwall].cstat = wall[wall[src_wall_num].nextwall].cstat;
            wall[wall[dest_wall_num].nextwall].shade = wall[wall[src_wall_num].nextwall].shade;
            wall[wall[dest_wall_num].nextwall].xpanning = wall[wall[src_wall_num].nextwall].xpanning;
            wall[wall[dest_wall_num].nextwall].ypanning = wall[wall[src_wall_num].nextwall].ypanning;
            wall[wall[dest_wall_num].nextwall].hitag = wall[wall[src_wall_num].nextwall].hitag;
            wall[wall[dest_wall_num].nextwall].lotag = wall[wall[src_wall_num].nextwall].lotag;
            wall[wall[dest_wall_num].nextwall].extra = wall[wall[src_wall_num].nextwall].extra;
            }

        dest_wall_num = wall[dest_wall_num].point2;
        src_wall_num = wall[src_wall_num].point2;
        }
    while(dest_wall_num != start_wall);
    }                    

void CopySectorMatch(short match)
    {
    short ed,nexted,ss,nextss;
    SPRITEp dest_sp, src_sp;
    SECTORp dsectp,ssectp;
    short kill, nextkill;
    SPRITEp k;
    
    TRAVERSE_SPRITE_STAT(headspritestat[STAT_COPY_DEST], ed, nexted)    
        {
        dest_sp = &sprite[ed];
        dsectp = &sector[dest_sp->sectnum];

        if (match != sprite[ed].lotag)
            continue;
        
        TRAVERSE_SPRITE_STAT(headspritestat[STAT_COPY_SOURCE], ss, nextss)    
            {
            src_sp = &sprite[ss];
            
            if (SP_TAG2(src_sp) == SPRITE_TAG2(ed) &&
                SP_TAG3(src_sp) == SPRITE_TAG3(ed))
                {
                short src_move, nextsrc_move;
                ssectp = &sector[src_sp->sectnum];
                
                // !!!!!AAAAAAAAAAAAAAAAAAAAAAHHHHHHHHHHHHHHHHHHHHHH
                // Don't kill anything you don't have to
                // this wall killing things on a Queue causing
                // invalid situations
                    
                #if 1
                // kill all sprites in the dest sector that need to be
                TRAVERSE_SPRITE_SECT(headspritesect[dest_sp->sectnum], kill, nextkill)    
                    {
                    k = &sprite[kill];
                    
                    // kill anything not invisible
                    if (!TEST(k->cstat, CSTAT_SPRITE_INVISIBLE))
                        {
                        if (User[kill])
                            {
                            // be safe with the killing
                            //SetSuicide(kill);
                            }
                        else
                            {    
                            SpriteQueueDelete(kill); // new function to allow killing - hopefully
                            KillSprite(kill);
                            }
                        }
                    }
                #endif    
                
                CopySectorWalls(dest_sp->sectnum, src_sp->sectnum);
                
                TRAVERSE_SPRITE_SECT(headspritesect[src_sp->sectnum], src_move, nextsrc_move)    
                    {
                    // don't move ST1 Copy Tags
                    if (SPRITE_TAG1(src_move) != SECT_COPY_SOURCE)
                        {
                        long sx,sy,dx,dy,src_xoff,src_yoff,trash;
                        
                        // move sprites from source to dest - use center offset
                        
                        // get center of src and dest sect
                        SectorMidPoint(src_sp->sectnum, &sx, &sy, &trash);
                        SectorMidPoint(dest_sp->sectnum, &dx, &dy, &trash);
                        
                        // get offset
                        src_xoff = sx - sprite[src_move].x;
                        src_yoff = sy - sprite[src_move].y;
                        
                        // move sprite to dest sector
                        sprite[src_move].x = dx - src_xoff;
                        sprite[src_move].y = dy - src_yoff;
                        
                        // change sector
                        changespritesect(src_move, dest_sp->sectnum);
                        
                        // check to see if it moved on to a sector object
                        if (TEST(sector[dest_sp->sectnum].extra, SECTFX_SECTOR_OBJECT))
                            {
                            SECTOR_OBJECTp sop;
                            extern short GlobSpeedSO;
                            
                            // find and add sprite to SO
                            sop = DetectSectorObject(&sector[sprite[src_move].sectnum]);
                            AddSpriteToSectorObject(src_move, sop);
                            
                            // update sprites postions so they aren't in the
                            // wrong place for one frame
                            GlobSpeedSO = 0;
                            RefreshPoints(sop, 0, 0, TRUE);
                            }
                        }
                    }
                
                // copy sector user if there is one
                if (SectUser[src_sp->sectnum] || SectUser[dest_sp->sectnum])
                    {
                    SECT_USERp ssectu = GetSectUser(src_sp->sectnum);    
                    SECT_USERp dsectu = GetSectUser(dest_sp->sectnum);    
                    
                    memcpy(dsectu, ssectu, sizeof(SECT_USER));
                    }

                dsectp->hitag = ssectp->hitag;
                dsectp->lotag = ssectp->lotag;
                
                dsectp->floorz = ssectp->floorz;
                dsectp->ceilingz = ssectp->ceilingz;
                
                dsectp->floorshade = ssectp->floorshade;
                dsectp->ceilingshade = ssectp->ceilingshade;
                
                dsectp->floorpicnum = ssectp->floorpicnum;
                dsectp->ceilingpicnum = ssectp->ceilingpicnum;
                
                dsectp->floorheinum = ssectp->floorheinum;
                dsectp->ceilingheinum = ssectp->ceilingheinum;
                
                dsectp->floorpal = ssectp->floorpal;
                dsectp->ceilingpal = ssectp->ceilingpal;
                
                dsectp->floorxpanning = ssectp->floorxpanning;
                dsectp->ceilingxpanning = ssectp->ceilingxpanning;
                
                dsectp->floorypanning = ssectp->floorypanning;
                dsectp->ceilingypanning = ssectp->ceilingypanning;
                
                dsectp->floorstat = ssectp->floorstat;
                dsectp->ceilingstat = ssectp->ceilingstat;

                dsectp->extra = ssectp->extra;
                dsectp->visibility = ssectp->visibility;
                }
            }    
        }    
    
    // do this outside of processing loop for safety    
    
    // kill all matching dest
    TRAVERSE_SPRITE_STAT(headspritestat[STAT_COPY_DEST], ed, nexted)    
        {
        if (match == sprite[ed].lotag)
            KillSprite(ed);    
        }

    // kill all matching sources
    TRAVERSE_SPRITE_STAT(headspritestat[STAT_COPY_SOURCE], ss, nextss)    
        {
        if (match == sprite[ss].lotag)
            KillSprite(ss);
        }
        
    }
