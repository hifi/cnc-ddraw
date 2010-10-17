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

#ifndef SURFACE_H
#define SURFACE_H

#include <windows.h>
#include "ddraw.h"
#include "palette.h"

HRESULT ddraw_CreateSurface(void *This, LPDDSURFACEDESC DDSurfaceDesc, LPDIRECTDRAWSURFACE FAR *DDSurface, IUnknown FAR * unkOuter);

typedef struct
{
    /* IUnknown */
    HRESULT (*QueryInterface)(void *, REFIID, void **);
    ULONG (*AddRef)(void *);
    ULONG (*Release)(void *);

    /* IDirectDrawSurface */
    HRESULT (*AddAttachedSurface)(void *, LPDIRECTDRAWSURFACE);
    HRESULT (*AddOverlayDirtyRect)(void *, LPRECT);
    HRESULT (*Blt)(void *, LPRECT,LPDIRECTDRAWSURFACE, LPRECT,DWORD, LPDDBLTFX);
    HRESULT (*BltBatch)(void *, LPDDBLTBATCH, DWORD, DWORD );
    HRESULT (*BltFast)(void *, DWORD,DWORD,LPDIRECTDRAWSURFACE, LPRECT,DWORD);
    HRESULT (*DeleteAttachedSurface)(void *, DWORD,LPDIRECTDRAWSURFACE);
    HRESULT (*EnumAttachedSurfaces)(void *, LPVOID,LPDDENUMSURFACESCALLBACK);
    HRESULT (*EnumOverlayZOrders)(void *, DWORD,LPVOID,LPDDENUMSURFACESCALLBACK);
    HRESULT (*Flip)(void *, LPDIRECTDRAWSURFACE, DWORD);
    HRESULT (*GetAttachedSurface)(void *, LPDDSCAPS, LPDIRECTDRAWSURFACE FAR *);
    HRESULT (*GetBltStatus)(void *, DWORD);
    HRESULT (*GetCaps)(void *, LPDDSCAPS);
    HRESULT (*GetClipper)(void *, LPDIRECTDRAWCLIPPER FAR*);
    HRESULT (*GetColorKey)(void *, DWORD, LPDDCOLORKEY);
    HRESULT (*GetDC)(void *, HDC FAR *);
    HRESULT (*GetFlipStatus)(void *, DWORD);
    HRESULT (*GetOverlayPosition)(void *, LPLONG, LPLONG );
    HRESULT (*GetPalette)(void *, LPDIRECTDRAWPALETTE FAR*);
    HRESULT (*GetPixelFormat)(void *, LPDDPIXELFORMAT);
    HRESULT (*GetSurfaceDesc)(void *, LPDDSURFACEDESC);
    HRESULT (*Initialize)(void *, LPDIRECTDRAW, LPDDSURFACEDESC);
    HRESULT (*IsLost)(void *);
    HRESULT (*Lock)(void *, LPRECT,LPDDSURFACEDESC,DWORD,HANDLE);
    HRESULT (*ReleaseDC)(void *, HDC);
    HRESULT (*Restore)(void *);
    HRESULT (*SetClipper)(void *, LPDIRECTDRAWCLIPPER);
    HRESULT (*SetColorKey)(void *, DWORD, LPDDCOLORKEY);
    HRESULT (*SetOverlayPosition)(void *, LONG, LONG );
    HRESULT (*SetPalette)(void *, LPDIRECTDRAWPALETTE);
    HRESULT (*Unlock)(void *, LPVOID);
    HRESULT (*UpdateOverlay)(void *, LPRECT, LPDIRECTDRAWSURFACE,LPRECT,DWORD, LPDDOVERLAYFX);
    HRESULT (*UpdateOverlayDisplay)(void *, DWORD);
    HRESULT (*UpdateOverlayZOrder)(void *, DWORD, LPDIRECTDRAWSURFACE);
} fakeDirectDrawSurface;

typedef struct
{
    fakeDirectDrawSurface *Functions;

    DWORD width;
    DWORD height;
    DWORD bpp;
    DWORD caps;

    fakeDirectDrawPaletteObject *palette;

    void *surface;
    DWORD lPitch;
    DWORD lXPitch;

    HDC hDC;
    HGLRC hRC;
    int *glTex;

    ULONG Ref;

} fakeDirectDrawSurfaceObject;

extern fakeDirectDrawSurface siface;

#endif
