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

// FILE: readini.h
// PURPOSE: routines for reading an INI file.  Basicly std C but with
//			c++ comments and suc.

#ifndef BOOL
#define BOOL int
#endif

BOOL FindSection(FILE *fh,char *sectionName);

unsigned int GetOption(FILE *fh, char *key,char *optStr, long strSz);

