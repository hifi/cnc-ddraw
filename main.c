/*
 * Copyright (c) 2010 Toni Spets <toni.spets@iki.fi>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include "ddraw.h"

#include "main.h"
#include "palette.h"
#include "surface.h"
#include "clipper.h"

/* from mouse.c */
void mouse_init(HWND);
void mouse_lock();
void mouse_unlock();

IDirectDrawImpl *ddraw = NULL;

DWORD WINAPI render_main(void);

HRESULT __stdcall ddraw_Compact(IDirectDrawImpl *This)
{
    printf("DirectDraw::Compact(This=%p)\n", This);

    return DD_OK;
}

HRESULT __stdcall ddraw_DuplicateSurface(IDirectDrawImpl *This, LPDIRECTDRAWSURFACE src, LPDIRECTDRAWSURFACE *dest)
{
    printf("DirectDraw::DuplicateSurface(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_EnumDisplayModes(IDirectDrawImpl *This, DWORD a, LPDDSURFACEDESC b, LPVOID c, LPDDENUMMODESCALLBACK d)
{
    printf("DirectDraw::EnumDisplayModes(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_EnumSurfaces(IDirectDrawImpl *This, DWORD a, LPDDSURFACEDESC b, LPVOID c, LPDDENUMSURFACESCALLBACK d)
{
    printf("DirectDraw::EnumSurfaces(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_FlipToGDISurface(IDirectDrawImpl *This)
{
    printf("DirectDraw::FlipToGDISurface(This=%p)\n", This);

    return DD_OK;
}

HRESULT __stdcall ddraw_GetCaps(IDirectDrawImpl *This, LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDEmulCaps)
{
    printf("DirectDraw::GetCaps(This=%p, lpDDDriverCaps=%p, lpDDEmulCaps=%p)\n", This, lpDDDriverCaps, lpDDEmulCaps);

    if(lpDDDriverCaps)
    {
        lpDDDriverCaps->dwSize = sizeof(DDCAPS);
        lpDDDriverCaps->dwCaps = DDCAPS_BLT|DDCAPS_PALETTE;
        lpDDDriverCaps->dwCKeyCaps = 0;
        lpDDDriverCaps->dwPalCaps = DDPCAPS_8BIT|DDPCAPS_PRIMARYSURFACE;
        lpDDDriverCaps->dwVidMemTotal = 16777216;
        lpDDDriverCaps->dwVidMemFree = 16777216;
        lpDDDriverCaps->dwMaxVisibleOverlays = 0;
        lpDDDriverCaps->dwCurrVisibleOverlays = 0;
        lpDDDriverCaps->dwNumFourCCCodes = 0;
        lpDDDriverCaps->dwAlignBoundarySrc = 0;
        lpDDDriverCaps->dwAlignSizeSrc = 0;
        lpDDDriverCaps->dwAlignBoundaryDest = 0;
        lpDDDriverCaps->dwAlignSizeDest = 0;
    }

    if(lpDDEmulCaps)
    {
        lpDDEmulCaps->dwSize = 0;
    }

    return DD_OK;
}

HRESULT __stdcall ddraw_GetDisplayMode(IDirectDrawImpl *This, LPDDSURFACEDESC a)
{
    printf("DirectDraw::GetDisplayMode(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_GetFourCCCodes(IDirectDrawImpl *This, LPDWORD a, LPDWORD b)
{
    printf("DirectDraw::GetFourCCCodes(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_GetGDISurface(IDirectDrawImpl *This, LPDIRECTDRAWSURFACE *a)
{
    printf("DirectDraw::GetGDISurface(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_GetMonitorFrequency(IDirectDrawImpl *This, LPDWORD a)
{
    printf("DirectDraw::GetMonitorFrequency(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_GetScanLine(IDirectDrawImpl *This, LPDWORD a)
{
    printf("DirectDraw::GetScanLine(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_GetVerticalBlankStatus(IDirectDrawImpl *This, LPBOOL a)
{
    printf("DirectDraw::GetVerticalBlankStatus(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_Initialize(IDirectDrawImpl *This, GUID *a)
{
    printf("DirectDraw::Initialize(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_RestoreDisplayMode(IDirectDrawImpl *This)
{
    printf("DirectDraw::RestoreDisplayMode(This=%p)\n", This);

    if(!This->render.run)
    {
        return DD_OK;
    }

    EnterCriticalSection(&This->cs);
    This->render.run = FALSE;
    LeaveCriticalSection(&This->cs);

    WaitForSingleObject(This->render.thread, INFINITE);
    This->render.thread = NULL;

    if(!ddraw->windowed)
    {
        This->mode.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFLAGS|DM_DISPLAYFREQUENCY|DM_POSITION;
        ChangeDisplaySettings(&This->mode, 0);
    }

    return DD_OK;
}

HRESULT __stdcall ddraw_SetDisplayMode(IDirectDrawImpl *This, DWORD width, DWORD height, DWORD bpp)
{
    printf("DirectDraw::SetDisplayMode(This=%p, width=%d, height=%d, bpp=%d)\n", This, (unsigned int)width, (unsigned int)height, (unsigned int)bpp);

    if(This->render.run)
    {
        return DD_OK;
    }

    This->render.run = TRUE;

    /* currently we only support 8 bit modes */
    if(bpp != 8)
    {
        return DDERR_INVALIDMODE;
    }

    This->mode.dmSize = sizeof(DEVMODE);
    This->mode.dmDriverExtra = 0;

    if(EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &This->mode) == FALSE)
    {
        /* not expected */
        return DDERR_UNSUPPORTED;
    }

    This->width = width;
    This->height = height;
    This->bpp = bpp;

    if(This->render.width < This->width)
    {
        This->render.width = This->width;
    }
    if(This->render.height < This->height)
    {
        This->render.height = This->height;
    }

    mouse_unlock();

    if(This->windowed)
    {
        if(!This->windowed_init)
        {
            SetWindowLong(This->hWnd, GWL_STYLE, GetWindowLong(This->hWnd, GWL_STYLE) | WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_MINIMIZEBOX);

            /* center the window with correct dimensions */
            int x = (This->mode.dmPelsWidth / 2) - (This->render.width / 2);
            int y = (This->mode.dmPelsHeight / 2) - (This->render.height / 2);
            RECT dst = { x, y, This->render.width+x, This->render.height+y };
            AdjustWindowRect(&dst, GetWindowLong(This->hWnd, GWL_STYLE), FALSE);
            SetWindowPos(This->hWnd, HWND_NOTOPMOST, dst.left, dst.top, (dst.right - dst.left), (dst.bottom - dst.top), SWP_SHOWWINDOW);

            This->windowed_init = TRUE;
        }
    }
    else
    {
        SetWindowPos(This->hWnd, HWND_TOPMOST, 0, 0, This->render.width, This->render.height, SWP_SHOWWINDOW);

        mouse_lock();

        memset(&This->render.mode, 0, sizeof(DEVMODE));
        This->render.mode.dmSize = sizeof(DEVMODE);
        This->render.mode.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT;
        This->render.mode.dmPelsWidth = This->render.width;
        This->render.mode.dmPelsHeight = This->render.height;
        if(This->render.bpp)
        {
            This->render.mode.dmFields |= DM_BITSPERPEL;
            This->render.mode.dmBitsPerPel = This->render.bpp;
        }

        if(!This->devmode && ChangeDisplaySettings(&This->render.mode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            This->render.run = FALSE;
            return DDERR_INVALIDMODE;
        }
    }

    This->render.thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)render_main, NULL, 0, NULL);

    return DD_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_SYSCOMMAND:
            if(wParam == SC_CLOSE)
            {
                exit(0);
            }
            break;
        case WM_SIZE:
            if(wParam == SIZE_RESTORED)
            {
                ddraw_SetDisplayMode(ddraw, ddraw->width, ddraw->height, ddraw->bpp);
            }
            if(wParam == SIZE_MINIMIZED)
            {
                ddraw_RestoreDisplayMode(ddraw);
            }
            break;
        case WM_NCACTIVATE:
            /* game's drawing loop stops when it inactivates, so don't */
            DefWindowProc(hWnd, uMsg, wParam, lParam);

            if(wParam == FALSE)
            {
                mouse_unlock();
            }

            if(!ddraw->windowed)
            {
                if(wParam == FALSE)
                {
                    ShowWindow(ddraw->hWnd, SW_MINIMIZE);
                }
                else
                {
                    ShowWindow(ddraw->hWnd, SW_RESTORE);
                }
            }

            /* fall trough */
        case WM_WINDOWPOSCHANGED:
            GetClientRect(ddraw->hWnd, &ddraw->cursorclip);

            POINT pt = { ddraw->cursorclip.left, ddraw->cursorclip.top };
            POINT pt2 = { ddraw->cursorclip.right, ddraw->cursorclip.bottom };
            ClientToScreen(ddraw->hWnd, &pt);
            ClientToScreen(ddraw->hWnd, &pt2);
            SetRect(&ddraw->cursorclip, pt.x, pt.y, pt2.x, pt2.y);

            ddraw->center.x = ddraw->cursorclip.left + ( (ddraw->cursorclip.right - ddraw->cursorclip.left) / 2);
            ddraw->center.y = ddraw->cursorclip.top + ( (ddraw->cursorclip.bottom - ddraw->cursorclip.top) / 2);

            DefWindowProc(hWnd, uMsg, wParam, lParam);
            return 0;
        case WM_KEYDOWN:
            if(wParam == VK_CONTROL || wParam == VK_TAB)
            {
                if(GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState(VK_TAB) & 0x8000)
                {
                    mouse_unlock();
                }
            }
            break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            if(!ddraw->locked)
            {
                mouse_lock();
                return 0;
            }
            lParam = MAKELPARAM(ddraw->cursor.x, ddraw->cursor.y);
            break;
        case WM_MOUSEMOVE:
            if(ddraw->locked)
            {
                if(LOWORD(lParam) != ddraw->render.width / 2 || HIWORD(lParam) != ddraw->render.height / 2)
                {
                    if(ddraw->adjmouse)
                    {
                        ddraw->cursor.x += (LOWORD(lParam) - ddraw->render.width / 2) * ((float)ddraw->width / ddraw->render.width);
                        ddraw->cursor.y += (HIWORD(lParam) - ddraw->render.height / 2) * ((float)ddraw->height / ddraw->render.height);
                    }
                    else
                    {
                        ddraw->cursor.x += LOWORD(lParam) - ddraw->render.width / 2;
                        ddraw->cursor.y += HIWORD(lParam) - ddraw->render.height / 2;
                    }

                    if(ddraw->cursor.x < 0) ddraw->cursor.x = 0;
                    if(ddraw->cursor.y < 0) ddraw->cursor.y = 0;
                    if(ddraw->cursor.x > ddraw->width-1) ddraw->cursor.x = ddraw->width-1;
                    if(ddraw->cursor.y > ddraw->height-1) ddraw->cursor.y = ddraw->height-1;

                    SetCursorPos(ddraw->center.x, ddraw->center.y);
                }
            }
            break;
        case WM_MOVE:
            ddraw->winpos.x = LOWORD(lParam);
            ddraw->winpos.y = HIWORD(lParam);
            if(ddraw->winpos.x < 0)
            {
                ddraw->winpos.x = 0;
            }
            if(ddraw->winpos.y < 0)
            {
                ddraw->winpos.y = 0;
            }
            break;
    }

    return ddraw->WndProc(hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall ddraw_SetCooperativeLevel(IDirectDrawImpl *This, HWND hWnd, DWORD dwFlags)
{
    PIXELFORMATDESCRIPTOR pfd;

    printf("DirectDraw::SetCooperativeLevel(This=%p, hWnd=0x%08X, dwFlags=0x%08X)\n", This, (unsigned int)hWnd, (unsigned int)dwFlags);

    /* Red Alert for some weird reason does this on Windows XP */
    if(hWnd == NULL)
    {
        return DDERR_INVALIDPARAMS;
    }

    This->hWnd = hWnd;

    mouse_init(hWnd);

    This->WndProc = (LRESULT CALLBACK (*)(HWND, UINT, WPARAM, LPARAM))GetWindowLong(This->hWnd, GWL_WNDPROC);
    if(!This->devmode)
    {
        SetWindowLong(This->hWnd, GWL_WNDPROC, (LONG)WndProc);
    }

    if(!This->render.hDC)
    {
        This->render.hDC = GetDC(This->hWnd);

        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = ddraw->render.bpp ? ddraw->render.bpp : ddraw->mode.dmBitsPerPel;
        pfd.iLayerType = PFD_MAIN_PLANE;
        SetPixelFormat( This->render.hDC, ChoosePixelFormat( This->render.hDC, &pfd ), &pfd );
    }

    return DD_OK;
}

HRESULT __stdcall ddraw_WaitForVerticalBlank(IDirectDrawImpl *This, DWORD a, HANDLE b)
{
#if _DEBUG
    printf("DirectDraw::WaitForVerticalBlank(This=%p, ...)\n", This);
#endif
    WaitForSingleObject(This->render.ev, INFINITE);
    return DD_OK;
}

HRESULT __stdcall ddraw_QueryInterface(IDirectDrawImpl *This, REFIID riid, void **obj)
{
    printf("DirectDraw::QueryInterface(This=%p, riid=%08X, obj=%p)\n", This, (unsigned int)riid, obj);
    return S_OK;
}

ULONG __stdcall ddraw_AddRef(IDirectDrawImpl *This)
{
    printf("DirectDraw::AddRef(This=%p)\n", This);

    This->Ref++;

    return This->Ref;
}

ULONG __stdcall ddraw_Release(IDirectDrawImpl *This)
{
    printf("DirectDraw::Release(This=%p)\n", This);

    This->Ref--;

    if(This->Ref == 0)
    {
        if(This->render.run)
        {
            EnterCriticalSection(&This->cs);

            This->render.run = FALSE;
            WaitForSingleObject(This->render.thread, INFINITE);
            This->render.thread = NULL;

            LeaveCriticalSection(&This->cs);
        }

        if(This->render.hDC)
        {
            ReleaseDC(This->hWnd, This->render.hDC);
            This->render.hDC = NULL;
        }

        if(This->render.ev)
        {
            CloseHandle(This->render.ev);
            ddraw->render.ev = NULL;
        }

        DeleteCriticalSection(&This->cs);

        /* restore old wndproc, subsequent ddraw creation will otherwise fail */
        SetWindowLong(This->hWnd, GWL_WNDPROC, (LONG)This->WndProc);
        //free(This);
        ddraw = NULL;
        return 0;
    }

    return This->Ref;
}

struct IDirectDrawImplVtbl iface =
{
    /* IUnknown */
    ddraw_QueryInterface,
    ddraw_AddRef,
    ddraw_Release,
    /* IDirectDrawImpl */
    ddraw_Compact,
    ddraw_CreateClipper,
    ddraw_CreatePalette,
    ddraw_CreateSurface,
    ddraw_DuplicateSurface,
    ddraw_EnumDisplayModes,
    ddraw_EnumSurfaces,
    ddraw_FlipToGDISurface,
    ddraw_GetCaps,
    ddraw_GetDisplayMode,
    ddraw_GetFourCCCodes,
    ddraw_GetGDISurface,
    ddraw_GetMonitorFrequency,
    ddraw_GetScanLine,
    ddraw_GetVerticalBlankStatus,
    ddraw_Initialize,
    ddraw_RestoreDisplayMode,
    ddraw_SetCooperativeLevel,
    ddraw_SetDisplayMode,
    ddraw_WaitForVerticalBlank
};

int stdout_open = 0;
HRESULT WINAPI DirectDrawCreate(GUID FAR* lpGUID, LPDIRECTDRAW FAR* lplpDD, IUnknown FAR* pUnkOuter) 
{
#if _DEBUG
    if(!stdout_open)
    {
        freopen("stdout.txt", "w", stdout);
        setvbuf(stdout, NULL, _IONBF, 0);
        stdout_open = 1;
    }
#endif

    printf("DirectDrawCreate(lpGUID=%p, lplpDD=%p, pUnkOuter=%p)\n", lpGUID, lplpDD, pUnkOuter);

    if(ddraw)
    {
        /* FIXME: check the calling module before passing the call! */
        return ddraw->DirectDrawCreate(lpGUID, lplpDD, pUnkOuter);

        /*
        printf(" returning DDERR_DIRECTDRAWALREADYCREATED\n");
        return DDERR_DIRECTDRAWALREADYCREATED;
        */
    } 

    HMODULE real_dll = LoadLibrary("system32\\ddraw.dll");
    if(!real_dll)
    {
        return DDERR_GENERIC;
    }

    IDirectDrawImpl *This = (IDirectDrawImpl *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(IDirectDrawImpl));
    This->lpVtbl = &iface;
    printf(" This = %p\n", This);
    *lplpDD = (LPDIRECTDRAW)This;
    ddraw = This;

    This->DirectDrawCreate = (HRESULT WINAPI (*)(GUID FAR*, LPDIRECTDRAW FAR*, IUnknown FAR*))GetProcAddress(real_dll, "DirectDrawCreate");

    CloseHandle(real_dll);

    if(!This->DirectDrawCreate)
    {
        ddraw_Release(This);
        return DDERR_GENERIC;
    }

    InitializeCriticalSection(&This->cs);
    This->render.ev = CreateEvent(NULL, TRUE, FALSE, NULL);

    /* load configuration options from ddraw.ini */
    char cwd[MAX_PATH];
    char ini_path[MAX_PATH];
    char tmp[256];
    GetCurrentDirectoryA(sizeof(cwd), cwd);
    snprintf(ini_path, sizeof(ini_path), "%s\\ddraw.ini", cwd);

    if(GetFileAttributes(ini_path) == 0xFFFFFFFF)
    {
        FILE *fh = fopen(ini_path, "w");
        fputs(
            "[ddraw]\n"
            "width=640\n"
            "height=400\n"
            "; bits per pixel, possible values: 16, 24 and 32, 0 = auto\n"
            "bpp=0\n"
            "windowed=true\n"
            "; real rendering rate, -1 = vsync, 0 = unlimited, n = cap\n"
            "maxfps=120\n"
            "; scaling filter, nearest = sharp, linear = smooth\n"
            "filter=nearest\n"
            "; mouse sensitivity scaling\n"
            "adjmouse=false\n"
            "; enable C&C/RA mouse hack\n"
            "mhack=true\n"
        , fh);
        fclose(fh);
    }

    GetPrivateProfileStringA("ddraw", "windowed", "TRUE", tmp, sizeof(tmp), ini_path);
    if(tolower(tmp[0]) == 'n' || tolower(tmp[0]) == 'f' || tmp[0] == '0')
    {
        This->windowed = FALSE;
    }
    else
    {
        This->windowed = TRUE;
    }

    This->render.maxfps = GetPrivateProfileIntA("ddraw", "maxfps", 120, ini_path);
    This->render.width = GetPrivateProfileIntA("ddraw", "width", 640, ini_path);
    if(This->render.width < 640)
    {
        This->render.width = 640;
    }

    This->render.height = GetPrivateProfileIntA("ddraw", "height", 400, ini_path);
    if(This->render.height < 400)
    {
        This->render.height = 400;
    }
    This->render.bpp = GetPrivateProfileIntA("ddraw", "bpp", 32, ini_path);
    if(This->render.bpp != 16 && This->render.bpp != 24 && This->render.bpp != 32)
    {
        This->render.bpp = 0;
    }

    GetPrivateProfileStringA("ddraw", "filter", tmp, tmp, sizeof(tmp), ini_path);
    if(tolower(tmp[0]) == 'l' || tolower(tmp[3]) == 'l')
    {
        This->render.filter = 1;
    }
    else
    {
        This->render.filter = 0;
    }

    GetPrivateProfileStringA("ddraw", "adjmouse", "FALSE", tmp, sizeof(tmp), ini_path);
    if(tolower(tmp[0]) == 'y' || tolower(tmp[0]) == 't' || tmp[0] == '1')
    {
        This->adjmouse = TRUE;
    }
    else
    {
        This->adjmouse = FALSE;
    }

    GetPrivateProfileStringA("ddraw", "mhack", "TRUE", tmp, sizeof(tmp), ini_path);
    if(tolower(tmp[0]) == 'y' || tolower(tmp[0]) == 't' || tmp[0] == '1')
    {
        This->mhack = TRUE;
    }
    else
    {
        This->mhack = FALSE;
    }

    GetPrivateProfileStringA("ddraw", "devmode", "FALSE", tmp, sizeof(tmp), ini_path);
    if(tolower(tmp[0]) == 'y' || tolower(tmp[0]) == 't' || tmp[0] == '1')
    {
        This->devmode = TRUE;
        This->mhack = FALSE;
    }
    else
    {
        This->devmode = FALSE;
    }

    This->Ref = 0;
    ddraw_AddRef(This);

    return DD_OK;
}
