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

#define SAVE_EXTERN
#include "_save.h"
#undef SAVE_EXTERN

STATEpp ActorStateTable[] = 
    {
    #define SAVE_TABLE
    #include "_save.h"
    #undef SAVE_TABLE
    NULL
    };

STATEpp ActorNoRotStateTable[] = 
    { 
    #define SAVE_TABLE2
    #include "_save.h"
    #undef SAVE_TABLE2
    NULL
    };

ANIM_CALLBACKp AnimFuncTable[] = 
{
CallbackSOsink,
NULL
};

ANIMATORp SpriteFuncTable[] = 
    {
    NullAnimator,
    DoGrating,
    NinjaJumpActionFunc,
    DoGenerateSewerDebris,
    DoLavaErupt,
    SpearOnFloor,
    SpearOnCeiling,
    DoShrapWallFloor,
    NULL
    };
    
VOID SpecialUziRetractFunc(PANEL_SPRITEp psp);

PANEL_SPRITE_FUNCp PanelSpriteFuncTable[] = 
    {
    StringTimer,
    SwordBlur,
    pSuicide,
    SpecialUziRetractFunc,
    NULL
    };
    
VOID DoPlayerTeleportPause(PLAYERp);
VOID DoPlayerDeathDrown(PLAYERp);
VOID DoPlayerOperateTank(PLAYERp);
VOID DoPlayerOperateTurret(PLAYERp);

PLAYER_ACTION_FUNCp PlayerFuncTable[] =  
    {
    DoPlayerTeleportPause,
    DoPlayerJump,
    DoPlayerForceJump,
    DoPlayerFall,
    DoPlayerClimb,
    DoPlayerSwim,
    DoPlayerCrawl,
    DoPlayerFly,
    DoPlayerStand,
    DoPlayerDive,
    DoPlayerWadeStand,
    DoPlayerWade,
    DoPlayerOperateBoat,
    DoPlayerRun,
    DoPlayerBeginStand,
    DoPlayerDeathFlip,
    DoPlayerDeathCrumble,
    DoPlayerDeathExplode,
    DoPlayerDeathFlip,
    DoPlayerDeathExplode,
    DoPlayerDeathDrown,
    DoPlayerOperateTank,
    DoPlayerOperateTurret,
    NULL
    };
