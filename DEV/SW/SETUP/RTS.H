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
//    RTS.H
//
//***************************************************************************

#ifndef __rts_public__
#define __rts_public__

/*
====================
=
= RTS_Init
=
= Files with a .rts extension are idlink files with multiple lumps
=
====================
*/

void RTS_Init (char *filename);

/*
====================
=
= RTS_Shutdown
=
====================
*/

void RTS_Shutdown ( void );

/*
====================
=
= RTS_NumSounds
=
====================
*/

int32 RTS_NumSounds (void);
/*
====================
=
= RTS_SoundLength
=
= Returns the buffer size needed to load the given lump
=
====================
*/

int32 RTS_SoundLength (int32 lump);
/*
====================
=
= RTS_GetSoundName
=
====================
*/

char * RTS_GetSoundName (int32 i);
/*
====================
=
= RTS_GetSound
=
====================
*/
void *RTS_GetSound (int32 lump);
#endif
