//****************************************************************************
//
// common.h
//
// common defines for the setup program
//
//****************************************************************************

#ifndef _common_public_
#define _common_public_
#ifdef __cplusplus
extern "C" {
#endif

//****************************************************************************
//
// DEFINES
//
//****************************************************************************

//
// Color Defines
//

#define MENUBACK_FOREGROUND COLOR_BLACK
#define MENUBACK_BACKGROUND COLOR_DARKGRAY

#define MENUBACKBORDER_FOREGROUND COLOR_BLACK
#define MENUBACKBORDER_BACKGROUND COLOR_GRAY

#define MENU_ACTIVE_FOREGROUND   COLOR_WHITE
#define MENU_INACTIVE_FOREGROUND COLOR_GRAY
#define MENU_DISPLAY_FOREGROUND  COLOR_LIGHTGREEN

#define MENU_SECTIONHEADER_FOREGROUND   COLOR_YELLOW

//
// Setup program defines
//

#define SETUPFILENAME "SW.CFG"


#define SETUPPROGRAMNAME ("Shadow Warrior 3D Setup")
#define SETUPPROGRAMVERSION ("Alpha .9")

#define GAMENAME "Shadow Warrior 3D"
#define GAMELAUNCHER ("SW.EXE")
#define GAMETOTYPE ("SW")

#define MENUFOOTER "Esc Exits   Move  ды Selects\0"

#define COMMITLAUNCHER ("COMMIT.EXE")
#define COMMITFILENAME ("COMMIT.DAT")

#define MAXVOICES 8
#define SONGNAME ("Shadow Warrior Theme Song")
//#define SOUNDSETUPLAUNCHER ("SNDSETUP.EXE")

// Default Socket Number

#define DEFAULTSOCKETNUMBER 0x8849

// Default RTS file

#define DEFAULTRTSFILE "SW.RTS"

// Default RTS path

#define DEFAULTRTSPATH ".\\"

// Default UserLevel path

#define DEFAULTLEVELPATH ".\\"

// Default External Control file

#define DEFAULTCONTROLFILE "EXTERNAL.EXE"

// Default Help file

#define DEFAULTHELPFILE "SW3DHELP.EXE"

// RTS extension

#define RTSEXTENSION "RTS"

// MAP extension

#define MAPEXTENSION "MAP"

// Default Player name

#define DEFAULTPLAYERNAME "KATO"

// Default Macros

#define MACRO1  "An inspiration for birth control."
#define MACRO2  "You're gonna die for that!"
#define MACRO3  "It hurts to be you."
#define MACRO4  "Lucky Son of a Bitch."
#define MACRO5  "Hmmm....Payback time."
#define MACRO6  "You bottom dwelling scum sucker."
#define MACRO7  "Damn, you're ugly."
#define MACRO8  "Ha ha ha...Wasted!"
#define MACRO9  "You suck!"
#define MACRO10 "AARRRGHHHHH!!!"

#ifdef __cplusplus
};
#endif
#endif
