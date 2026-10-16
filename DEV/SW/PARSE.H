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

// scriplib.h

#define MAXTOKEN    255

extern  char    token[MAXTOKEN];
extern  char    *scriptbuffer,*script_p,*scriptend_p;
extern  int     grabbed;
extern  int     scriptline;
extern  BOOL    endofscript;


BOOL LoadScriptFile (char *filename);
void GetToken (BOOL crossline);
void UnGetToken (void);
BOOL TokenAvailable (void);

void DefaultExtension (char *path, char *extension);
void DefaultPath (char *path, char *basepath);
void StripFilename (char *path);
void ExtractFileBase (char *path, char *dest);

long ParseNum (char *str);

