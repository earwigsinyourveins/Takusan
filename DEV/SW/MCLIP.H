//-------------------------------------------------------------------------
/*
Copyright (C) 2019 GHI Media, LLC

This file is part of Shadow Warrior (1997).

Shadow Warrior (1997) is free software; you can redistribute it and/or
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
//-------------------------------------------------------------------------

#define RECT_CLIP 1

long MultiClipMove(PLAYERp pp, long z, long floor_dist);
short MultiClipTurn(PLAYERp pp, short new_ang, long z, long floor_dist);
long testquadinsect(long *point_num, long *qx, long *qy, short sectnum);
long RectClipMove(PLAYERp pp, long *qx, long *qy);
long testpointinquad(long x, long y, long *qx, long *qy);
//short RectClipTurn(PLAYERp pp, short new_ang, long z, long floor_dist, long *qx, long *qy);
short RectClipTurn(PLAYERp pp, short new_ang, long *qx, long *qy, long *ox, long *oy);
