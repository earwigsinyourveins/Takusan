// This file has been modified from Ken Silverman's original release
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"

//#include "settings.h"
#include "types.h"
#include "develop.h"
#include "scriplib.h"
#include "fx_man.h"
#undef MAXPLAYERS
#include "gamedefs.h"
#include "common.h"
#include "config.h"
#include "function.h"

short GamePlays = 0;

/*
===================
=
= SetGameDefaults
=
===================
*/

void SetGameDefaults( void )
   {
   }

extern BOOL DrawScreen;

void EncodePassword(char *pw)
    {
    long bak_DrawScreen = DrawScreen;
    long bak_randomseed = randomseed;
    long i;
    long len;

    DrawScreen = FALSE;
    randomseed = 1234L;
    
    strupr(pw);

    strupr(pw);
    
    len = strlen(pw);
    for (i = 0; i < len; i++)
        pw[i] += RANDOM_RANGE(26);
    
    randomseed = bak_randomseed;
    DrawScreen = bak_DrawScreen;
    }
   
void DecodePassword(char *pw)
    {
    long bak_DrawScreen = DrawScreen;
    long bak_randomseed = randomseed;
    long i;
    long len;

    DrawScreen = FALSE;
    randomseed = 1234L;
    
    strupr(pw);

    len = strlen(pw);
    for (i = 0; i < len; i++)
        pw[i] -= RANDOM_RANGE(26);
    
    randomseed = bak_randomseed;
    DrawScreen = bak_DrawScreen;
    }

/*
===================
=
= ReadGameSetup
=
===================
*/

/* Set by GAME.C command-line parsing; -1 means no command-line override. */
extern int AutoAimCommandLineOverride;
static long AutoAimConfigValue = -1;

void ReadGameSetup( int32 scripthandle )
    {
    long dummy;
    long ret;
    extern char WangBangMacro[10][64];

    for(dummy = 0;dummy < 10;dummy++)
        {
        sprintf(ds,"CommbatMacro#%d",dummy);
        SCRIPT_GetString( scripthandle, "Comm Setup",ds,WangBangMacro[dummy]);
        }

    SCRIPT_GetString( scripthandle, "Options","Rooster",gs.Password);
    DecodePassword(gs.Password);    
        
    // option stuff
    dummy = -1;
    ret = SCRIPT_GetNumber( scripthandle, "Options", "BorderNum",&dummy);
    if (dummy != -1) gs.BorderNum = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Brightness",&dummy);
    if (dummy != -1) gs.Brightness = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "BorderTile",&dummy);
    if (dummy != -1) gs.BorderTile = dummy;
        
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Bobbing",&dummy);
    if (dummy != -1) gs.Bobbing = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Tilting",&dummy);
    if (dummy != -1) gs.Tilting = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Shadows",&dummy);
    if (dummy != -1) gs.Shadows = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "AutoRun",&dummy);
    if (dummy != -1) gs.AutoRun = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Crosshair",&dummy);
    if (dummy != -1) gs.Crosshair = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "AutoAim",&dummy);
    if (dummy != -1) gs.AutoAim = dummy;
    /* Remember the saved setting, then apply a launch-only override. */
    AutoAimConfigValue = gs.AutoAim;
    if (AutoAimCommandLineOverride == 0 || AutoAimCommandLineOverride == 1)
        gs.AutoAim = AutoAimCommandLineOverride;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Messages",&dummy);
    if (dummy != -1) gs.Messages = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Talking",&dummy);
    if (dummy != -1) gs.Talking = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Ambient",&dummy);
    if (dummy != -1) gs.Ambient = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "FxOn",&dummy);
    if (dummy != -1) gs.FxOn = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "MusicOn",&dummy);
    if (dummy != -1) gs.MusicOn = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Controls", "MouseAiming",&dummy);
    if (dummy != -1) gs.MouseAimingType = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetGameType",&dummy);
    if (dummy != -1) gs.NetGameType = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetLevel",&dummy);
    if (dummy != -1) gs.NetLevel = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetMonsters",&dummy);
    if (dummy != -1) gs.NetMonsters = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetHurtTeammate",&dummy);
    if (dummy != -1) gs.NetHurtTeammate = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetSpawnMarkers",&dummy);
    if (dummy != -1) gs.NetSpawnMarkers = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetTeamPlay",&dummy);
    if (dummy != -1) gs.NetTeamPlay = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetKillLimit",&dummy);
    if (dummy != -1) gs.NetKillLimit = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetTimeLimit",&dummy);
    if (dummy != -1) gs.NetTimeLimit = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetColor",&dummy);
    if (dummy != -1) gs.NetColor = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Voxels",&dummy);
    if (dummy != -1) gs.Voxels = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Stats",&dummy);
    if (dummy != -1) gs.Stats = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "MouseAimingOn",&dummy);
    if (dummy != -1) gs.MouseAimingOn = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "MouseInvert",&dummy);
    if (dummy != -1) gs.MouseInvert = dummy;
    
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Kiwi",&dummy);
    if (dummy != -1) gs.ParentalLock = dummy;
    
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "PlayCD",&dummy);
    if (dummy != -1) gs.PlayCD = dummy;
   
#ifdef SW_SHAREWARE   
   dummy = -1;
   SCRIPT_GetNumber( scripthandle, "Options", "Chickens",&dummy);
   if (dummy != -1) GamePlays = dummy;

   {
        time_t start_time, new_time;

        printf
            (
            "\n"            
            "You have played Shadow Warrior %d times.  Please call and order the full\n"
            "version at 1(800)-3DREALMS or see the ORDER.FRM file.\n\n"
            ,GamePlays);
            
        start_time = time(NULL);
        
        while (TRUE)
            {
            new_time = time(NULL);
            
            if (difftime(new_time, start_time) > 1.0)
                break;
            }
   
    GamePlays++;
    }
#endif    
    }

/*
===================
=
= WriteGameSetup
=
===================
*/

void WriteGameSetup( int32 scripthandle)
   {
   long dummy;
   
   
   dummy = gs.BorderNum;
   SCRIPT_PutNumber( scripthandle, "Options", "BorderNum",dummy,false,false);
   dummy = gs.Brightness;
   SCRIPT_PutNumber( scripthandle, "Options", "Brightness",dummy,false,false);
   dummy = gs.BorderTile;
   SCRIPT_PutNumber( scripthandle, "Options", "BorderTile",dummy,false,false);
   dummy = gs.Bobbing;
   SCRIPT_PutNumber( scripthandle, "Options", "Bobbing",dummy,false,false);
   dummy = gs.Tilting;
   SCRIPT_PutNumber( scripthandle, "Options", "Tilting",dummy,false,false);
   dummy = gs.Shadows;
   SCRIPT_PutNumber( scripthandle, "Options", "Shadows",dummy,false,false);
   dummy = gs.AutoRun;
   SCRIPT_PutNumber( scripthandle, "Options", "AutoRun",dummy,false,false);
   dummy = gs.Crosshair;
   SCRIPT_PutNumber( scripthandle, "Options", "Crosshair",dummy,false,false);
   /* Keep -aim0/-aim1 temporary; do not overwrite AutoAim in SW.CFG. */
   if (AutoAimCommandLineOverride == 0 || AutoAimCommandLineOverride == 1)
      dummy = AutoAimConfigValue;
   else
      dummy = gs.AutoAim;
   SCRIPT_PutNumber( scripthandle, "Options", "AutoAim",dummy,false,false);
   dummy = gs.Messages;
   SCRIPT_PutNumber( scripthandle, "Options", "Messages",dummy,false,false);
   dummy = gs.Talking;
   SCRIPT_PutNumber( scripthandle, "Options", "Talking",dummy,false,false);
   dummy = gs.Ambient;
   SCRIPT_PutNumber( scripthandle, "Options", "Ambient",dummy,false,false);
   dummy = gs.FxOn;
   SCRIPT_PutNumber( scripthandle, "Options", "FxOn",dummy,false,false);
   dummy = gs.MouseAimingType;
   SCRIPT_PutNumber( scripthandle, "Controls", "MouseAiming",dummy,false,false);
   
   dummy = gs.MusicOn;
   SCRIPT_PutNumber( scripthandle, "Options", "MusicOn",dummy,false,false);
   
   dummy = gs.NetGameType;
   SCRIPT_PutNumber( scripthandle, "Options", "NetGameType",dummy,false,false);
   dummy = gs.NetLevel;
   SCRIPT_PutNumber( scripthandle, "Options", "NetLevel",dummy,false,false);
   dummy = gs.NetMonsters;
   SCRIPT_PutNumber( scripthandle, "Options", "NetMonsters",dummy,false,false);
   dummy = gs.NetHurtTeammate;
   SCRIPT_PutNumber( scripthandle, "Options", "NetHurtTeammate",dummy,false,false);
   dummy = gs.NetSpawnMarkers;
   SCRIPT_PutNumber( scripthandle, "Options", "NetSpawnMarkers",dummy,false,false);
   dummy = gs.NetTeamPlay;
   SCRIPT_PutNumber( scripthandle, "Options", "NetTeamPlay",dummy,false,false);
   dummy = gs.NetKillLimit;
   SCRIPT_PutNumber( scripthandle, "Options", "NetKillLimit",dummy,false,false);
   dummy = gs.NetTimeLimit;
   SCRIPT_PutNumber( scripthandle, "Options", "NetTimeLimit",dummy,false,false);
   dummy = gs.NetColor;
   SCRIPT_PutNumber( scripthandle, "Options", "NetColor",dummy,false,false);
   dummy = gs.Voxels;
   SCRIPT_PutNumber( scripthandle, "Options", "Voxels",dummy,false,false);
   dummy = gs.MouseAimingOn;
   SCRIPT_PutNumber( scripthandle, "Options", "MouseAimingOn",dummy,false,false);
   dummy = gs.MouseAimingOn;
   SCRIPT_PutNumber( scripthandle, "Options", "MouseAimingOn",dummy,false,false);
   dummy = gs.MouseInvert;
   SCRIPT_PutNumber( scripthandle, "Options", "MouseInvert",dummy,false,false);
   dummy = gs.Stats;
   SCRIPT_PutNumber( scripthandle, "Options", "Stats",dummy,false,false);
   
   EncodePassword(gs.Password);    
   SCRIPT_PutString( scripthandle, "Options","Rooster",gs.Password);
   DecodePassword(gs.Password);    
   
   dummy = gs.ParentalLock;
   SCRIPT_PutNumber( scripthandle, "Options", "Kiwi",dummy,false,false);

   dummy = gs.PlayCD;
   SCRIPT_PutNumber( scripthandle, "Options", "PlayCD",dummy,false,false);
   
#ifdef SW_SHAREWARE   
   dummy = GamePlays;
   SCRIPT_PutNumber( scripthandle, "Options", "Chickens",dummy,false,false);
#endif   
   }

void TermSetup(void)
{
extern BOOL BotMode;
CONFIG_WriteSetup();
RTS_Shutdown();
}
