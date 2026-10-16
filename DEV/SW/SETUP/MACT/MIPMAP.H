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

#ifndef _mipmap_public_
#define _mipmap_public_
#ifdef __cplusplus
extern "C" {
#endif

int32 CreateMipMap
   (
   int32 origw,
   int32 origh,
   int32 level,
   byte * srcbitmap,
   byte * destbitmap,
   byte * pal,
   boolean usediffusion
   );

#ifdef __cplusplus
};
#endif
#endif
