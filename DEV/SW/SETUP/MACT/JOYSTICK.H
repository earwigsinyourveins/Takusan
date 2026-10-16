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

#ifndef _joystick_public
#define _joystick_public
#ifdef __cplusplus
extern "C" {
#endif

//***************************************************************************
//
// Public header for JOYSTICK.ASM.
//
//***************************************************************************

extern int32 Joy_x1;
extern int32 Joy_y1;
extern int32 Joy_x2;
extern int32 Joy_y2;
extern byte  Joy_mask;

void JoyStick_Vals( void );
#if defined(__FLAT__) || defined(__NT__)
#pragma aux JoyStick_Vals modify exact [eax ebx ecx edx esi edi]
#endif

#define JOYSTICK_GetPos( x1, y1, x2, y2 ) \
   { \
   JoyStick_Vals(); \
   *( x1 ) = Joy_x1; \
   *( y1 ) = Joy_y1; \
   *( x2 ) = Joy_x2; \
   *( y2 ) = Joy_y2; \
   }


#ifdef __cplusplus
};
#endif
#endif
