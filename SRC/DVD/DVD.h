// common include project header

#pragma once

// DVD interface

namespace DVD
{
    void InitSubsystem();
    void ShutdownSubsystem();

    // Mount current DVD image for read/seek/open file operations
    bool MountFile(const wchar_t* file);
    bool MountFile(const std::string& file);
    bool MountFile(const std::wstring& file);

    // Mount DolphinSDK directory
    bool MountSdk(const wchar_t* path);
    bool MountSdk(std::string path);

    // Unmount
    void Unmount();

    bool IsMounted();

    // Seek and read operations on mounted DVD
    void Seek(int position);
    int GetSeek();
    bool Read(void* buffer, size_t length);

    // Open file in DVD root. Return file position, or 0 if no such file.
    // Note: DVD must be mounted first!
    // example use : long banner = DVDOpenFile("/opening.bnr");
    long OpenFile(std::string& dvdfile);
}

// other include files
#include "FileSystem.h"     // DVD file system, based on hotquik's code from Dolwin 0.09
#include "MountSDK.h"
#include "Region.h"

// all important data is placed here
struct DVDControl
{
    bool mountedImage;
    wchar_t gcm_filename[0x1000];
    int   gcm_size;       // size of mounted file
    int   seekval;        // current DVD position

    DVD::MountDolphinSdk* mountedSdk;
};

extern DVDControl dvd;             // share with other modules

// DDU Core
#include "DduCore.h"
