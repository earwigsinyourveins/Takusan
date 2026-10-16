// This file has been modified from Ken Silverman's original release
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

extern FILE *DemoFile;
extern BOOL DemoPlaying;
extern BOOL DemoRecording;
extern VOID DemoAutoRecordStart(VOID);
extern VOID DemoAutoRecordCapturePlayer(short player_index, char *player_name, short kills);
extern VOID DemoAutoRecordScoreboardComplete(short player_count);
extern BOOL DemoEdit;
extern BOOL DemoMode;
extern BOOL DemoOverride;
extern CHAR DemoFileName[80];
extern CHAR DemoLevelName[16];

extern FILE *DemoSyncFile;
extern BOOL DemoSyncTest;
extern BOOL DemoSyncRecord;
extern char DemoTmpName[16];

extern BOOL DemoDebugMode;
extern BOOL DemoInitOnce;
extern BOOL DemoRewindRestart;
extern BOOL DemoRewindEnabled;
extern short DemoDebugBufferMax;

extern SW_PACKET DemoBuffer[DEMO_BUFFER_MAX];
extern long DemoRecCnt;                    // One packet per connected player per tic

#define DEMO_FILE_GROUP 0
#define DEMO_FILE_STD   1
#define DEMO_FILE_TYPE DEMO_FILE_GROUP

// Demo File - reading from group
#if DEMO_FILE_TYPE == DEMO_FILE_GROUP
typedef long DFILE; 
#define DREAD(ptr, size, num, handle) kread((handle),(ptr),(size)*(num))
#define DOPEN_READ(name) kopen4load(name,0)
#define DCLOSE(handle) kclose(handle)
#define DF_ERR -1
#else
typedef FILE *DFILE; 
#define DREAD(ptr, size, num,handle) fread((ptr),(size),(num),(handle))
#define DWRITE(ptr, size, num,handle) fwrite((ptr),(size),(num),(handle))
#define DOPEN_WRITE(name) fopen(name,"wb")
#define DOPEN_READ(name) fopen(name,"rb")
#define DCLOSE(handle) fclose(handle)
#define DF_ERR 0
#endif

