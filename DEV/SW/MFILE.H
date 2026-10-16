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

#define FILE_ANSI_STD   0
#define FILE_KEN        1
#define FILE_DOS        2

//#define FILE_TYPE FILE_ANSI_STD
#define FILE_TYPE FILE_KEN

#if FILE_TYPE == FILE_ANSI_STD
typedef FILE *MFILE; 
#define MREAD(ptr, size, num,handle) fread((ptr),(size),(num),(handle))
#define MWRITE(ptr, size, num,handle) fwrite((ptr),(size),(num),(handle))
#define MOPEN_WRITE(name) fopen(name,"wb")
#define MOPEN_READ(name) fopen(name,"rb")
#define MCLOSE(handle) fclose(handle)
#define MF_ERR 0
#endif

#if FILE_TYPE == FILE_KEN
typedef FILE *MFILE; 
size_t dfread( void *, size_t, size_t, FILE * );
size_t dfwrite( void *, size_t, size_t, FILE * );
#define MREAD(ptr, size, num,handle) dfread((ptr),(size),(num),(handle))
#define MWRITE(ptr, size, num,handle) dfwrite((ptr),(size),(num),(handle))
#define MOPEN_WRITE(name) fopen(name,"wb")
#define MOPEN_READ(name) fopen(name,"rb")
#define MCLOSE(handle) fclose(handle)
#define MF_ERR 0
#endif

#if FILE_TYPE == FILE_DOS
typedef long MFILE; 
#define MREAD(ptr, size, num,handle) read((handle),(ptr),(size)*(num))
#define MWRITE(ptr, size, num,handle) write((handle),(ptr),(size)*(num))
#define MOPEN_WRITE(name) open(name,O_BINARY|O_TRUNC|O_CREAT|O_WRONLY,S_IWRITE)
#define MOPEN_READ(name) open(name,O_BINARY|O_RDWR,S_IREAD)
#define MCLOSE(handle) close(handle)
#define MF_ERR -1
#endif
