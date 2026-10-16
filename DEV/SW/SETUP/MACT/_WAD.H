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

#ifndef _wad_private
#define _wad_private

#define CHECKPERIOD 20
//#define WAD_LUMP_NAME_SIZE 16
#define WAD_LUMP_NAME_SIZE 8

#define ID_STRING   "IWAD"
//"DWAD"
//===============
//   TYPES
//===============


typedef struct
   {
   char   name[ WAD_LUMP_NAME_SIZE ];
   int    handle;
   dword  position;
   dword  size;
   } lumpinfo_t;

typedef struct
   {
   char   identification[ 4 ]; // should be DWAD
   dword  numlumps;
   dword  infotableofs;
   } wadinfo_t;

typedef struct
   {
   dword  filepos;
   dword  size;
   char   name[ WAD_LUMP_NAME_SIZE ];
   } filelump_t;

#endif
