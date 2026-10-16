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

#ifndef _develop_public
#define _develop_public
#ifdef __cplusplus
extern "C" {
#endif

#define DEVELOPMENT          0
//#define SHAREWARE            0
#define LOCATIONINFO         1
#define SOFTWAREERROR        1
#define MEMORYCORRUPTIONTEST 1
#define PRECACHETEST         0
#define DATACORRUPTIONTEST   0
#define RANDOMNUMBERTEST     0


#if ( LOCATIONINFO == 1 )

#define funcstart() \
   { \
   SoftError( "funcstart : module '%s' at line %d.\n", __FILE__, __LINE__ );\
   }

#define funcend() \
   { \
   SoftError( "  funcend : module '%s' at line %d.\n", __FILE__, __LINE__ );\
   }

#else

#define funcstart()
#define funcend()

#endif

#ifdef __cplusplus
};
#endif
#endif
