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

extern struct render render_ddraw;
extern struct render render_opengl;

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
    return ddraw->render->RestoreDisplayMode();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_NCACTIVATE:
            /* game's drawing loop stops when it inactivates, so don't */
            if(ddraw->windowed)
            {
                DefWindowProc(hWnd, uMsg, wParam, lParam);
                if(wParam == FALSE)
                {
                    mouse_unlock();
                }
                else
                {
                    mouse_lock();
                }
                SetCursor(LoadCursor((HINSTANCE)GetWindowLong(ddraw->hWnd, GWL_HINSTANCE), IDC_ARROW));
                return 0;
            }
            break;
        case WM_KEYDOWN:
            if(ddraw->windowed)
            {
                if(wParam == VK_CONTROL)
                {
                    ddraw->key_ctrl = TRUE;
                }
                if(wParam == VK_TAB)
                {
                    ddraw->key_tab = TRUE;
                }
                if(ddraw->key_tab && ddraw->key_ctrl)
                {
                    mouse_unlock();
                }
            }
            break;
        case WM_KEYUP:
            if(ddraw->windowed)
            {
                if(wParam == VK_CONTROL)
                {
                    ddraw->key_ctrl = FALSE;
                }
                if(wParam == VK_TAB)
                {
                    ddraw->key_tab = FALSE;
                }
            }
            break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
            if(!ddraw->locked && ddraw->windowed)
            {
                mouse_lock();
                return 0;
            }
            lParam = MAKELPARAM(ddraw->cursor.x, ddraw->cursor.y);
            break;
        case WM_MOUSEMOVE:
            if(ddraw->locked)
            {
                if(LOWORD(lParam) != ddraw->render->width / 2 || HIWORD(lParam) != ddraw->render->height / 2)
                {
                    if(ddraw->adjmouse)
                    {
                        ddraw->cursor.x += (LOWORD(lParam) - ddraw->render->width / 2) * ((float)ddraw->width / ddraw->render->width);
                        ddraw->cursor.y += (HIWORD(lParam) - ddraw->render->height / 2) * ((float)ddraw->height / ddraw->render->height);
                    }
                    else
                    {
                        ddraw->cursor.x += LOWORD(lParam) - ddraw->render->width / 2;
                        ddraw->cursor.y += HIWORD(lParam) - ddraw->render->height / 2;
                    }

                    if(ddraw->cursor.x < 0) ddraw->cursor.x = 0;
                    if(ddraw->cursor.y < 0) ddraw->cursor.y = 0;
                    if(ddraw->cursor.x > ddraw->width) ddraw->cursor.x = ddraw->width;
                    if(ddraw->cursor.y > ddraw->height) ddraw->cursor.y = ddraw->height;

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
        case WM_WINDOWPOSCHANGED:
            GetClientRect(ddraw->hWnd, &ddraw->cursorclip);

            POINT pt = { ddraw->cursorclip.left, ddraw->cursorclip.top };
            POINT pt2 = { ddraw->cursorclip.right, ddraw->cursorclip.bottom };
            ClientToScreen(ddraw->hWnd, &pt);
            ClientToScreen(ddraw->hWnd, &pt2);
            SetRect(&ddraw->cursorclip, pt.x, pt.y, pt2.x, pt2.y);

            ddraw->center.x = ddraw->cursorclip.left + ( (ddraw->cursorclip.right - ddraw->cursorclip.left) / 2);
            ddraw->center.y = ddraw->cursorclip.top + ( (ddraw->cursorclip.bottom - ddraw->cursorclip.top) / 2);

            /* our own SendMessage only triggers this */
            if(lParam == 0)
            {
                return 0;
            }

            break;
    }

    return ddraw->WndProc(hWnd, uMsg, wParam, lParam);
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

    mouse_init(hWnd);

    This->WndProc = (LRESULT CALLBACK (*)(HWND, UINT, WPARAM, LPARAM))GetWindowLong(This->hWnd, GWL_WNDPROC);
    SetWindowLong(This->hWnd, GWL_WNDPROC, (LONG)WndProc);

    return DD_OK;
}

HRESULT __stdcall ddraw_SetDisplayMode(IDirectDrawImpl *This, DWORD width, DWORD height, DWORD bpp)
{
    DEVMODE mode;

    printf("DirectDraw::SetDisplayMode(This=%p, width=%d, height=%d, bpp=%d)\n", This, (unsigned int)width, (unsigned int)height, (unsigned int)bpp);

    /* currently we only support 8 bit modes */
    if(bpp != 8)
    {
        return DDERR_INVALIDMODE;
    }

    mode.dmSize = sizeof(DEVMODE);
    mode.dmDriverExtra = 0;

    if(EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &mode) == FALSE)
    {
        /* not expected */
        return DDERR_UNSUPPORTED;
    }

    This->width = width;
    This->height = height;
    This->bpp = bpp;
    This->freq = mode.dmDisplayFrequency;

    if(This->render->width < This->width)
    {
        This->render->width = This->width;
    }
    if(This->render->height < This->height)
    {
        This->render->height = This->height;
    }

    mouse_unlock();
    if(!This->windowed)
    {
        mouse_lock();
    }

    if(This->windowed)
    {
        SetWindowLong(This->hWnd, GWL_STYLE, GetWindowLong(This->hWnd, GWL_STYLE) | WS_CAPTION | WS_BORDER);

        /* center the window with correct dimensions */
        int x = (mode.dmPelsWidth / 2) - (This->render->width / 2);
        int y = (mode.dmPelsHeight / 2) - (This->render->height / 2);
        RECT dst = { x, y, This->render->width+x, This->render->height+y };
        AdjustWindowRect(&dst, GetWindowLong(This->hWnd, GWL_STYLE), FALSE);
        SetWindowPos(This->hWnd, HWND_TOPMOST, dst.left, dst.top, (dst.right - dst.left), (dst.bottom - dst.top), SWP_SHOWWINDOW);
    }
    else
    {
        SetWindowPos(This->hWnd, HWND_TOPMOST, 0, 0, This->render->width, This->render->height, SWP_SHOWWINDOW);
        SendMessage(This->hWnd, WM_WINDOWPOSCHANGED, 0, 0);
    }

    return ddraw->render->SetDisplayMode(width, height);
}

HRESULT __stdcall ddraw_WaitForVerticalBlank(IDirectDrawImpl *This, DWORD a, HANDLE b)
{
#if _DEBUG
    printf("DirectDraw::WaitForVerticalBlank(This=%p, ...)\n", This);
#endif
    WaitForSingleObject(ddraw->render->ev, INFINITE);
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
        free(This);
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

    /* load configuration options from ddraw.ini */
    char cwd[MAX_PATH];
    char ini_path[MAX_PATH];
    char tmp[256];
    GetCurrentDirectoryA(sizeof(cwd), cwd);
    snprintf(ini_path, sizeof(ini_path), "%s\\ddraw.ini", cwd);

    GetPrivateProfileStringA("ddraw", "renderer", "ddraw", tmp, sizeof(tmp), ini_path);
    if(tolower(tmp[0]) == 'o')
    {
        This->render = &render_opengl;
    }
    else
    {
        This->render = &render_ddraw;
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

    This->render->maxfps = GetPrivateProfileIntA("ddraw", "maxfps", -1, ini_path);
    This->render->width = GetPrivateProfileIntA("ddraw", "width", 640, ini_path);
    if(This->render->width < 640)
    {
        This->render->width = 640;
    }

    This->render->height = GetPrivateProfileIntA("ddraw", "height", 400, ini_path);
    if(This->render->height < 400)
    {
        This->render->height = 400;
    }
    This->render->bpp = GetPrivateProfileIntA("ddraw", "bpp", 24, ini_path);
    if(This->render->bpp != 16 && This->render->bpp != 24 && This->render->bpp != 32)
    {
        This->render->bpp = 24;
    }

    GetPrivateProfileStringA("ddraw", "filter", tmp, tmp, sizeof(tmp), ini_path);
    if(tolower(tmp[0]) == 'l' || tolower(tmp[3]) == 'l')
    {
        This->render->filter = 1;
    }
    else
    {
        This->render->filter = 0;
    }
    GetPrivateProfileStringA("ddraw", "adjmouse", "TRUE", tmp, sizeof(tmp), ini_path);
    if(tolower(tmp[0]) == 'y' || tolower(tmp[0]) == 't' || tmp[0] == '1')
    {
        This->adjmouse = TRUE;
    }
    else
    {
        This->adjmouse = FALSE;
    }

    This->render->Initialize();

    This->Ref = 0;
    ddraw_AddRef(This);

    return DD_OK;
}
