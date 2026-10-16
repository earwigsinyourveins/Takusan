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

//****************************************************************************
//
// Public header for EXTERNAL controller support
//
//****************************************************************************

#ifndef _external_public_
#define _external_public_
#ifdef __cplusplus
extern "C" {
#endif

// External Controller ID

#define CONTROLID 0xdead
#define EXTERNALPARM "control"
#define EXTERNAL_GetInput  1
#define EXTERNALAXISUNDEFINED   0x7f
#define EXTERNALBUTTONUNDEFINED 0x7f
#define MAXEXTERNALAXES 6
#define MAXEXTERNALBUTTONS 32

typedef struct
   {
   word      id;
   word      intnum;
   int32     axes[MAXEXTERNALAXES];
   uint32    buttonstate;
   byte      buttonmap[MAXEXTERNALBUTTONS][2];
   byte      analogaxesmap[MAXEXTERNALAXES];
   word      command;
   byte      digitalaxesmap[MAXEXTERNALAXES][2];
   } ExternalControlInfo;

#ifdef __cplusplus
};
#endif
#endif
