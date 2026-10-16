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

#ifndef _z_zone_private
#define _z_zone_private

#define MINFRAGMENT     64
#define DPMI_INT        0x31
#define MAXMEMORYSIZE   9000000
#define NUMZONES        8

// memory storage data types

#define MEMORYPRETAG   ( 0x1a2b3c4d )
#define MEMORYPOSTTAG  ( 0x9f8e7d6b )

typedef struct memblock_s
   {
   #if ( MEMORYCORRUPTIONTEST == 1 )
   int32     pretag;
   #endif

   int16     zone;   // which zone this block of memory belongs to
   int16     tag;    // purgelevel
   int32     size;   // including the header and possibly tiny fragments
   void    **user;   // NULL if a free block

   struct memblock_s *next;
   struct memblock_s *prev;

   } memblock_t;

typedef struct
   {
   int32       size;      // total bytes malloced, including header
   memblock_t  blocklist; // start / end cap for linked list
   memblock_t *rover;     // points to a free block of memory
   } memzone_t;

static memzone_t *Z_GetZonePtr( int32 zonenum );
static void       Z_SetZonePtr( int32 zonenum, memzone_t *zoneptr );
static void       Z_ClearZone( int32 zonenum, memzone_t *zone );
static memzone_t *Z_AllocateZone( int32 zonenum, int32 size );
static int32      Z_GetFreeZone( void );

#endif
