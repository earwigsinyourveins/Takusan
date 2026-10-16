/*
Copyright (C) 2019 Apogee Software, Ltd.

This program is free software; you can redistribute it and/or
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

// scrplib.h

#ifndef _scriplib_private
#define _scriplib_private

#define SCRIPTSECTIONSTART ('[')
#define SCRIPTSECTIONEND   (']')
#define SCRIPTENTRYSEPARATOR ('=')
#define SCRIPTCOMMENT      (';')
#define SCRIPTEOL          ('\n')
#define SCRIPTNULL         ('\0')
#define SCRIPTSTRINGSEPARATOR ('"')
#define SCRIPTHEXFIRST ('0')
#define SCRIPTHEXSECOND ('x')
#define SCRIPTSPACE     (' ')
#define SCRIPTDEFAULTVALUE ('~')

typedef enum
   {
   linetype_comment,
   linetype_section,
   linetype_entry
   } linetype_t;

typedef struct scriptline
   {
   int32  type;
   void * ptr;
   struct scriptline *nextline;
   struct scriptline *prevline;
   } ScriptLineType;

typedef struct scriptentry
   {
   char * name;
   char * value;
   struct scriptentry *nextentry;
   struct scriptentry *preventry;
   } ScriptEntryType;

typedef struct scriptsection
   {
   char * name;
   ScriptEntryType      *entries;
   ScriptLineType       *lastline;
   struct scriptsection *nextsection;
   struct scriptsection *prevsection;
   } ScriptSectionType;

#endif
