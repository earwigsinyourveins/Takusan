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

#ifndef __dyn_public__
#define __dyn_public__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
   {
   int32 maxnumber;
   int32 numberperblock;
   int32 arrayblockshift;
   int32 datasize;
   int32 datashift;
   int32 numberblocks;
   void ** arrays;
   } dyn_t;

dyn_t * DYN_Create
   (
   int32 maxnumber,
   int32 numberperblock,
   int32 datasize
   );

void DYN_Destroy
   (
   dyn_t * dyn
   );

void * DYN_Entry
   (
   dyn_t * dyn,
   int32 itemnumber
   );

#ifdef __cplusplus
};
#endif
#endif
