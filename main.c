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

void hacks();

int SDL_Main(IDirectDrawImpl *ddraw);

IDirectDrawImpl *ddraw = NULL;

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

    return DD_OK;
}

HRESULT __stdcall ddraw_SetDisplayMode(IDirectDrawImpl *This, DWORD width, DWORD height, DWORD bpp)
{
    printf("DirectDraw::SetDisplayMode(This=%p, width=%d, height=%d, bpp=%d)\n", This, (unsigned int)width, (unsigned int)height, (unsigned int)bpp);

    This->width = width;
    This->height = height;
    This->bpp = bpp;

    if (SDL_VideoModeOK(width, height, bpp, SDL_HWSURFACE|SDL_HWPALETTE|SDL_FULLSCREEN))
    {
        This->running = TRUE;
        This->thread = SDL_CreateThread((int (*)(void *))SDL_Main, This);
        return DD_OK;
    }

    return DDERR_INVALIDMODE;
}

LRESULT CALLBACK NULL_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_DESTROY:
            return ddraw->WndProc(hWnd, uMsg, wParam, lParam);
        case 1129: /* this somehow triggers network activity in C&C in WCHAT mode */
        case 1139: /* this somehow triggers network activity in RA, investigate */
        case 2024: /* this somehow allows RA edwin to work, investigate */
            return ddraw->WndProc(hWnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall ddraw_SetCooperativeLevel(IDirectDrawImpl *This, HWND hWnd, DWORD dwFlags)
{
    printf("DirectDraw::SetCooperativeLevel(This=%p, hWnd=0x%08X, dwFlags=0x%08X)\n", This, (unsigned int)hWnd, (unsigned int)dwFlags);

    /* Red Alert for some weird reason does this on Windows XP */
    if(hWnd == NULL)
    {
        return DDERR_INVALIDPARAMS;
    }

    This->hWnd = hWnd;

    This->WndProc = (LRESULT CALLBACK (*)(HWND, UINT, WPARAM, LPARAM))GetWindowLong(This->hWnd, GWL_WNDPROC);
    SetWindowLong(This->hWnd, GWL_WNDPROC, (LONG)NULL_WndProc);
    ShowWindow(hWnd, SW_HIDE);

    return DD_OK;
}

HRESULT __stdcall ddraw_WaitForVerticalBlank(IDirectDrawImpl *This, DWORD a, HANDLE b)
{
#if _DEBUG
    printf("DirectDraw::WaitForVerticalBlank(This=%p, ...)\n", This);
#endif
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
        if (ddraw->running)
        {
            SDL_Event ev;
            ev.type = SDL_USEREVENT;
            ddraw->running = FALSE;
            SDL_PushEvent(&ev);
            SDL_WaitThread(ddraw->thread, NULL);
        }

        SDL_Quit();

        if(This->real_dll)
        {
            FreeLibrary(This->real_dll);
        }

        /* restore old wndproc, subsequent ddraw creation will otherwise fail */
        SetWindowLong(This->hWnd, GWL_WNDPROC, (LONG)This->WndProc);
        HeapFree(GetProcessHeap(), 0, This);
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
#if 0
    if(!stdout_open)
    {
        freopen("stdout.txt", "w", stdout);
        setvbuf(stdout, NULL, _IONBF, 0);
        stdout_open = 1;
    }
#endif

    printf("DirectDrawCreate(lpGUID=%p, lplpDD=%p, pUnkOuter=%p)\n", lpGUID, lplpDD, pUnkOuter);

    hacks();

    if(ddraw)
    {
        /* FIXME: check the calling module before passing the call! */
        return ddraw->DirectDrawCreate(lpGUID, lplpDD, pUnkOuter);
    } 

    IDirectDrawImpl *This = (IDirectDrawImpl *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(IDirectDrawImpl));
    This->lpVtbl = &iface;
    printf(" This = %p\n", This);
    *lplpDD = (LPDIRECTDRAW)This;
    This->Ref = 0;
    ddraw_AddRef(This);

    ddraw = This;
    ddraw->event = 0;

    This->real_dll = LoadLibrary("system32\\ddraw.dll");
    if(!This->real_dll)
    {
        ddraw_Release(This);
        return DDERR_GENERIC;
    }

    This->DirectDrawCreate = (HRESULT WINAPI (*)(GUID FAR*, LPDIRECTDRAW FAR*, IUnknown FAR*))GetProcAddress(This->real_dll, "DirectDrawCreate");

    if(!This->DirectDrawCreate)
    {
        ddraw_Release(This);
        return DDERR_GENERIC;
    }

    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);

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
            "windowed=true\n"
            "maxfps=0\n"
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

    return DD_OK;
}
