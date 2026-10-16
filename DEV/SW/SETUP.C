// This file has been modified from Ken Silverman's original release
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "game.h"
#undef MAXPLAYERS

#include "types.h"
#include "develop.h"
#include "sndcards.h"
#include "fx_man.h"
#include "music.h"
#include "scriplib.h"
#include "file_lib.h"
#include "gamedefs.h"
#include "keyboard.h"
#include "util_lib.h"

#include "control.h"
#include "config.h"
#include "sounds.h"
#include "function.h"

#include "def.h"

/*
===================

=
= SetupGameButtons
=
===================
*/

void SetupGameButtons( void )
    {
    // processes array from function.h - char * gamefunctions[];
    
    short gamefunc;
    
    for (gamefunc = 0; gamefunc < NUMGAMEFUNCTIONS; gamefunc++)
        {
        CONTROL_DefineFlag(gamefunc,false);
        }
   }

void CenterCenter(void)
   {
   printf("\nCenter the joystick and press a button\n");
   }

void UpperLeft(void)
   {
   printf("Move joystick to upper-left corner and press a button\n");
   }

void LowerRight(void)
   {
   printf("Move joystick to lower-right corner and press a button\n");
   }

void CenterThrottle(void)
   {
   printf("Center the throttle control and press a button\n");
   }

void CenterRudder(void)
   {
   printf("Center the rudder control and press a button\n");
   }

/*
===================
=
= GetTime
=
===================
*/

static int32 timert;

int32 GetTime(void)
   {
   return timert++;
   }

void InitSetup(void)
   {
   //RegisterShutdownFunction( ShutDown );

   //StartWindows();
   initkeys();
   CONFIG_GetSetupFilename();
   //InitializeKeyDefList();
   CONFIG_ReadSetup();
   CONTROL_Startup( ControllerType, &GetTime, 120 );
   SetupGameButtons();
   
   if (CONTROL_JoystickEnabled)
      {
      CONTROL_CenterJoystick(CenterCenter, UpperLeft, LowerRight, CenterThrottle,
         CenterRudder);
      }
      
   RTS_Init(RTSName);
   }

#if 0   
void TermSetup(void)
   {
   //FreeKeyDefList();
   }
#endif   
   
// BELOW IS FROM A TEST SETUP BY MARK DOC
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
   
#if 0
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "develop.h"
#include "sndcards.h"
#include "fx_man.h"
#include "music.h"
#include "scriplib.h"
#include "file_lib.h"
#include "gamedefs.h"
#include "keyboard.h"
#include "util_lib.h"

#include "control.h"
#include "config.h"
#include "sounds.h"
#include "function.h"
#include "rts.h"
#include "timer.h"

int32 timerhandle=0;
volatile int32 timer;
/*
===================
=
= Shutdown
=
===================
*/

void ShutDown( void )
   {
   KB_Shutdown();
   TIME_RemoveTimer( timerhandle );
   SoundShutdown();
   MusicShutdown();
   CONFIG_WriteSetup();
   }

/*
===================

=
= SetupGameButtons
=
===================
*/

void SetupGameButtons( void )
   {
   CONTROL_DefineFlag(gamefunc_Move_Forward,      false );
   CONTROL_DefineFlag(gamefunc_Move_Backward,     false );
   CONTROL_DefineFlag(gamefunc_Turn_Left,         false );
   CONTROL_DefineFlag(gamefunc_Turn_Right,        false );
   CONTROL_DefineFlag(gamefunc_Strafe,            false );
   CONTROL_DefineFlag(gamefunc_Strafe_Left,       false );
   CONTROL_DefineFlag(gamefunc_Strafe_Right,      false );
   CONTROL_DefineFlag(gamefunc_TurnAround,        false );
   CONTROL_DefineFlag(gamefunc_Jump,              false );
   CONTROL_DefineFlag(gamefunc_Crouch,            false );
   CONTROL_DefineFlag(gamefunc_Fire,              false );
   CONTROL_DefineFlag(gamefunc_Open,              false );
   CONTROL_DefineFlag(gamefunc_Look_Up,           false );
   CONTROL_DefineFlag(gamefunc_Look_Down,         false );
   CONTROL_DefineFlag(gamefunc_Aim_Up,            false );
   CONTROL_DefineFlag(gamefunc_Aim_Down,          false );
   CONTROL_DefineFlag(gamefunc_Run,               false );
   CONTROL_DefineFlag(gamefunc_SendMessage,       false );
   CONTROL_DefineFlag(gamefunc_Weapon_1,          false );
   CONTROL_DefineFlag(gamefunc_Weapon_2,          false );
   CONTROL_DefineFlag(gamefunc_Weapon_3,          false );
   CONTROL_DefineFlag(gamefunc_Weapon_4,          false );
   CONTROL_DefineFlag(gamefunc_Weapon_5,          false );
   CONTROL_DefineFlag(gamefunc_Weapon_6,          false );
   CONTROL_DefineFlag(gamefunc_Weapon_7,          false );
   CONTROL_DefineFlag(gamefunc_Weapon_8,          false );
   CONTROL_DefineFlag(gamefunc_Weapon_9,          false );
   CONTROL_DefineFlag(gamefunc_Weapon_10,         false );
   CONTROL_DefineFlag(gamefunc_Map,               false );
   CONTROL_DefineFlag(gamefunc_Look_Left,         false );
   CONTROL_DefineFlag(gamefunc_Look_Right,        false );
   CONTROL_DefineFlag(gamefunc_Shrink_Screen,     false );
   CONTROL_DefineFlag(gamefunc_Enlarge_Screen,    false );
   CONTROL_DefineFlag(gamefunc_AutoRun,           false );
   CONTROL_DefineFlag(gamefunc_Center_View,       false );
   CONTROL_DefineFlag(gamefunc_Holster_Weapon,    false );
   CONTROL_DefineFlag(gamefunc_Inventory_Left,    false );
   CONTROL_DefineFlag(gamefunc_Inventory_Right,   false );
   CONTROL_DefineFlag(gamefunc_Inventory,         false );
   
   }

/*
===================
=
= GetTime
=
===================
*/

int32 GetTime(void)
   {
   return timer;
   }

/*
===================
=
= CenterCenter
=
===================
*/

void CenterCenter(void)
   {
   printf("Center the joystick and press a button\n");
   }

/*
===================
=
= UpperLeft
=
===================
*/

void UpperLeft(void)
   {
   printf("Move joystick to upper-left corner and press a button\n");
   }

/*
===================
=
= LowerRight
=
===================
*/

void LowerRight(void)
   {
   printf("Move joystick to lower-right corner and press a button\n");
   }

/*
===================
=
= CenterThrottle
=
===================
*/

void CenterThrottle(void)
   {
   printf("Center the throttle control and press a button\n");
   }

/*
===================
=
= CenterRudder
=
===================
*/

void CenterRudder(void)
   {
   printf("Center the rudder control and press a button\n");
   }

void main()
   {
   char * song;
   char * voc;
   volatile int32 lasttime;

   RegisterShutdownFunction( ShutDown );
   KB_Startup();
   timerhandle = TIME_AddTimer( 40, &timer );
   CONFIG_GetSetupFilename();
   CONFIG_ReadSetup();
   CONTROL_Startup( ControllerType, &GetTime, 1500 );
   SetupGameButtons();
   if (CONTROL_JoystickEnabled)
      {
      CONTROL_CenterJoystick(CenterCenter, UpperLeft, LowerRight, CenterThrottle, CenterRudder );
      }
      
   MusicStartup();
   SoundStartup();
   RTS_Init(RTSName);

   // load in some test data

   LoadFile("test.mid",&song);
   LoadFile("test.voc",&voc);

   // start playing a song

   MUSIC_PlaySong( song, MUSIC_LoopSong );


   lasttime = timer;
   while (1)
      {
      int32 i;
      ControlInfo info;

      while (lasttime==timer)
         {
         ServiceEvents();
         }
      lasttime = timer;
//      printf("timer=%ld\n",timer);
      CONTROL_GetInput( &info );

      if  (
          info.dx!=0 ||
          info.dy!=0 ||
          info.dz!=0 ||
          info.dpitch!=0 ||
          info.dyaw!=0 ||
          info.droll!=0
          )
          printf("x=%6ld y=%6ld z=%6ld yaw=%6ld pitch=%6ld roll=%6ld\n",
                 info.dx,info.dy,info.dz,info.dyaw,info.dpitch,info.droll);
      // Get Keyboard input and set appropiate game function states
      for (i=0;i<NUMGAMEFUNCTIONS;i++)
         {
         if (BUTTON(i) && !BUTTONHELD(i))
            {
            printf("%s\n",gamefunctions[i]);
            }
         }
      for (i=0;i<10;i++)
         {
         if (KB_KeyPressed(sc_F1+i))
            {
            byte * ptr;
            KB_ClearKeyDown(sc_F1+i);
            ptr = RTS_GetSound(i);
            FX_PlayVOC( ptr, 0, 255, 255, 255, 255, 0);
            }
         }
      // Check to see if fire is being pressed so we can play a sound
      if (BUTTON(gamefunc_Fire) && !BUTTONHELD(gamefunc_Fire))
         {
         FX_PlayVOC( voc, 0, 255, 255, 255, 255, 0);
         }

      // Check to see if we want to exit
      if ( KB_KeyPressed(sc_Escape) )
         {
         break;
         }

      }

   ShutDown();
   }
#endif
