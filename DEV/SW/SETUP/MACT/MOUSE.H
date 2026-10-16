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

#ifndef __mouse_h
#define __mouse_h
#ifdef __cplusplus
extern "C" {
#endif

#define LEFT_MOUSE   1
#define RIGHT_MOUSE  2
#define MIDDLE_MOUSE 4
#define LEFT_MOUSE_PRESSED( button ) ( ( ( button ) & LEFT_MOUSE ) != 0 )
#define RIGHT_MOUSE_PRESSED( button ) ( ( ( button ) & RIGHT_MOUSE ) != 0 )
#define MIDDLE_MOUSE_PRESSED( button ) ( ( ( button ) & MIDDLE_MOUSE ) != 0 )

boolean MOUSE_Init( void );
void    MOUSE_Shutdown( void );
void    MOUSE_ShowCursor( void );
void    MOUSE_HideCursor( void );
int32   MOUSE_GetButtons( void );
void    MOUSE_GetPosition( int32*x, int32*y  );
void    MOUSE_GetDelta( int32*x, int32*y  );

#ifdef __cplusplus
};
#endif
#endif /* __mouse_h */
