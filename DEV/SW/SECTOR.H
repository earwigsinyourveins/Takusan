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

#ifndef SECTOR_H

#define SECTOR_H


VOID SectorSetup(VOID);
DOOR_AUTO_CLOSEp SetDoorAutoClose(short SectorNum, short Type);
VOID DoDragging(VOID);
int MoveDoorVert(short door_sector, short dir, short door_speed);
int MoveDoorUp(short door_sector, short auto_close, short door_speed);
int MoveDoorDown(short door_sector, short dir, short door_speed);
int MoveDoorHoriz(short door_sector, short dir, short door_speed);
VOID DoDoorsClose(VOID);
short Switch(short SwitchSector);
VOID PlayerOperateEnv(PLAYERp pp);
int TeleportToSector(PLAYERp pp, long newsector);
    
enum SO_SCALE_TYPE 
    {
    SO_SCALE_NONE,
    SO_SCALE_HOLD,
    SO_SCALE_DEST,
    SO_SCALE_RANDOM,
    SO_SCALE_CYCLE,
    SO_SCALE_RANDOM_POINT
    };
    
#define SCALE_POINT_SPEED (4 + RANDOM_RANGE(8))

typedef struct
{
long dist;
short sectnum, wallnum, spritenum;
}NEAR_TAG_INFO, *NEAR_TAG_INFOp;    
extern short nti_cnt;

    
#endif    
