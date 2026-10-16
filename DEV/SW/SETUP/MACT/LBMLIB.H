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

/////////////////////////////////////////////////////////////////////////////
//
//      LBMLIB.H
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _lbmlib_public_
#define _lbmlib_public_
#ifdef __cplusplus
extern "C" {
#endif

typedef enum
   {
   LoadLBM_Okay,
   LoadLBM_badsize,
   LoadLBM_stencil
   } LoadLBM_Status;

int32 LoadLBM
   (
   byte * compressed,
   byte * uncompressed,
   byte * palette,
   int32 * lbmwidth,
   int32 * lbmheight
   );

int32 SaveLBM
   (
   byte * uncompressed,
   byte * compressed,
   byte * palette,
   int32 lbmwidth,
   int32 lbmheight
   );
#ifdef __cplusplus
};
#endif
#endif
