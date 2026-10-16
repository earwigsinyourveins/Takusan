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

#ifndef _rt_com_public
#define _rt_com_public

//***************************************************************************
//
// RT_COM.C
//
//***************************************************************************

typedef struct
{
   int32 consoleplayer;
   int32 numplayers;
   int32 sender;
   int32 packetlength;
   boolean badpacket;
   int8 * packet;
} comm_type;

extern comm_type comm;

void InitGAMECOM   ( void );
boolean ReadPacket ( void );
void WritePacket   ( void * buffer, int32 length, int32 destination );
void SyncPlayers   ( void );

#endif
