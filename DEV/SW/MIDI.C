#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "build.h"
#undef MAXPLAYERS
#include "names2.h"
#include "types.h"
#include "sndcards.h"
#include "fx_man.h"
#include "music.h"
#include "util_lib.h"
#include "gamedefs.h"
#include "config.h"
#include "panel.h"
#include "game.h"
#include "sounds.h"
#include "ai.h"

char gm_on[] = {0xF0, 0x7E, 0x7F, 0x09, 0x01, 0xF7};
char gs_on[] = {0xF0, 0x41, 0x10, 0x42, 0x12, 0x40, 0x00, 0x7F, 0x00, 0x41, 0xF7};
char xg_on[] = {0xF0, 0x43, 0x10, 0x4C, 0x00, 0x00, 0x7E, 0x00, 0xF7};

// config adjustable    
//extern long MidiPort;
#define midi_status_port (MidiPort+1)
#define midi_status_busy_flag (BIT(6))

void SendGeneralMidiSysX(void)
    {
    short i;
    
    if (MusicDevice != GenMidi)
        return;
    
    for (i = 0; i < SIZ(gm_on); i++)
        MidiStatusOut(gm_on[i]);
        
    for (i = 0; i < SIZ(gs_on); i++)
        MidiStatusOut(gs_on[i]);
        
    for (i = 0; i < SIZ(xg_on); i++)
        MidiStatusOut(xg_on[i]);
    }

void MidiStatusOut(char data)
    {
    long count = 1024;
    
    while (count > 0 && TEST(inp(midi_status_port+1), midi_status_busy_flag))
        {
        count--;
        }
        
    outp(midi_status_port, data);
    }