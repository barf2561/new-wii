#include "pch.h"

// Basic application output

namespace UI
{
    // fatal error
    void DolwinError(const wchar_t* title, const wchar_t* fmt, ...)
    {
        va_list arg;
        wchar_t buf[0x1000];

        va_start(arg, fmt);
        vswprintf_s(buf, _countof(buf) - 1, fmt, arg);
        va_end(arg);

        MessageBox(NULL, buf, title, MB_ICONHAND | MB_OK | MB_TOPMOST);
        if (IsWindow(wnd.hMainWindow))
        {
            SendMessage(wnd.hMainWindow, WM_CLOSE, 0, 0);
        }
        else
        {
            exit(-1);
        }
    }

    // application message
    void DolwinReport(const wchar_t* fmt, ...)
    {
        va_list arg;
        wchar_t buf[0x1000];

        va_start(arg, fmt);
        vswprintf_s(buf, _countof(buf) - 1, fmt, arg);
        va_end(arg);

        MessageBox(NULL, buf, APPNAME L" Reports", MB_ICONINFORMATION | MB_OK | MB_TOPMOST);
    }
}

// WinMain (very first run-time initialization and main loop)

// Check for multiple instancies.
static void LockMultipleCalls()
{
    static HANDLE dolwinsem;

    // mutex will fail if semephore already exists
    dolwinsem = CreateMutex(NULL, 0, APPNAME);
    if(dolwinsem == NULL)
    {
        auto app_name = std::wstring(APPNAME);
        UI::DolwinReport(L"We are already running %s!!", app_name);
        exit(0);    // return good
    }
    CloseHandle(dolwinsem);

    dolwinsem = CreateSemaphore(NULL, 0, 1, APPNAME);
}

static bool IsDirectoryExists(LPCWSTR szPath)
{
    DWORD dwAttrib = GetFileAttributesW(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
        (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

static void InitFileSystem(HINSTANCE hInst)
{
    wchar_t cwd[0x1000];

    // Set current working directory relative to Dolwin executable
    GetModuleFileName(hInst, cwd, sizeof(cwd));
    *(wcsrchr(cwd, L'\\') + 1) = 0;
    SetCurrentDirectory(cwd);
    
    // Check for default settings.

    if (!Util::FileExists(L"./Data/DefaultSettings.json"))
    {
        UI::DolwinError(L"Error",
            L"No default settings found.\n\n"
            L"If you are a developer and run Dolwin from Visual Studio, make sure your working directory (DolwinLegacy Properties -> Debugging -> Working Directory) is set to $(SolutionDir).\n\n"
            L"If you are a user, make sure that the main executable file Dolwin.exe is located in the directory with the `Data` folder. The `Data` folder contains all important data for the emulator to work.");
    }
}

// return file name without quotes
char* FixCommandLine(char *lpCmdLine)
{
    if(*lpCmdLine == '\"' || *lpCmdLine == '\'')
    {
        lpCmdLine++;
    }
    size_t len = strlen(lpCmdLine);
    if(lpCmdLine[len-1] == '\"' || lpCmdLine[len-1] == '\'')
    {
        lpCmdLine[len-1] = 0;
    }
    return lpCmdLine;
}

/* Keyboard accelerators (no need to be shared). */
HACCEL  hAccel;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nShowCmd);

    InitFileSystem(hInstance);

    // Create an interface for communicating with the emulator core

    UI::Jdi = new UI::JdiClient;

    // Allow only one instance of Dolwin to run at once?
    if (UI::Jdi->GetConfigBool(USER_RUNONCE, USER_UI))
    {
        LockMultipleCalls();
    }

    hAccel = LoadAccelerators(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_ACCELERATOR));

    // Start the emulator and user interface
    // (emulator will be initialized during main window creation).
    CreateMainWindow(hInstance);

    // Main loop
    MSG msg = { 0 };
    while (true)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == 0)
        {
            Sleep(1);
        }

        /* Idle loop */
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(wnd.hMainWindow, hAccel, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    // Should never reach this point. Dolwin always exits.
    UI::DolwinError ( L"Error", L"SHOULD NEVER REACH HERE" );
    return -2;
}
