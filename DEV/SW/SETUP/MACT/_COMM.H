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

#ifndef _comm_private
#define _comm_private

#define SYNCCHECKSUM (0x12345678)

#define SYNC_HEREIAM   (1)
#define SYNC_STARTGAME (2)

typedef struct
{
   int32 checksum;
   int32 command;
} syncpackettype;

#endif
