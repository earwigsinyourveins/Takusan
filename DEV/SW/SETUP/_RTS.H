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

#ifndef __rts_private__
#define __rts_private__

//===============
//   TYPES
//===============

typedef struct
   {
   char name[8];
   int32 handle,position,size;
   } lumpinfo_t;

typedef struct
   {
   char identification[4];              // should be IWAD
   int32 numlumps;
   int32 infotableofs;
   } wadinfo_t;

typedef struct
   {
   int32 filepos;
   int32 size;
   char name[8];
   } filelump_t;

#endif
