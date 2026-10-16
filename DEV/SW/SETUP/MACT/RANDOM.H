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

#ifndef _random_public
#define _random_public
#ifdef __cplusplus
extern "C" {
#endif

void  InitializeRandomNumberGenerator ( void );
int32 GetRandomNumberSeed ( void );


#if RANDOMNUMBERTEST
byte    GameRandom ( char * string );
#define GameRandomNumber( string )   GameRandom(string)
#else
byte    GameRandom ( void );
#define GameRandomNumber(string)   GameRandom()
#endif

byte    Random ( void );
#define RandomNumber   Random

void    SetRandomNumberIndex ( int32 i );
int32   GetRandomNumberIndex ( void );

#ifdef __cplusplus
};
#endif
#endif
