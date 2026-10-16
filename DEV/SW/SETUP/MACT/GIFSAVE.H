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

#ifndef GIFSAVE_H
#define GIFSAVE_H



enum GIF_Code {
    GIF_OK,
    GIF_ERRCREATE,
    GIF_ERRWRITE,
    GIF_OUTMEM
};



int  GIF_Create(
         char *filename,
         int width, int height,
         int numcolors, int colorres
     );

void GIF_SetColor(
         int colornum,
         int red, int green, int blue
     );

int  GIF_CompressImage(
         int left, int top,
         int width, int height,
         int (*getpixel)(int x, int y)
     );

int  GIF_Close(void);



#endif
