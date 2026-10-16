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

#ifndef __timelib_public__
#define __timelib_public__
#ifdef __cplusplus
extern "C" {
#endif

/*
================
TIMELIB_CreateTimer
Create a new timer
================
*/
int32 TIMELIB_CreateTimer (void);

/*
================
TIMELIB_DestroyTimer
Destroy a timer
================
*/
void TIMELIB_DestroyTimer ( int32 handle );

/*
================
TIMELIB_StartTimer
Start a timer
================
*/
void TIMELIB_StartTimer ( int32 handle );

/*
================
TIMELIB_StopTimer
Stop a timer
================
*/
void TIMELIB_StopTimer ( int32 handle );

/*
================
TIMELIB_GetTimer
get the current time of a timer
================
*/
float32 TIMELIB_GetTimer ( int32 handle );

/*
================
TIMELIB_StartTiming
simple stop watch start
================
*/
void TIMELIB_StartTiming ( void );

/*
================
TIMELIB_StopTiming
simple stop watch stop
================
*/
float32 TIMELIB_StopTiming ( void );

#ifdef __cplusplus
};
#endif
#endif
