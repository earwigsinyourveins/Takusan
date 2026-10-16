/*
Copyright (C) 2019 Apogee Software, Ltd.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.

*/

//****************************************************************************
//
// Private header for CONTROL.C
//
//****************************************************************************

#ifndef _control_private
#define _control_private

//****************************************************************************
//
// DEFINES
//
//****************************************************************************

#define BUTTON0_SCAN_1   sc_Space
#define BUTTON0_SCAN_2   sc_Enter
#define BUTTON1_SCAN     sc_Escape

#define SENSITIVE        60
#define CENTER           ( SENSITIVE * 2 )

#define ResetMouse       0
#define GetMouseButtons  3
#define GetMouseDelta    11

#define MouseInt         0x33
#define JoyScaleMax      16384
#define MaxJoyValue      5000
#define MINIMUMMOUSESENSITIVITY 0x1000
#define DEFAULTMOUSESENSITIVITY 0x7000+MINIMUMMOUSESENSITIVITY

#define CONTROL_NUM_FLAGS   64
#define INSTANT_ONOFF       0
#define TOGGLE_ONOFF        1

#define MAXCONTROLVALUE  0x7fff

// Maximum number of buttons for any controller
#define MAXBUTTONS 8

// Number of Mouse buttons

#define MAXMOUSEBUTTONS 3

// Number of JOY buttons

#define MAXJOYBUTTONS 4

#define BUTTONSET(x,value) \
    (\
    ((x)>31) ?\
    (CONTROL_ButtonState2 |= (value<<((x)-32)))  :\
    (CONTROL_ButtonState1 |= (value<<(x)))\
    )
#define BUTTONHELDSET(x,value)\
    (\
    ((x)>31) ?\
    (CONTROL_ButtonHeldState2 |= value<<((x)-32)) :\
    (CONTROL_ButtonHeldState1 |= value<<(x))\
    )

#define LIMITCONTROL(x)\
    {\
    if ((*x)>MAXCONTROLVALUE) \
       {\
       (*x) = MAXCONTROLVALUE;\
       }\
    if ((*x)<-MAXCONTROLVALUE) \
       {\
       (*x) = -MAXCONTROLVALUE;\
       }\
    }

//****************************************************************************
//
// TYPEDEFS
//
//****************************************************************************

typedef enum
   {
	motion_Left  = -1,
	motion_Up    = -1,
   motion_None  = 0,
   motion_Right = 1,
   motion_Down  = 1
   } motion;


typedef struct
   {
   word joyMinX;
   word joyMinY;
   word threshMinX;
   word threshMinY;
   word threshMaxX;
   word threshMaxY;
   word joyMaxX;
   word joyMaxY;
   word joyMultXL;
   word joyMultYL;
   word joyMultXH;
   word joyMultYH;
   } JoystickDef;

//typedef enum
//   {
//   currentstatus,
//   on_off
//   } controltype;

typedef struct controlflags
   {
   byte active     : 1;
   byte used       : 1;
   byte toggle     : 1;
   byte buttonheld : 1;
   byte cleared    : 1;
   } controlflags;

typedef struct
   {
   kb_scancode key1;
   kb_scancode key2;
   } controlkeymaptype;

typedef struct
   {
   int32 singleclicked;
   int32 doubleclicked;
   } controlbuttontype;


//***************************************************************************
//
// PROTOTYPES
//
//***************************************************************************

void    CONTROL_GetMouseDelta( int32 *x, int32 *y );
byte    CONTROL_GetMouseButtons( void );
boolean CONTROL_StartMouse( void );
void    CONTROL_GetJoyAbs( word joy, word *xp, word *yp );
void    CONTROL_GetJoyDelta( word joy, int32 *dx, int32 *dy );
byte    CONTROL_JoyButtons( word joy );
void    CONTROL_SetJoyScale( word joy );
void    CONTROL_SetupJoy
   (
   word joy, word minx,
   word maxx, word miny,
   word maxy, word centerx,
   word centery
   );
boolean CONTROL_StartJoy( word joy );
void    CONTROL_ShutJoy( word joy );
void    CONTROL_SetFlag( int32 which, boolean active );
boolean CONTROL_ButtonFunctionPressed( int32 whichfunction );
boolean CONTROL_KeyboardFunctionPressed( int32 whichfunction );
void    CONTROL_CheckRange( int32 which );

#endif
