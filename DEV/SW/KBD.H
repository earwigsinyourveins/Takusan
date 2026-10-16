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

#ifndef KBD_H
#define KBD_H

#define KEYFIFOSIZ 64
extern void (__interrupt __far *oldkeyhandler)();
extern void __interrupt __far keyhandler(void);
extern volatile char keystatus[256], keyfifo[KEYFIFOSIZ], keyfifoplc, keyfifoend;
extern volatile char readch, oldreadch, extended, keytemp;

#endif
