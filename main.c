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
#include "ddraw.h"

#include "main.h"
#include "palette.h"
#include "surface.h"

HRESULT ddraw_GetCaps(void *This, LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDEmulCaps)
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

HRESULT ddraw_SetCooperativeLevel(void *_This, HWND hWnd, DWORD dwFlags)
{
    fakeDirectDrawObject *This = (fakeDirectDrawObject *)_This;
    printf("DirectDraw::SetCooperativeLevel(This=%p, hWnd=0x%08X, dwFlags=0x%08X)\n", This, (unsigned int)hWnd, (unsigned int)dwFlags);

    This->hWnd = hWnd;

    return DD_OK;
}

HRESULT ddraw_SetDisplayMode(void *_This, DWORD width, DWORD height, DWORD bpp)
{
    fakeDirectDrawObject *This = (fakeDirectDrawObject *)_This;

    printf("DirectDraw::SetDisplayMode(This=%p, width=%d, height=%d, bpp=%d)\n", This, (unsigned int)width, (unsigned int)height, (unsigned int)bpp);

    This->width = width;
    This->height = height;
    This->bpp = bpp;

    MoveWindow(This->hWnd, 0, 0, This->width, This->height, TRUE);
    SetWindowLong(This->hWnd, GWL_STYLE, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE);

    return DD_OK;
}

HRESULT QueryInterface(void *This, REFIID riid, void **obj)
{
    printf("QueryInterface(This=%p, riid=%08X, obj=%p)\n", This, (unsigned int)riid, obj);
    return S_OK;
}

ULONG AddRef(void *This)
{
    printf("AddRef(This=%p)\n", This);
    ((fakeDirectDrawObject *)This)->Ref++;
    return ((fakeDirectDrawObject *)This)->Ref;
}

ULONG Release(void *This)
{
    printf("Release(This=%p)\n", This);
    ((fakeDirectDrawObject *)This)->Ref--;

    if(((fakeDirectDrawObject *)This)->Ref == 0)
    {
        free(This);
        return 0;
    }

    return ((fakeDirectDrawObject *)This)->Ref;
}

HRESULT null(void *This)
{
    printf("Warning: null method called for instance %p!\n", This);
    return DD_OK;
}

fakeDirectDraw iface =
{
    /* IUnknown */
    QueryInterface,
    AddRef,
    Release,
    /* IDirectDraw */
    null, //Compact,
    null, //CreateClipper,
    ddraw_CreatePalette,
    ddraw_CreateSurface,
    null, //DuplicateSurface,
    null, //EnumDisplayModes,
    null, //EnumSurfaces,
    null, //FlipToGDISurface,
    ddraw_GetCaps,
    null, //GetDisplayMode,
    null, //GetFourCCCodes,
    null, //GetGDISurface,
    null, //GetMonitorFrequency,
    null, //GetScanLine,
    null, //GetVerticalBlankStatus,
    null, //Initialize,
    null, //RestoreDisplayMode,
    ddraw_SetCooperativeLevel,
    ddraw_SetDisplayMode,
    null  //WaitForVerticalBlank
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HRESULT WINAPI DirectDrawCreate(GUID FAR* lpGUID, LPDIRECTDRAW FAR* lplpDD, IUnknown FAR* pUnkOuter) 
{
    printf("DirectDrawCreate(lpGUID=%p, lplpDD=%p, pUnkOuter=%p)\n", lpGUID, lplpDD, pUnkOuter);

    fakeDirectDrawObject *This = (fakeDirectDrawObject *)malloc(sizeof(fakeDirectDrawObject));
    This->Ref = 1;
    This->Functions = &iface;
    This->hWnd = NULL;
    printf(" This = %p\n", This);
    *lplpDD = (LPDIRECTDRAW)This;

    return DD_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CLOSE:
            PostQuitMessage( 0 );
        case WM_CREATE:
        case WM_DESTROY:
        case WM_KEYDOWN:
            return 0;
        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }
}
