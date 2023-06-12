// Windows GDI for VI module, to support homedev.
#include "pch.h"

using namespace Debug;

static HWND     savedHwnd;
static HDC      hdcMainWnd, hdcWndComp;
static HBITMAP  hbmDIBSection;
static HGDIOBJ  oldSelected;

static BOOL     gdi_init;
static int      gdi_width, gdi_height;
static int      bm_width, bm_height;

bool VideoOutOpen(HWConfig* config, int width, int height, RGB **gfxbuf)
{
    HDC         hdc;
    BITMAPINFO* bmi;
    uint8_t     *DIBase;

    if(gdi_init == TRUE) return TRUE;

    Report(Channel::VI, "Windows DIB for video interface\n");

    savedHwnd = (HWND)config->renderTarget;

    bmi = (BITMAPINFO *)calloc(sizeof(BITMAPINFO) + 16*4, 1);
    if(bmi == NULL) return FALSE;

    hdcMainWnd = hdc = GetDC(savedHwnd);

    memset(bmi, 0, sizeof(BITMAPINFO));
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bm_width  = bmi->bmiHeader.biWidth = width;
    bm_height = bmi->bmiHeader.biHeight = -height;
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = 32;
    bmi->bmiHeader.biCompression = BI_RGB;

    hbmDIBSection = CreateDIBSection(NULL, bmi, DIB_RGB_COLORS, (void **)&DIBase, NULL, 0);
    if(hbmDIBSection == NULL) return FALSE;
    *gfxbuf = (RGB*)DIBase;

    hdcWndComp = CreateCompatibleDC(hdc);
    if(hdcWndComp == NULL) return FALSE;
    oldSelected = SelectObject(hdcWndComp, hbmDIBSection);

    free(bmi);

    VideoOutResize(width, height);
    gdi_init = TRUE;

    return TRUE;
}

void VideoOutClose()
{
    if(gdi_init == FALSE) return;

    if(hbmDIBSection)
    {
        SelectObject(hdcWndComp, oldSelected);
        DeleteObject(hbmDIBSection);
        hbmDIBSection = NULL;
    }

    if(hdcWndComp)
    {
        DeleteDC(hdcWndComp);
        hdcWndComp = NULL;
    }

    if(hdcMainWnd)
    {
        ReleaseDC(savedHwnd, hdcMainWnd);
        hdcMainWnd = NULL;
    }

    gdi_init = FALSE;
}

void VideoOutRefresh()
{
    if(gdi_init)
    {
        BitBlt(
            hdcMainWnd,
            0, 0, 
            gdi_width, gdi_height,
            hdcWndComp,
            0, 0,
            SRCCOPY
        );
    }
}

void VideoOutResize(int width, int height)
{
    gdi_width  = width;
    gdi_height = height;
}
