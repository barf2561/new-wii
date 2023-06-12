// Emulator includes

#pragma once

#define EMU_VERSION L"0.16"

void    EMUGetHwConfig(HWConfig* config);

// emulator controls API
void    EMUCtor();
void    EMUDtor();
void    EMUOpen(const std::wstring& filename);    // Power up system
void    EMUClose();         // Power down system
void    EMUReset();         // Reset
void    EMURun();           // Run Gekko
void    EMUStop();          // Stop Gekko

// all important data is placed here
typedef struct Emulator
{
    bool    init;
    bool    loaded;         // file loaded
    std::wstring lastLoaded;
} Emulator;

extern  Emulator emu;

#include "EmuCommands.h"
#include "JdiServer.h"
