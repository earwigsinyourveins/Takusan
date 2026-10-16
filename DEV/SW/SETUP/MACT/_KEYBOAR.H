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

//***************************************************************************
//
//    Keyboard Constants
//
//***************************************************************************

#ifndef _keyboard_private
#define _keyboard_private

#define KEYQMAX            256
#define KEYBOARDINT        0x09
#define NUMPAUSESCANCODES  6
#define EXTENDEDKEYSCANCODE 0xe0

/*
=============================================================================

                                    MACROS

=============================================================================
*/

#define KB_SetKeyDown( scan ) \
   { \
   KB_KeyDown[ ( scan ) ] = true; \
   }

#define IS_KEYPAD_CODE(scode)         \
           (                          \
              (                       \
                 ((scode) >= 0x47) && \
                 ((scode) <= 0x53)    \
              )  ||                   \
              (                       \
                 ((scode) >= 0x67) && \
                 ((scode) <= 0x68)    \
              )  ||                   \
              (                       \
                 ((scode) == 0x37)    \
              )                       \
           )
#endif
