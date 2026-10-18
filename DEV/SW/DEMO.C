// This file has been modified from Ken Silverman's original release
//#define MAIN
//#define QUIET
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <sys\types.h>
#include <time.h>
#include <direct.h>
#include <io.h>
#include <sys\stat.h>
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "net.h"

#include "types.h"
#include "control.h"
#include "function.h"
#include "def.h"
#include "demo.h"

DFILE DemoFileIn;
FILE *DemoFileOut;
BOOL DemoPlaying = FALSE;
BOOL DemoRecording = FALSE;
BOOL DemoEdit = FALSE;
BOOL DemoMode = FALSE;
BOOL DemoModeMenuState = FALSE;
BOOL DemoOverride = FALSE;
CHAR DemoFileName[80] = "demo.dmo";
CHAR DemoLevelName[16] = "";
extern BOOL NewGame;
extern BOOL TenScreenMode = FALSE;

// Demo sync stuff
FILE *DemoSyncFile;
BOOL DemoSyncTest = FALSE, DemoSyncRecord = FALSE;
char DemoTmpName[16] = "";

SW_PACKET DemoBuffer[DEMO_BUFFER_MAX];
long DemoRecCnt = 0;                    // One packet per connected player per tic
static long DemoBufferNumber = 0;       // Which block is currently loaded

BOOL DemoDone;



//
// DemoDebug Vars
//

// DemoDebugMode will close the file after every write
BOOL DemoDebugMode = FALSE;
//BOOL DemoDebugMode = TRUE;
BOOL DemoInitOnce = FALSE;
short DemoDebugBufferMax = 1;

extern char LevelName[];
extern char LevelSong[16];
extern BYTE FakeMultiNumPlayers;
extern BOOL QuitFlag;

/* SAVE.C consults this only while writing/loading private demo checkpoints. */
char DemoCheckpointOverride[80] = "";
extern int SaveGame(short save_num);
extern int LoadGame(short save_num);
extern char SaveGameDescr[10][80];
extern VOID DemoCheckpointMemoryClear(VOID);
extern long lastUpdate;
extern long PlayClock;
extern long ototalclock;
extern unsigned long MoveThingsCount;
extern BOOL MoveSkip2, MoveSkip4, MoveSkip8;

/* Automatic multiplayer demo recording metadata. */
static BOOL DemoAutoRecording = FALSE;
static char DemoAutoPlayerNames[MAX_SW_PLAYERS][32];
static short DemoAutoPlayerKills[MAX_SW_PLAYERS];
static char DemoAutoPlayerColors[MAX_SW_PLAYERS];
static BOOL DemoAutoPlayerCaptured[MAX_SW_PLAYERS];
static short DemoAutoPlayerCount = 0;
static short DemoAutoScoreboardCount = 0;
static BOOL DemoAutoScoreboardAvailable = FALSE;
static short DemoAutoLevel = 0;
static char DemoAutoMapName[80] = "";
static BOOL DemoAutoBots = FALSE;
static BYTE DemoAutoBotCount = 0;

extern int BotMode;
extern BOOL MapCopyEnabled;

///////////////////////////////////////////    
//
// Demo File Manipulation
//    
///////////////////////////////////////////    

char *DemoSyncFileName(VOID)
    {
    static char file_name[96];
    char *ptr;
    
    strcpy(file_name, DemoFileName);
    
    if ((ptr = strchr(file_name, '.')) == 0)
        strcat(file_name, ".dms");
    else
        {
        *ptr = '\0';
        strcat(file_name, ".dms");
        }
    
    return(file_name);    
    }
    
VOID
DemoSetup(VOID)
    {
    if (DemoRecording)
        {
        if (DemoSyncRecord)
            DemoSyncFile = fopen(DemoSyncFileName(),"wb");
            
        DemoWriteHeader();
        memset(&DemoBuffer, -1, sizeof(DemoBuffer));
        }

    if (DemoPlaying)
        {
        if (DemoSyncRecord)
            DemoSyncFile = fopen(DemoSyncFileName(),"wb");
        if (DemoSyncTest)
            DemoSyncFile = fopen(DemoSyncFileName(),"rb");

        DemoReadHeader();
        memset(&DemoBuffer, -1, sizeof(DemoBuffer));
        DemoReadBuffer();
        }
    }
    
VOID
DemoRecordSetup(VOID)
    {
    if (DemoRecording)
        {
        if (DemoSyncRecord)
            DemoSyncFile = fopen(DemoSyncFileName(),"wb");
            
        DemoWriteHeader();
        memset(&DemoBuffer, -1, sizeof(DemoBuffer));
        }
    }

VOID
DemoPlaySetup(VOID)
    {
    if (DemoPlaying)
        {
        if (DemoSyncRecord)
            DemoSyncFile = fopen(DemoSyncFileName(),"wb");
        if (DemoSyncTest)
            DemoSyncFile = fopen(DemoSyncFileName(),"rb");

        DemoReadHeader();
        memset(&DemoBuffer, -1, sizeof(DemoBuffer));
        DemoReadBuffer();
        }
    }
    
VOID
DemoWriteHeader(VOID)
    {
    DEMO_HEADER dh;
    DEMO_START_POS dsp;
    PLAYERp pp;

    DemoFileOut = fopen(DemoFileName, "wb");

    if (!DemoFileOut)
        {
        printf("Could not create demo file %s; recording disabled.\n", DemoFileName);
        DemoRecording = FALSE;
        DemoAutoRecording = FALSE;
        return;
        }

    strcpy(dh.map_name, LevelName);
    strcpy(dh.LevelSong, LevelSong);
    dh.Level = Level;

    if (FakeMultiNumPlayers)
        dh.numplayers = FakeMultiNumPlayers;
    else
        dh.numplayers = numplayers;

    fwrite(&dh, sizeof(dh), 1, DemoFileOut);

    for (pp = Player; pp < Player + dh.numplayers; pp++)
        {
        dsp.x = pp->posx;
        dsp.y = pp->posy;
        dsp.z = pp->posz;
        fwrite(&dsp, sizeof(dsp), 1, DemoFileOut);
        fwrite(&pp->Flags, sizeof(pp->Flags), 1, DemoFileOut);
        fwrite(&pp->pang, sizeof(pp->pang), 1, DemoFileOut);
        }
        
    fwrite(&Skill, sizeof(Skill), 1, DemoFileOut);    
    fwrite(&gNet, sizeof(gNet), 1, DemoFileOut);
    
    if (DemoDebugMode)    
        {
        DemoDebugBufferMax = numplayers;
        fclose(DemoFileOut);
        }
    }

/* Start recording a multiplayer match to DEMOS\DDMMYY##.DMO. The date
   and next daily sequence are prepared before DemoRecordSetup writes the header. */
VOID
DemoAutoRecordStart(VOID)
    {
    struct stat dir_info;
    time_t record_stamp;
    struct tm *record_time;
    int i, attempt, next_sequence, extended_sequence;

    if (DemoRecording || DemoPlaying || DemoEdit || DemoMode)
        return;

    if (mkdir("DEMOS") != 0)
        {
        if (stat("DEMOS", &dir_info) != 0 || !(dir_info.st_mode & S_IFDIR))
            {
            printf("Automatic demo recording disabled: cannot create DEMOS directory.\n");
            return;
            }
        }

    /* Use the DOS system date and preserve the highest existing daily number.
       Sequences 01-99 are numeric; 100-359 use A0-Z9. Scanning existing files
       makes numbering survive game restarts. */
    time(&record_stamp);
    record_time = localtime(&record_stamp);
    if (record_time == NULL || record_time->tm_mday < 1 ||
        record_time->tm_mday > 31 || record_time->tm_mon < 0 ||
        record_time->tm_mon > 11)
        {
        printf("Automatic demo recording disabled: could not read the system date.\n");
        return;
        }

    next_sequence = 1;
    for (attempt = 1; attempt <= 359; attempt++)
        {
        if (attempt <= 99)
            sprintf(DemoFileName, "DEMOS\\%02d%02d%02d%02d.DMO",
                    record_time->tm_mday, record_time->tm_mon + 1,
                    (record_time->tm_year + 1900) % 100, attempt);
        else
            {
            extended_sequence = attempt - 100;
            sprintf(DemoFileName, "DEMOS\\%02d%02d%02d%c%d.DMO",
                    record_time->tm_mday, record_time->tm_mon + 1,
                    (record_time->tm_year + 1900) % 100,
                    'A' + (extended_sequence / 10), extended_sequence % 10);
            }

        if (access(DemoFileName, 0) == 0)
            next_sequence = attempt + 1;
        }

    if (next_sequence > 359)
        {
        printf("Automatic demo recording disabled: daily demo number limit (359) has been reached.\n");
        return;
        }

    if (next_sequence <= 99)
        sprintf(DemoFileName, "DEMOS\\%02d%02d%02d%02d.DMO",
                record_time->tm_mday, record_time->tm_mon + 1,
                (record_time->tm_year + 1900) % 100, next_sequence);
    else
        {
        extended_sequence = next_sequence - 100;
        sprintf(DemoFileName, "DEMOS\\%02d%02d%02d%c%d.DMO",
                record_time->tm_mday, record_time->tm_mon + 1,
                (record_time->tm_year + 1900) % 100,
                'A' + (extended_sequence / 10), extended_sequence % 10);
        }

    DemoRecCnt = 0;
    DemoDebugMode = FALSE;

    DemoAutoPlayerCount = numplayers;
    if (DemoAutoPlayerCount < 1)
        DemoAutoPlayerCount = 1;
    if (DemoAutoPlayerCount > MAX_SW_PLAYERS)
        DemoAutoPlayerCount = MAX_SW_PLAYERS;

    DemoAutoLevel = Level;
    strncpy(DemoAutoMapName, LevelName, sizeof(DemoAutoMapName) - 1);
    DemoAutoMapName[sizeof(DemoAutoMapName) - 1] = '\0';
    DemoAutoBots = (BotMode && FakeMultiNumPlayers > 0);
    DemoAutoBotCount = FakeMultiNumPlayers;
    DemoAutoScoreboardCount = 0;
    DemoAutoScoreboardAvailable = FALSE;

    for (i = 0; i < MAX_SW_PLAYERS; i++)
        {
        DemoAutoPlayerNames[i][0] = '\0';
        DemoAutoPlayerKills[i] = 0;
        DemoAutoPlayerColors[i] = 0;
        DemoAutoPlayerCaptured[i] = FALSE;
        if (i < DemoAutoPlayerCount)
            {
            strncpy(DemoAutoPlayerNames[i], Player[i].PlayerName,
                    sizeof(DemoAutoPlayerNames[i]) - 1);
            DemoAutoPlayerNames[i][sizeof(DemoAutoPlayerNames[i]) - 1] = '\0';
            DemoAutoPlayerColors[i] = Player[i].TeamColor;
            }
        }

    DemoRecording = TRUE;
    DemoAutoRecording = TRUE;
    }

/* Capture the values actually displayed by the multiplayer scoreboard. */
VOID
DemoAutoRecordCapturePlayer(short player_index, char *player_name, short kills)
    {
    if (!DemoAutoRecording)
        return;
    if (player_index < 0 || player_index >= MAX_SW_PLAYERS)
        return;

    if (player_name != NULL)
        {
        strncpy(DemoAutoPlayerNames[player_index], player_name,
                sizeof(DemoAutoPlayerNames[player_index]) - 1);
        DemoAutoPlayerNames[player_index][sizeof(DemoAutoPlayerNames[player_index]) - 1] = '\0';
        }
    DemoAutoPlayerKills[player_index] = kills;
    DemoAutoPlayerCaptured[player_index] = TRUE;
    }

VOID
DemoAutoRecordScoreboardComplete(short player_count)
    {
    int i;

    if (!DemoAutoRecording || player_count < 1 || player_count > MAX_SW_PLAYERS)
        return;

    for (i = 0; i < player_count; i++)
        if (!DemoAutoPlayerCaptured[i])
            return;

    DemoAutoScoreboardCount = player_count;
    DemoAutoScoreboardAvailable = TRUE;
    }

static int DemoAutoCopyFile(char *source_name, char *dest_name)
    {
    FILE *source, *dest;
    char buffer[4096];
    size_t bytes_read;
    int ok = TRUE;

    source = fopen(source_name, "rb");
    if (source == NULL)
        return FALSE;

    dest = fopen(dest_name, "wb");
    if (dest == NULL)
        {
        fclose(source);
        return FALSE;
        }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source)) > 0)
        if (fwrite(buffer, 1, bytes_read, dest) != bytes_read)
            {
            ok = FALSE;
            break;
            }

    if (ferror(source))
        ok = FALSE;
    if (fclose(source) != 0)
        ok = FALSE;
    if (fclose(dest) != 0)
        ok = FALSE;
    return ok;
    }

static void DemoAutoBuildMapPart(char *map_part, unsigned map_part_size)
    {
    char map_copy[80];
    char *leaf, *dot, *slash, *backslash;

    if (DemoAutoLevel > 0)
        {
        sprintf(map_part, "L%d", (int)DemoAutoLevel);
        return;
        }

    strncpy(map_copy, DemoAutoMapName, sizeof(map_copy) - 1);
    map_copy[sizeof(map_copy) - 1] = '\0';
    slash = strrchr(map_copy, '/');
    backslash = strrchr(map_copy, '\\');
    leaf = map_copy;
    if (slash != NULL && (backslash == NULL || slash > backslash))
        leaf = slash + 1;
    else if (backslash != NULL)
        leaf = backslash + 1;

    dot = strrchr(leaf, '.');
    if (dot != NULL)
        *dot = '\0';
    if (leaf[0] == '\0')
        leaf = "UNKNOWN";

    strncpy(map_part, leaf, map_part_size - 1);
    map_part[map_part_size - 1] = '\0';
    }

static int DemoAutoWriteWB(char *file_name, char *demo_base, char *copied_map_name)
    {
    FILE *out;
    int i, instance_count, player_index, color;
    char map_arg[96];
    char *dot, *map_source_name;

    out = fopen(file_name, "wt");
    if (out == NULL)
        return FALSE;

    instance_count = DemoAutoBots ? 1 : DemoAutoPlayerCount;
    if (instance_count < 1)
        instance_count = 1;

    fprintf(out, "PLAYERS = %d\n", instance_count);
    fprintf(out, "---\n");

    map_arg[0] = '\0';
    if (DemoAutoLevel <= 0)
        {
        /* If the map was archived, launch the copy beside this WB file. */
        if (copied_map_name != NULL && copied_map_name[0] != '\0')
            map_source_name = copied_map_name;
        else
            map_source_name = DemoAutoMapName;
        strncpy(map_arg, map_source_name, sizeof(map_arg) - 1);
        map_arg[sizeof(map_arg) - 1] = '\0';
        dot = strrchr(map_arg, '.');
        if (dot == NULL || stricmp(dot, ".map") != 0)
            strcat(map_arg, ".map");
        }

    for (i = 0; i < instance_count; i++)
        {
        player_index = DemoAutoBots ? 0 : i;
        color = (int)DemoAutoPlayerColors[player_index];
        if (color < 0 || color > 7)
            color = 0;

        fprintf(out, "INSTANCE%d = MOUNT C . & C: & ", i + 1);
        if (DemoAutoBots)
            {
            fprintf(out, "vcommit sw.exe -dp%s.DMO -bots%d ",
                    demo_base, (int)DemoAutoBotCount);
            }
        else
            {
            if (i == 0)
                fprintf(out, "ipxnet startserver & ");
            else
                fprintf(out, "ipxnet connect localhost & ");
            fprintf(out, "ipxsetup -player %d -nodes %d vcommit sw.exe -dp%s.DMO ",
                    i + 1, instance_count, demo_base);
            }

        if (DemoAutoLevel > 0)
            fprintf(out, "-level%d ", (int)DemoAutoLevel);
        else if (map_arg[0] != '\0')
            fprintf(out, "-map %s ", map_arg);

        fprintf(out, "-autonet 0,3,0,1,1,0,20,0,%d,0 -name %s\n",
                color, DemoAutoPlayerNames[player_index][0] ?
                DemoAutoPlayerNames[player_index] : "Player");
        }

    if (fclose(out) != 0)
        return FALSE;
    return TRUE;
    }

/* Resolve a user-map filename, adding .map when it is omitted. */
static int DemoAutoResolveMapFile(char *map_file, unsigned map_file_size)
    {
    char *dot;
    size_t len;

    if (DemoAutoMapName[0] == '\0' || map_file == NULL || map_file_size == 0)
        return FALSE;

    strncpy(map_file, DemoAutoMapName, map_file_size - 1);
    map_file[map_file_size - 1] = '\0';
    dot = strrchr(map_file, '.');
    if (dot == NULL || stricmp(dot, ".map") != 0)
        {
        len = strlen(map_file);
        if (len + 4 >= map_file_size)
            return FALSE;
        strcat(map_file, ".map");
        }
    return TRUE;
    }

/* Copy a user map beside the archived demo and return its basename for WB. */
static int DemoAutoCopyMap(char *folder_path, char *copied_map_name,
                           unsigned copied_name_size)
    {
    char source_map[96], dest_map[128];
    char *slash, *backslash, *leaf;

    if (!MapCopyEnabled || DemoAutoLevel > 0 || copied_map_name == NULL ||
        copied_name_size == 0)
        return FALSE;
    copied_map_name[0] = '\0';
    if (!DemoAutoResolveMapFile(source_map, sizeof(source_map)))
        return FALSE;

    slash = strrchr(source_map, '/');
    backslash = strrchr(source_map, '\\');
    leaf = source_map;
    if (slash != NULL && (backslash == NULL || slash > backslash))
        leaf = slash + 1;
    else if (backslash != NULL)
        leaf = backslash + 1;
    if (leaf[0] == '\0' || strlen(leaf) >= copied_name_size)
        return FALSE;

    strcpy(copied_map_name, leaf);
    sprintf(dest_map, "%s\\%s", folder_path, leaf);
    if (!DemoAutoCopyFile(source_map, dest_map))
        {
        copied_map_name[0] = '\0';
        return FALSE;
        }
    return TRUE;
    }

/* Return the size of a user-supplied map file without reading its contents.
   Campaign levels are excluded by the caller. */
static int DemoAutoMapFileSize(unsigned long *size_result)
    {
    char map_file[96];
    struct stat map_info;

    if (size_result == NULL ||
        !DemoAutoResolveMapFile(map_file, sizeof(map_file)))
        return FALSE;

    if (stat(map_file, &map_info) != 0 || map_info.st_size < 0L)
        return FALSE;

    *size_result = (unsigned long)map_info.st_size;
    return TRUE;
    }

static int DemoAutoWriteTXT(char *file_name, char *demo_base, char *archive_label)
    {
    FILE *out;
    int i, count;
    unsigned long map_size;

    out = fopen(file_name, "wt");
    if (out == NULL)
        return FALSE;

    fprintf(out, "%s\n", archive_label);
    fprintf(out, "DEMO: %s.DMO\n", demo_base);
    
    if (DemoAutoLevel > 0)
        fprintf(out, "LEVEL: %d\n", (int)DemoAutoLevel);
    else
        {
        /* Report map size only for user maps, never for campaign levels. */
        if (DemoAutoMapFileSize(&map_size))
            fprintf(out, "MAP: %s | SIZE: %lu BYTES\n",
                    DemoAutoMapName, map_size);
        else
            fprintf(out, "MAP: %s | SIZE: UNAVAILABLE\n", DemoAutoMapName);
        }
	
	
	
    fprintf(out, "PLAYNUM: %d\n", (int)DemoAutoPlayerCount);

    fprintf(out, "---\n");
	if (DemoAutoScoreboardAvailable)
        fprintf(out, "SCOREBOARD\n");
    else
        fprintf(out, "SCORE UNAVAILABLE\n");

    count = DemoAutoScoreboardAvailable ? DemoAutoScoreboardCount : DemoAutoPlayerCount;
    for (i = 0; i < count; i++)
        {
        if (DemoAutoScoreboardAvailable && i < DemoAutoScoreboardCount)
            fprintf(out, "PLAYER %d: %s | KILLS: %d\n", i + 1,
                    DemoAutoPlayerNames[i], (int)DemoAutoPlayerKills[i]);
        else
            fprintf(out, "PLAYER %d: %s | KILLS: UNAVAILABLE\n", i + 1,
                    DemoAutoPlayerNames[i][0] ? DemoAutoPlayerNames[i] : "Unknown");
        }
    fprintf(out, "SUBMIT YOUR DEMO TO WANGBANG DEMO ARCHIVE! https://desomorphine.ru/sw/demos\n"); 
    if (fclose(out) != 0)
        return FALSE;
    return TRUE;
    }

/* Find the next persistent bundle number by scanning numeric subdirectories
   of DEMOS. This avoids a separate counter file and survives game restarts. */
static BOOL DemoAutoNextFolderNumber(unsigned long *next_number)
    {
    struct find_t dir_entry;
    char search_path[16];
    char *p;
    unsigned long value, max_number;
    int all_digits;

    if (next_number == NULL)
        return FALSE;

    strcpy(search_path, "DEMOS\\*.*");
    max_number = 0;

    if (_dos_findfirst(search_path, _A_SUBDIR, &dir_entry) == 0)
        {
        do
            {
            if (!(dir_entry.attrib & _A_SUBDIR))
                continue;
            if (!strcmp(dir_entry.name, ".") ||
                !strcmp(dir_entry.name, ".."))
                continue;

            value = 0;
            all_digits = TRUE;
            for (p = dir_entry.name; *p != '\0'; p++)
                {
                if (*p < '0' || *p > '9')
                    {
                    all_digits = FALSE;
                    break;
                    }
                value = value * 10UL + (unsigned long)(*p - '0');
                }

            if (all_digits && value > max_number)
                max_number = value;
            }
        while (_dos_findnext(&dir_entry) == 0);
        }

    /* DOS directory names have an eight-character component limit. */
    if (max_number >= 99999999UL)
        return FALSE;

    *next_number = max_number + 1UL;
    return TRUE;
    }

static VOID DemoAutoRecordFinish(VOID)
    {
    struct stat file_info;
    struct tm *file_time;
    time_t stamp;
    char source_name[96], folder_path[96], dest_demo[128];
    char dest_wb[128], dest_txt[128], demo_base[16];
    char copied_map_name[96];
    unsigned long folder_number;
    char map_part[80], date_part[16], player_part[384], archive_label[512];
    char kill_text[16];
    char *leaf, *dot, *slash, *backslash;
    int i, copied, map_copied, label_player_count;

    strcpy(source_name, DemoFileName);
    slash = strrchr(source_name, '/');
    backslash = strrchr(source_name, '\\');
    leaf = source_name;
    if (slash != NULL && (backslash == NULL || slash > backslash))
        leaf = slash + 1;
    else if (backslash != NULL)
        leaf = backslash + 1;

    strncpy(demo_base, leaf, sizeof(demo_base) - 1);
    demo_base[sizeof(demo_base) - 1] = '\0';
    dot = strrchr(demo_base, '.');
    if (dot != NULL)
        *dot = '\0';
    if (demo_base[0] == '\0')
        {
        printf("Automatic demo archive skipped: invalid demo filename.\n");
        return;
        }

    if (!DemoAutoNextFolderNumber(&folder_number))
        {
        printf("Automatic demo archive skipped: numeric DEMOS folders are exhausted.\n");
        return;
        }

    sprintf(folder_path, "DEMOS\\%lu", folder_number);
    if (mkdir(folder_path) != 0)
        {
        printf("Automatic demo archive skipped: could not create %s.\n", folder_path);
        return;
        }

    sprintf(dest_demo, "%s\\%s.DMO", folder_path, demo_base);
    sprintf(dest_wb, "%s\\%s.WB", folder_path, demo_base);
    sprintf(dest_txt, "%s\\%s.TXT", folder_path, demo_base);

    copied = DemoAutoCopyFile(DemoFileName, dest_demo);
    if (!copied)
        printf("Automatic demo archive warning: could not copy %s.\n", DemoFileName);

    /* Copy only user maps (never campaign levels), and only when enabled. */
    copied_map_name[0] = '\0';
    map_copied = FALSE;
    if (DemoAutoLevel <= 0 && MapCopyEnabled)
        {
        map_copied = DemoAutoCopyMap(folder_path, copied_map_name,
                                     sizeof(copied_map_name));
        if (!map_copied)
            printf("Automatic demo archive warning: could not copy user map %s.\n",
                   DemoAutoMapName);
        }

    DemoAutoBuildMapPart(map_part, sizeof(map_part));
    if (stat(DemoFileName, &file_info) == 0)
        stamp = file_info.st_mtime;
    else
        time(&stamp);
    file_time = localtime(&stamp);
    if (file_time != NULL)
        sprintf(date_part, "%02d.%02d.%04d", file_time->tm_mday,
                file_time->tm_mon + 1, file_time->tm_year + 1900);
    else
        strcpy(date_part, "00.00.0000");

    player_part[0] = '\0';
    label_player_count = DemoAutoScoreboardAvailable ?
                         DemoAutoScoreboardCount : DemoAutoPlayerCount;
    for (i = 0; i < label_player_count; i++)
        {
        if (i > 0)
            strcat(player_part, "x");
        strcat(player_part, DemoAutoPlayerNames[i][0] ?
               DemoAutoPlayerNames[i] : "Unknown");
        if (DemoAutoScoreboardAvailable && i < DemoAutoScoreboardCount)
            sprintf(kill_text, "[%d]", (int)DemoAutoPlayerKills[i]);
        else
            strcpy(kill_text, "[?]");
        strcat(player_part, kill_text);
        }

    strcpy(archive_label, player_part);
    strcat(archive_label, map_part);
    strcat(archive_label, "_");
    strcat(archive_label, date_part);

    if (!DemoAutoWriteWB(dest_wb, demo_base,
                         map_copied ? copied_map_name : NULL))
        printf("Automatic demo archive warning: could not write %s.\n", dest_wb);
    if (!DemoAutoWriteTXT(dest_txt, demo_base, archive_label))
        printf("Automatic demo archive warning: could not write %s.\n", dest_txt);

    printf("Automatic demo archive: %s\n", folder_path);
    printf("WANGEN-style name: %s\n", archive_label);
    }

VOID
DemoReadHeader(VOID)
    {
    DEMO_HEADER dh;
    DEMO_START_POS dsp;
    PLAYERp pp;

    #if DEMO_FILE_TYPE != DEMO_FILE_GROUP
    if (DemoEdit)
        {
        DemoFileIn = fopen(DemoFileName, "rb+");
        }
    else
    #endif    
        {
        //DemoFileIn = fopen(DemoFileName, "rb");
        DemoFileIn = DOPEN_READ(DemoFileName);
        }

    if (DemoFileIn == DF_ERR)
        {
        TerminateGame();
        printf("File %s is not a valid demo file.",DemoFileName);
        exit(0);
        }

    DREAD(&dh, sizeof(dh), 1, DemoFileIn);

    strcpy(DemoLevelName, dh.map_name);
    strcpy(LevelSong, dh.LevelSong);
    Level = dh.Level;
    if (dh.numplayers > 1)
        {
        FakeMultiNumPlayers = dh.numplayers;
        }
    else
        numplayers = dh.numplayers;

    for (pp = Player; pp < Player + dh.numplayers; pp++)
        {
        DREAD(&dsp, sizeof(dsp), 1, DemoFileIn);
        pp->posx = dsp.x;
        pp->posy = dsp.y;
        pp->posz = dsp.z;
        COVERupdatesector(pp->posx, pp->posy, &pp->cursectnum);
        //pp->cursectnum = 0;
        //updatesectorz(pp->posx, pp->posy, pp->posz, &pp->cursectnum);
        DREAD(&pp->Flags, sizeof(pp->Flags), 1, DemoFileIn);
        DREAD(&pp->pang, sizeof(pp->pang), 1, DemoFileIn);
        }
    
    DREAD(&Skill, sizeof(Skill), 1, DemoFileIn);    
    DREAD(&gNet, sizeof(gNet), 1, DemoFileIn);
    }

VOID
DemoDebugWrite(VOID)
    {
    long size;
    
    DemoFileOut = fopen(DemoFileName, "ab");

    ASSERT(DemoFileOut);
    
    size = sizeof(SW_PACKET) * DemoDebugBufferMax;
    fwrite(&DemoBuffer, size, 1, DemoFileOut);
    memset(&DemoBuffer, -1, size);
    
    fclose(DemoFileOut);
    }
    
VOID
DemoWriteBuffer(VOID)
    {
    fwrite(&DemoBuffer, sizeof(DemoBuffer), 1, DemoFileOut);
    memset(&DemoBuffer, -1, sizeof(DemoBuffer));
    }

VOID
DemoReadBuffer(VOID)
    {
    memset(&DemoBuffer, -1, sizeof(DemoBuffer));
    DREAD(&DemoBuffer, sizeof(DemoBuffer), 1, DemoFileIn);
    }

VOID
DemoBackupBuffer(VOID)
    {
    #if DEMO_FILE_TYPE != DEMO_FILE_GROUP
    FILE *NewDemoFile;
    FILE *OldDemoFile = DemoFileIn;
    int pos,i;
    char copy_buffer;
    char NewDemoFileName[96] = "!";

    // seek backwards to beginning of last buffer
    fseek(OldDemoFile, -sizeof(DemoBuffer), SEEK_CUR);
    pos = ftell(OldDemoFile);

    // open a new edit file
    strcat(NewDemoFileName, DemoFileName);
    NewDemoFile = fopen(NewDemoFileName, "wb");

    rewind(OldDemoFile);

    // copy old demo to new demo
    for (i = 0; i < pos; i++)
        {
        fread(&copy_buffer, sizeof(copy_buffer), 1, OldDemoFile);
        fwrite(&copy_buffer,sizeof(copy_buffer), 1, NewDemoFile);
        }

    DemoFileOut = NewDemoFile;
    fclose(OldDemoFile);
    #endif
    }

VOID
DemoTerm(VOID)
    {
    if (DemoRecording)
        {
        // if already closed
        if (DemoFileOut == NULL)
            {
            if (DemoAutoRecording)
                {
                DemoRecording = FALSE;
                DemoAutoRecording = FALSE;
                }
            return;
            }
            
        if (DemoDebugMode)
            {
            DemoFileOut = fopen(DemoFileName, "ab");
            ASSERT(DemoFileOut);
            }
        else 
            {
            // paste on a -1 record to the current buffer
            if (DemoRecCnt < DEMO_BUFFER_MAX)
                memset(&DemoBuffer[DemoRecCnt], -1, sizeof(DemoBuffer[DemoRecCnt]));

            DemoWriteBuffer();
            }

        // write at least 1 record at the end filled with -1
        // just for good measure
        memset(&DemoBuffer[0], -1, sizeof(DemoBuffer[0]));
        fwrite(&DemoBuffer[0], sizeof(DemoBuffer[0]), 1, DemoFileOut);

        fclose(DemoFileOut);
        DemoFileOut = NULL;

        /* Archive an automatic recording only after the DMO has been closed. */
        if (DemoAutoRecording)
            {
            DemoAutoRecordFinish();
            DemoRecording = FALSE;
            DemoAutoRecording = FALSE;
            }
        }

    if (DemoPlaying)
        {
        if (DemoFileIn == NULL)
            return;
            
        DCLOSE(DemoFileIn);
        DemoFileIn = NULL;
        }

    if (DemoSyncTest||DemoSyncRecord)
        {
        fclose(DemoSyncFile);
        DemoSyncFile = NULL;
        }
    }

///////////////////////////////////////////    
//
// Demo Play Back
//    
///////////////////////////////////////////    

/*
 * Rewind uses native gameplay checkpoints plus demo-file position and timing
 * metadata. R restores the closest earlier checkpoint and replays recorded
 * inputs invisibly to reconstruct an earlier world state.
 */
#define DEMO_REWIND_INTERVAL 120L
#define DEMO_REWIND_MAX_CHECKPOINTS 8L /* eight checkpoints, about 24 seconds at 40 tics/sec */
#define DEMO_REWIND_STEP 40L   /* one second at 40 demo tics/sec */

typedef struct
    {
    long tick;
    long buf_ndx;
    long buffer_number;
    long totalclock_value;
    long totalsynctics_value;
    long ototalclock_value;
    long randomseed_value;
    long numframes_value;
    long lastUpdate_value;
    long PlayClock_value;
    unsigned long MoveThingsCount_value;
    BOOL MoveSkip2_value;
    BOOL MoveSkip4_value;
    BOOL MoveSkip8_value;
    } DEMO_REWIND_INFO;

/* Rewind timing metadata stays in RAM alongside the serialized game state. */
static DEMO_REWIND_INFO DemoRewindInfoSlots[DEMO_REWIND_MAX_CHECKPOINTS];
static BOOL DemoRewindInfoValid[DEMO_REWIND_MAX_CHECKPOINTS];

static BOOL DemoRewindDisabled = FALSE;

static BOOL DemoRewindNames(long tick, char *save_name, char *info_name)
    {
    unsigned int slot;

    /* Keep the familiar slot names as stable IDs for the RAM ring. The same
       names are used only to clean up disk files from earlier prototypes. */
    if (tick < 0 || (unsigned long)tick > 0xFFFFFUL ||
        (tick % DEMO_REWIND_INTERVAL) != 0)
        return(FALSE);

    slot = (unsigned int)(((unsigned long)tick /
        (unsigned long)DEMO_REWIND_INTERVAL) %
        (unsigned long)DEMO_REWIND_MAX_CHECKPOINTS);
    sprintf(save_name, "RW%1X%1X.SAV",
        (unsigned)(myconnectindex & 0x0F), slot);
    sprintf(info_name, "RW%1X%1X.RWI",
        (unsigned)(myconnectindex & 0x0F), slot);
    return(TRUE);
    }

static BOOL DemoRewindSaveCheckpoint(long tick, long buf_ndx,
                                      long buffer_number)
    {
    DEMO_REWIND_INFO info;
    char save_name[16], info_name[16];
    int result;
    int slot;

    if (!DemoRewindEnabled || DemoRewindDisabled)
        return(FALSE);
    if (!DemoRewindNames(tick, save_name, info_name))
        return(FALSE);

    slot = (int)(((unsigned long)tick / (unsigned long)DEMO_REWIND_INTERVAL) %
                 (unsigned long)DEMO_REWIND_MAX_CHECKPOINTS);
    /* Invalidate the overwritten ring slot before beginning its new save. */
    DemoRewindInfoValid[slot] = FALSE;

    strcpy(DemoCheckpointOverride, save_name);
    result = SaveGame(0);
    DemoCheckpointOverride[0] = '\0';
    if (result != 0)
        {
        DemoRewindDisabled = TRUE;
        return(FALSE);
        }

    info.tick = tick;
    info.buf_ndx = buf_ndx;
    info.buffer_number = buffer_number;
    info.totalsynctics_value = totalsynctics;

    /* There is no disk I/O during checkpoint creation; keep timing anchored to
       demo tics so serialization cost never causes clock catch-up. */
    totalclock = totalsynctics;
    ototalclock = totalsynctics;
    info.totalclock_value = totalclock;
    info.ototalclock_value = ototalclock;
    info.randomseed_value = randomseed;
    info.numframes_value = numframes;
    info.lastUpdate_value = lastUpdate;
    info.PlayClock_value = PlayClock;
    info.MoveThingsCount_value = MoveThingsCount;
    info.MoveSkip2_value = MoveSkip2;
    info.MoveSkip4_value = MoveSkip4;
    info.MoveSkip8_value = MoveSkip8;

    DemoRewindInfoSlots[slot] = info;
    DemoRewindInfoValid[slot] = TRUE;
    return(TRUE);
    }

static BOOL DemoReadRewindInfo(long tick, DEMO_REWIND_INFO *info,
                                char *save_name)
    {
    char info_name[16];
    int slot;

    if (!DemoRewindEnabled || !DemoRewindNames(tick, save_name, info_name))
        return(FALSE);
    slot = (int)(((unsigned long)tick / (unsigned long)DEMO_REWIND_INTERVAL) %
                 (unsigned long)DEMO_REWIND_MAX_CHECKPOINTS);
    if (!DemoRewindInfoValid[slot] || DemoRewindInfoSlots[slot].tick != tick)
        return(FALSE);
    *info = DemoRewindInfoSlots[slot];
    return(TRUE);
    }

static BOOL DemoOpenBufferAt(long buffer_number)
    {
    DEMO_HEADER dh;
    long header_rest;
    long seek_amount;

    if (DemoFileIn != NULL && DemoFileIn != DF_ERR)
        DCLOSE(DemoFileIn);
    DemoFileIn = DOPEN_READ(DemoFileName);
    if (DemoFileIn == DF_ERR)
        return(FALSE);

    memset(&dh, 0, sizeof(dh));
    DREAD(&dh, sizeof(dh), 1, DemoFileIn);
    if (dh.numplayers < 1 || dh.numplayers > MAX_SW_PLAYERS)
        {
        DCLOSE(DemoFileIn);
        DemoFileIn = DF_ERR;
        return(FALSE);
        }

    /* Skip initial positions/flags/angles, skill and the network settings. */
    header_rest = (long)dh.numplayers *
        (long)(sizeof(DEMO_START_POS) + sizeof(Player[0].Flags) +
               sizeof(Player[0].pang));
    header_rest += (long)sizeof(Skill) + (long)sizeof(gNet);
    seek_amount = header_rest + buffer_number * (long)sizeof(DemoBuffer);
    if (klseek(DemoFileIn, seek_amount, SEEK_CUR) < 0)
        {
        DCLOSE(DemoFileIn);
        DemoFileIn = DF_ERR;
        return(FALSE);
        }

    DemoReadBuffer();
    return(TRUE);
    }

static BOOL DemoRewindRestore(long target_tick, long *tick, long *buf_ndx,
                               long *buffer_number)
    {
    DEMO_REWIND_INFO info;
    long checkpoint_tick;
    char save_name[16], old_descr[80];
    int result;

    if (!DemoRewindEnabled)
        return(FALSE);
    if (target_tick < 0)
        target_tick = 0;
    checkpoint_tick = target_tick - (target_tick % DEMO_REWIND_INTERVAL);
    if (!DemoReadRewindInfo(checkpoint_tick, &info, save_name))
        return(FALSE);

    strcpy(old_descr, SaveGameDescr[0]);
    strcpy(DemoCheckpointOverride, save_name);
    result = LoadGame(0);
    DemoCheckpointOverride[0] = '\0';
    strcpy(SaveGameDescr[0], old_descr);
    if (result != 0)
        return(FALSE);

    /* LoadGame resets several timing variables; put them back at this demo tic. */
    totalsynctics = info.totalsynctics_value;
    totalclock = info.totalclock_value;
    ototalclock = info.ototalclock_value;
    randomseed = info.randomseed_value;
    numframes = info.numframes_value;
    lastUpdate = info.lastUpdate_value;
    PlayClock = info.PlayClock_value;
    MoveThingsCount = info.MoveThingsCount_value;
    MoveSkip2 = info.MoveSkip2_value;
    MoveSkip4 = info.MoveSkip4_value;
    MoveSkip8 = info.MoveSkip8_value;

    if (!DemoOpenBufferAt(info.buffer_number))
        return(FALSE);

    DemoPlaying = TRUE;
    DemoDone = FALSE;
    *tick = info.tick;
    *buf_ndx = info.buf_ndx;
    *buffer_number = info.buffer_number;
    return(TRUE);
    }

static VOID DemoRewindCleanup(VOID)
    {
    long tick;
    int i;
    char save_name[16], info_name[16];

    /* Remove legacy files left by older disk-based prototypes, then release
       the in-memory ring and cached symbol tables. */
    for (tick = 0;
         tick < DEMO_REWIND_MAX_CHECKPOINTS * DEMO_REWIND_INTERVAL;
         tick += DEMO_REWIND_INTERVAL)
        if (DemoRewindNames(tick, save_name, info_name))
            {
            remove(save_name);
            remove(info_name);
            }

    for (i = 0; i < DEMO_REWIND_MAX_CHECKPOINTS; i++)
        DemoRewindInfoValid[i] = FALSE;
    DemoCheckpointMemoryClear();
    }

VOID
DemoPlayBack(VOID)
    {
    long pnum, cnt;
    static long buf_ndx;
    static BOOL qWasDown = FALSE;
    static BOOL rWasDown = FALSE;
    int shiftFastForwardPhase = 0;
    BOOL demoPaused = FALSE;
    BOOL seeking = FALSE;
    BOOL freshPlayback;
    long rewindTarget = 0;
    PLAYERp pp;
    ControlInfo info;
    long Xdim, Ydim, ScreenSize;

    #ifdef SW_SHAREWARE
    // code here needs to be similar to RunLevel startup code
    PlaySong(LevelSong);
    #endif
    
    
    // Initialize Game part of network code (When ready2send != 0)
    InitNetVars();
        
    // IMPORTANT - MUST be right before game loop
    InitTimingVars();

    // THIS STUFF DEPENDS ON MYCONNECTINDEX BEING SET RIGHT
    pp = Player + myconnectindex;
    SetRedrawScreen(pp);
    
    freshPlayback = !DemoInitOnce;
    if (freshPlayback)
        {
        buf_ndx = 0;
        DemoBufferNumber = 0;
        DemoRewindDisabled = FALSE;
        qWasDown = FALSE;
        rWasDown = FALSE;
        }

    // everything has been inited at least once for PLAYBACK
    DemoInitOnce = TRUE;

    cnt = 0;
    ready2send = 0;
    DemoDone = FALSE;

    /* Remove leftovers from a previous run or crash before checkpointing. */
    if (freshPlayback && DemoPlaying && !DemoMode && !DemoEdit)
        {
        DemoRewindCleanup();
        if (DemoRewindEnabled)
            DemoRewindSaveCheckpoint(0, buf_ndx, DemoBufferNumber);
        }

    while (TRUE)
        {
        // Playback-only controls are checked outside the tic loop so pause
        // remains responsive even when the game is fast-forwarding.
        if (DemoPlaying && !DemoMode && !DemoEdit && !seeking)
            {
            int qDown = KEY_PRESSED(KEYSC_Q) != 0;
            int rDown = KEY_PRESSED(KEYSC_R) != 0;

            if (qDown && !qWasDown)
                {
                demoPaused = !demoPaused;
                totalclock = totalsynctics;
                }
            qWasDown = qDown;

            if (DemoRewindEnabled && rDown && !rWasDown && cnt > 0)
                {
                rewindTarget = cnt - DEMO_REWIND_STEP;
                if (rewindTarget < 0)
                    rewindTarget = 0;
                if (DemoRewindRestore(rewindTarget, &cnt, &buf_ndx,
                                      &DemoBufferNumber))
                    {
                    /* Restore an earlier native state, then deterministically
                       replay the demo inputs to the requested earlier tic. */
                    demoPaused = FALSE;
                    seeking = (cnt < rewindTarget);
                    /* Start the hidden simulation immediately. While seeking,
                       the clock must stay just ahead of totalsynctics or the
                       main tick loop would never run. */
                    totalclock = totalsynctics + (seeking ? 1 : 0);
                    /* One rewind request per key press.  Clearing this latch
                       let a held R repeatedly call LoadGame after each seek,
                       tearing down the game state again and again. */
                    rWasDown = rDown;
                    continue;
                    }
                if (DemoFileIn == DF_ERR)
                    {
                    DemoDone = TRUE;
                    break;
                    }
                }
            rWasDown = rDown;
            }

        if (demoPaused)
            {
            // Discard elapsed wall-clock time while paused, avoiding a burst
            // of catch-up simulation when Q is pressed again.
            totalclock = totalsynctics;

            if (QuitFlag)
                {
                DemoMode = FALSE;
                break;
                }

            if (ExitLevel)
                {
                ExitLevel = FALSE;
                if (DemoMode)
                    {
                    DemoPlaying = FALSE;
                    DemoMode = FALSE;
                    }
                break;
                }

            drawscreen(Player + screenpeek);
            continue;
            }

        // makes code run at the same rate
        while (totalclock > totalsynctics)
            {
            TRAVERSE_CONNECT(pnum)
                {
                pp = Player + pnum;
                pp->inputfifo[pp->movefifoend & (MOVEFIFOSIZ-1)] = DemoBuffer[buf_ndx];
                pp->movefifoend++;
                buf_ndx++;

                if (pp->inputfifo[(pp->movefifoend - 1) & (MOVEFIFOSIZ-1)].bits == -1)
                    {
                    DemoDone = TRUE;
                    break;
                    }

                if (buf_ndx > DEMO_BUFFER_MAX - 1)
                    {
                    DemoReadBuffer();
                    buf_ndx = 0;
                    DemoBufferNumber++;
                    }
                }

            if (DemoDone)
                break;

            cnt++;

            if (!seeking)
                {
                CONTROL_GetInput(&info);
                MNU_CheckForMenus();
                }
            
            domovethings();
            
            // During rewind seeking, replay input quickly without drawing
            // intermediate frames. Keep the clock one or more ticks ahead of
            // totalsynctics so the hidden simulation does not stall.
            if (seeking)
                totalclock += synctics;
            else if (DemoPlaying || DemoEdit)
                {
                if (KEY_PRESSED(KEYSC_F))
                    {
                    if (KEY_PRESSED(KEYSC_LSHIFT) || KEY_PRESSED(KEYSC_RSHIFT))
                        {
                        // Faster than F, but don't let the clock outrun
                        // totalsynctics indefinitely. Alternate a +2 and +3
                        // adjustment (average +2.5 when synctics is 3).
                        totalclock += synctics - 1;
                        if (++shiftFastForwardPhase >= 2)
                            {
                            totalclock += 1;
                            shiftFastForwardPhase = 0;
                            }
                        }
                    else
                        {
                        shiftFastForwardPhase = 0;
                        totalclock += synctics - 1;
                        }
                    }
                else
                    shiftFastForwardPhase = 0;

                if (KEY_PRESSED(KEYSC_S))
                    totalclock += 1 - synctics;
                }

            if (!DemoEdit)
                {
                #if DEBUG
                if (KEY_PRESSED(KEYSC_ALT) && KEY_PRESSED(KEYSC_CTRL) && KEY_PRESSED(KEYSC_S))
                    {
                    KEY_PRESSED(KEYSC_ALT) = KEY_PRESSED(KEYSC_CTRL) = KEY_PRESSED(KEYSC_S) = 0;
                    saveboard("demosave.map", &Player->posx, &Player->posy, &Player->posz, &Player->pang, &Player->cursectnum);
                    }
                #endif

                if (!seeking && BUTTON(gamefunc_See_Co_Op_View))
                    {
                    CONTROL_ClearButton(gamefunc_See_Co_Op_View);

                    screenpeek = connectpoint2[screenpeek];

                    if (screenpeek < 0)
                        screenpeek = connecthead;
                    }

                #if DEBUG
                if (KEY_PRESSED(KEYSC_RIGHT) || KEY_PRESSED(KEYSC_UP))
                    {
                    if (KEY_PRESSED(KEYSC_LSHIFT) || KEY_PRESSED(KEYSC_RSHIFT))
                        totalclock += synctics;
                    else
                        totalclock += synctics - 1;
                    }

                if (KEY_PRESSED(KEYSC_LEFT) || KEY_PRESSED(KEYSC_DOWN))
                    totalclock += 1 - synctics;
                #endif
                }


            // Stop the rewind seek exactly at the requested earlier tic.
            if (seeking && cnt >= rewindTarget)
                {
                seeking = FALSE;
                totalclock = totalsynctics;
                }

            if (DemoRewindEnabled && !seeking && DemoPlaying && !DemoMode && !DemoEdit &&
                cnt > 0 && (cnt % DEMO_REWIND_INTERVAL) == 0)
                DemoRewindSaveCheckpoint(cnt, buf_ndx, DemoBufferNumber);

            if (DemoSyncRecord)
                demosync_record();
            if (DemoSyncTest)
               demosync_test(cnt);
            }

        // Put this back in later when keyboard stuff is stable
        if (DemoEdit)
            {
            //CONTROL_GetButtonInput();
            CONTROL_GetInput(&info);

            // if a key is pressed, start recording from the point the key
            // was pressed
            if (BUTTON(gamefunc_Move_Forward) ||
                BUTTON(gamefunc_Move_Backward) ||
                BUTTON(gamefunc_Turn_Left) ||
                BUTTON(gamefunc_Turn_Right) ||
                BUTTON(gamefunc_Fire) ||
                BUTTON(gamefunc_Open) ||
                BUTTON(gamefunc_Jump) ||
                BUTTON(gamefunc_Crouch) ||
                BUTTON(gamefunc_Look_Up) ||
                BUTTON(gamefunc_Look_Down))
                {
                DemoBackupBuffer();

                DemoRecCnt = buf_ndx;
                DemoPlaying = FALSE;
                DemoRecording = TRUE;
                return;
                }
            }

        if (!seeking && BUTTON(gamefunc_See_Co_Op_View))
            {
            screenpeek += 1;
            if (screenpeek > numplayers-1)
                screenpeek = 0;
            }

        // demo is over
        if (DemoDone)
            break;

        if (QuitFlag)    
            {
            DemoMode = FALSE;
            break;
            }

        if (ExitLevel)
            {
            // Quiting Demo
            ExitLevel = FALSE;
            if (DemoMode)
                {
                DemoPlaying = FALSE;
                DemoMode = FALSE;
                }
            break;
            }

        // While seeking, keep simulating the recorded input but suppress
        // rendering; once the target tic is reached, normal rendering resumes.
        if (seeking)
            continue;

        drawscreen(Player + screenpeek);
        }

    /* Release the in-memory checkpoint ring when playback ends. */
    if (!DemoMode && !DemoEdit)
        DemoRewindCleanup();

    // only exit if conditions are write
    if (DemoDone && !DemoMode && !NewGame)
        {
        TerminateLevel();
        TerminateGame();
        exit(0);
        }
    
    }

//
// Still using old method of playback - this was for opening demo
//    

VOID
ScenePlayBack(VOID)
    {
    long buf_ndx, pnum, cnt;
    PLAYERp pp;

    #ifdef SW_SHAREWARE
    // code here needs to be similar to RunLevel startup code
    strcpy(LevelSong,"yokoha03.mid");
    PlaySong(LevelSong);
    #endif

    // IMPORTANT - MUST be right before game loop
    InitTimingVars();

    buf_ndx = 0;
    cnt = 0;
    ready2send = 0;
    DemoDone = FALSE;

    ResetKeys();

    while (TRUE)
        {
        // makes code run at the same rate
        while ((totalclock > totalsynctics))
            {
            TRAVERSE_CONNECT(pnum)
                {
                pp = Player + pnum;
                pp->inputfifo[pp->movefifoend & (MOVEFIFOSIZ - 1)] = DemoBuffer[buf_ndx];
                pp->movefifoend++;
                buf_ndx++;

                if (pp->inputfifo[(pp->movefifoend - 1) & (MOVEFIFOSIZ - 1)].bits == -1)
                    {
                    DemoDone = TRUE;
                    break;
                    }

                if (buf_ndx > DEMO_BUFFER_MAX - 1)
                    {
                    DemoReadBuffer();
                    buf_ndx = 0;
                    }
                }

            if (KeyPressed())
                DemoDone = TRUE;

            if (DemoDone)
                break;

            cnt++;

            //movethings();
            domovethings();
            
            MNU_CheckForMenus();
            }

        // demo is over
        if (DemoDone)
            break;

        drawscreen(Player + screenpeek);
        }
    }


    
