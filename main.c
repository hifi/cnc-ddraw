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

#include "palette.h"
#include "surface.h"

typedef struct
{
    /* IUnknown */
    HRESULT (*QueryInterface)(void *, REFIID, void **);
    ULONG (*AddRef)(void *);
    ULONG (*Release)(void *);

    /* IDirectDraw */
    HRESULT (*Compact)(void *);
    HRESULT (*CreateClipper)(void *);
    HRESULT (*CreatePalette)(void *, LPPALETTEENTRY, LPDIRECTDRAWPALETTE FAR *, IUnknown FAR *);
    HRESULT (*CreateSurface)(void *, LPDDSURFACEDESC, LPDIRECTDRAWSURFACE FAR *, IUnknown FAR *);
    HRESULT (*DuplicateSurface)(void *);
    HRESULT (*EnumDisplayModes)(void *);
    HRESULT (*EnumSurfaces)(void *);
    HRESULT (*FlipToGDISurface)(void *);
    HRESULT (*GetCaps)(void *);
    HRESULT (*GetDisplayMode)(void *);
    HRESULT (*GetFourCCCodes)(void *);
    HRESULT (*GetGDISurface)(void *);
    HRESULT (*GetMonitorFrequency)(void *);
    HRESULT (*SetScanLine)(void *);
    HRESULT (*GetVerticalBlankStatus)(void *);
    HRESULT (*Initialize)(void *);
    HRESULT (*RestoreDisplayMode)(void *);
    HRESULT (*SetCooperativeLevel)(void *, HWND, DWORD);
    HRESULT (*SetDisplayMode)(void *, DWORD, DWORD, DWORD, DWORD, DWORD);
    HRESULT (*WaitForVerticalBlank)(void *);
} fakeDirectDraw;

typedef struct
{
    fakeDirectDraw *Functions;

    ULONG Ref;

} fakeDirectDrawObject;

HRESULT ddraw_CreatePalette(void *This, LPPALETTEENTRY DDColorArray, LPDIRECTDRAWPALETTE FAR * DDPalette, IUnknown FAR * unkOuter)
{
    printf("DirectDraw::CreatePalette(This=%p, DDColorArray=%p, DDPalette=%p, unkOuter=%p)\n", This, DDColorArray, DDPalette, unkOuter);

    fakeDirectDrawPaletteObject *Palette = (fakeDirectDrawPaletteObject *)malloc(sizeof(fakeDirectDrawPaletteObject));
    Palette->Ref = 1;
    Palette->Functions = &piface;
    printf(" Palette = %p\n", Palette);
    *DDPalette = (LPDIRECTDRAWPALETTE)Palette;

    return DD_OK;
}

HRESULT ddraw_SetCooperativeLevel(void *This, HWND hWnd, DWORD dwFlags)
{
    printf("DirectDraw::SetCooperativeLevel(This=%p, hWnd=0x%08X, dwFlags=0x%08X)\n", This, (unsigned int)hWnd, (unsigned int)dwFlags);
    return DD_OK;
}

HRESULT ddraw_SetDisplayMode(void *This, DWORD width, DWORD height, DWORD bpp, DWORD refreshRate, DWORD flags)
{
    printf("DirectDraw::SetDisplayMode(This=%p, width=%d, height=%d, bpp=%d, refreshRate=%d, flags=0x%08X)\n",
            This, (unsigned int)width, (unsigned int)height, (unsigned int)bpp, (unsigned int)refreshRate, (unsigned int)flags);
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
    null, //GetCaps,
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

HRESULT WINAPI DirectDrawCreate(GUID FAR* lpGUID, LPDIRECTDRAW FAR* lplpDD, IUnknown FAR* pUnkOuter) 
{
    printf("DirectDrawCreate(lpGUID=%p, lplpDD=%p, pUnkOuter=%p)\n", lpGUID, lplpDD, pUnkOuter);

    fakeDirectDrawObject *This = (fakeDirectDrawObject *)malloc(sizeof(fakeDirectDrawObject));
    This->Ref = 1;
    This->Functions = &iface;
    printf(" This = %p\n", This);
    *lplpDD = (LPDIRECTDRAW)This;

    return DD_OK;
}
