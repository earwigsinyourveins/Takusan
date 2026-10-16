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

#ifndef _wadtype_public
#define _wadtype_public
#ifdef __cplusplus
extern "C" {
#endif

#define WAD_LUMP_NAME_SIZE 8
#define WADEXTENSION "wad"

#define ID_STRING   "IWAD"
//===============
//   TYPES
//===============

typedef struct
   {
   char    name[ WAD_LUMP_NAME_SIZE ];
   int16   handle;
   int16   lumptype;
   uint32  position;
   uint32  size;
   } lumpinfo_t;

typedef struct
   {
   char    identification[ 4 ]; // should be ID_STRING
   uint32  numlumps;
   uint32  infotableofs;
   } wadinfo_t;

typedef struct
   {
   uint32  filepos;
   uint32  size;
   int32   lumptype;
   char    name[ WAD_LUMP_NAME_SIZE ];
   } filelump_t;

//
// ID WAD2 Format
//

#define WAD2_LUMP_NAME_SIZE 16
#define WAD2EXTENSION "wad"

#define ID2_STRING   "WAD2"
//===============
//   TYPES
//===============

typedef struct
   {
   char    name[ WAD2_LUMP_NAME_SIZE ];
   int16   handle;
   int16   lumptype;
   uint32  position;
   uint32  size;
   } lumpinfo2_t;

typedef struct
   {
   char    identification[ 4 ]; // should be ID_STRING
   uint32  numlumps;
   uint32  infotableofs;
   } wadinfo2_t;

typedef struct
   {
   uint32  filepos;
   uint32  dsize;
   uint32  size;
   byte    lumptype;
   byte    compress;
   short   dummy;
   char    name[ WAD2_LUMP_NAME_SIZE ];
   } filelump2_t;

#ifdef __cplusplus
};
#endif
#endif
