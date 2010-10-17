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

#ifndef MAIN_H 
#define MAIN_H

#include <windows.h>
#include "ddraw.h"
#include <GL/gl.h>
#include <GL/glu.h>

typedef struct
{
    /* IUnknown */
    HRESULT (*QueryInterface)(void *, REFIID, void **);
    ULONG (*AddRef)(void *);
    ULONG (*Release)(void *);

    /* IDirectDraw */
    HRESULT (*Compact)(void *);
    HRESULT (*CreateClipper)(void *);
    HRESULT (*CreatePalette)(void *, DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE FAR *, IUnknown FAR *);
    HRESULT (*CreateSurface)(void *, LPDDSURFACEDESC, LPDIRECTDRAWSURFACE FAR *, IUnknown FAR *);
    HRESULT (*DuplicateSurface)(void *);
    HRESULT (*EnumDisplayModes)(void *);
    HRESULT (*EnumSurfaces)(void *);
    HRESULT (*FlipToGDISurface)(void *);
    HRESULT (*GetCaps)(void *, LPDDCAPS, LPDDCAPS);
    HRESULT (*GetDisplayMode)(void *);
    HRESULT (*GetFourCCCodes)(void *);
    HRESULT (*GetGDISurface)(void *);
    HRESULT (*GetMonitorFrequency)(void *);
    HRESULT (*SetScanLine)(void *);
    HRESULT (*GetVerticalBlankStatus)(void *);
    HRESULT (*Initialize)(void *);
    HRESULT (*RestoreDisplayMode)(void *);
    HRESULT (*SetCooperativeLevel)(void *, HWND, DWORD);
    HRESULT (*SetDisplayMode)(void *, DWORD, DWORD, DWORD);
    HRESULT (*WaitForVerticalBlank)(void *);
} fakeDirectDraw;

typedef struct
{
    fakeDirectDraw *Functions;

    ULONG Ref;

    DWORD width;
    DWORD height;
    DWORD bpp;

    HWND hWnd;

} fakeDirectDrawObject;

#endif
