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

enum InventoryNames
    {
    INVENTORY_MEDKIT, 
    INVENTORY_REPAIR_KIT,
    INVENTORY_CLOAK,        // de-cloak when firing
    INVENTORY_NIGHT_VISION, 
    INVENTORY_CHEMBOMB,
    INVENTORY_FLASHBOMB, 
    INVENTORY_CALTROPS,  
    MAX_INVENTORY
    };    

typedef struct
    {
    char *Name;
    VOID (*Init)(PLAYERp);
    VOID (*Stop)(PLAYERp, short);
    PANEL_STATEp State;
    short DecPerSec;
    short MaxInv;
    long  Scale;
    short Flags;
    } INVENTORY_DATA, *INVENTORY_DATAp;

extern INVENTORY_DATA InventoryData[MAX_INVENTORY+1];

#define INVF_AUTO_USE (BIT(0))
#define INVF_TIMED (BIT(1))
#define INVF_COUNT (BIT(2))

