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

void DoQuakeMatch(short match);
void ProcessQuakeOn(void);
void ProcessQuakeSpot(void);
void QuakeViewChange(PLAYERp pp, long *z_diff, long *x_diff, long *y_diff, short *ang_diff);
void DoQuake(PLAYERp pp);
BOOL SetQuake(PLAYERp pp, short tics, short amt); 
int SetExpQuake(SHORT Weapon);
int SetGunQuake(SHORT SpriteNum);
int SetPlayerQuake(PLAYERp mpp);
int SetNuclearQuake(SHORT Weapon);
int SetSumoQuake(SHORT SpriteNum);
int SetSumoFartQuake(SHORT SpriteNum);
    
