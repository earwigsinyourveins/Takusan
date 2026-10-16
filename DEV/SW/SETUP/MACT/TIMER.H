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

//////////////////////////////////////////////////////////////////////////////
//
// timer.h  -- Public header for time.c
//
// Simple timer functions.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __timer_h
#define __timer_h
#ifdef __cplusplus
extern "C" {
#endif

#define NO_MORE_TIMERS -1

int  TIME_AddTimer( int TicksPerSecond, volatile int32 *Counter );
void TIME_RemoveTimer( int which );

#ifdef __cplusplus
};
#endif
#endif /* __timer_h */
