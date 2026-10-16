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

BOOL WarpPlaneSectorInfo(short sectnum, SPRITEp *sp_ceiling, SPRITEp *sp_floor);
SPRITEp WarpPlane(LONGp x, LONGp y, LONGp z, SHORTp sectnum);
SPRITEp WarpToArea(SPRITEp sp_from, LONGp x, LONGp y, LONGp z, SHORTp sectnum);
BOOL WarpSectorInfo(short sectnum, SPRITEp *sp_warp);
SPRITEp Warp(LONGp x, LONGp y, LONGp z, SHORTp sectnum);
