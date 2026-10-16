/*

	tensw.h

	interface to TEN API for Shadow Warrior

	Copyright 1997 T E Network, Inc.  All rights reserved

*/


#ifndef TENSW_H
#define TENSW_H


/* Constants */

#define TENSW					// switches on TEN modifications
//#define TICDUP				// enables packet reduction


#ifdef __cplusplus
extern "C"
{
#endif

/* Globals */

extern int gTenActivated;
extern void *gTenLog, *gSyncLog;


/* Prototypes */

int tenSwInit(void);
int tenSwSendPacket(int nIndex, char *bufptr, int buflen);
short tenSwGetPacket(short *nIdx, char *buffer);
int tenSwScore(int killer, int killee);
int tenLogData(char *heading, long num, void *buffer, long size);
unsigned char tenChecksumChecksum(void *data);
void tenCalculateJitter(void);
void tenJitterStarve(int pnum);
void tenSwIdle(void);
void tenPlayerRoundup(void);
int tenPlayerDebugInfo(char *buffer, int i);
void tenResetClock(void);
void tenSyncLprintf(void *logRef, int queue, const char *format, ...);
void tenSyncLprintfBin(void *logRef, int queue, void *data, int size);

#ifdef __cplusplus
}
#endif


#endif /* TENSW_H */
