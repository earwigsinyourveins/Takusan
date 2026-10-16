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

#ifndef SPRITE_H

#define SPRITE_H

VOID KillSprite(SHORT SpriteNum);
SHORT SpawnSprite(short stat, short id, STATEp state, short sectnum, int x, int y, int z, int ang, int vel);
VOID SpriteSetup(VOID);
int move_actor(short SpriteNum, long xchange, long ychange, long zchange);
short GetSpriteDir(short sn);
short GetDirToPlayer(short sn);
short PlayerInVision(short sn, short view_deg);
short PlayerVisible(short sn);
short SpriteLookSector(short SpriteNum, short range);
short SpriteCanGoForward(short SpriteNum, short range);
void  SpriteFindNewDirection(short SpriteNum, short range);
int DoWalk(short SpriteNum);
int DoBody(short SpriteNum);
BOOL CanMoveHere(SHORT spritenum);
BOOL SpriteOverlap(SHORT spritenum_a, SHORT spritenum_b);
int DoActorDie(short SpriteNum, short weapon);
int DoGet(short SpriteNum);
VOID SpriteControl(VOID);

#endif