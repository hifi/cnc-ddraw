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

HRESULT __stdcall ddraw_CreateSurface(IDirectDrawImpl *This, LPDDSURFACEDESC DDSurfaceDesc, LPDIRECTDRAWSURFACE FAR *DDSurface, IUnknown FAR * unkOuter);

struct IDirectDrawSurfaceImpl;
struct IDirectDrawSurfaceImplVtbl;

extern struct IDirectDrawSurfaceImpl *ddraw_primary;

typedef struct IDirectDrawSurfaceImpl
{
    struct IDirectDrawSurfaceImplVtbl *lpVtbl;

    ULONG Ref;

    DWORD width;
    DWORD height;
    DWORD bpp;
    DWORD flags;
    DWORD caps;

    IDirectDrawPaletteImpl *palette;

    void *surface;
    DWORD lPitch;
    DWORD lXPitch;

} IDirectDrawSurfaceImpl;

struct IDirectDrawSurfaceImplVtbl
{
    /* IUnknown */
    HRESULT (__stdcall *QueryInterface)(IDirectDrawSurfaceImpl*, REFIID, void**);
    ULONG (__stdcall *AddRef)(IDirectDrawSurfaceImpl*);
    ULONG (__stdcall *Release)(IDirectDrawSurfaceImpl*);

    /* IDirectDrawSurface */
    HRESULT (__stdcall *AddAttachedSurface)(IDirectDrawSurfaceImpl*, LPDIRECTDRAWSURFACE);
    HRESULT (__stdcall *AddOverlayDirtyRect)(IDirectDrawSurfaceImpl*, LPRECT);
    HRESULT (__stdcall *Blt)(IDirectDrawSurfaceImpl*, LPRECT,LPDIRECTDRAWSURFACE, LPRECT,DWORD, LPDDBLTFX);
    HRESULT (__stdcall *BltBatch)(IDirectDrawSurfaceImpl*, LPDDBLTBATCH, DWORD, DWORD );
    HRESULT (__stdcall *BltFast)(IDirectDrawSurfaceImpl*, DWORD,DWORD,LPDIRECTDRAWSURFACE, LPRECT,DWORD);
    HRESULT (__stdcall *DeleteAttachedSurface)(IDirectDrawSurfaceImpl*, DWORD,LPDIRECTDRAWSURFACE);
    HRESULT (__stdcall *EnumAttachedSurfaces)(IDirectDrawSurfaceImpl*, LPVOID,LPDDENUMSURFACESCALLBACK);
    HRESULT (__stdcall *EnumOverlayZOrders)(IDirectDrawSurfaceImpl*, DWORD,LPVOID,LPDDENUMSURFACESCALLBACK);
    HRESULT (__stdcall *Flip)(IDirectDrawSurfaceImpl*, LPDIRECTDRAWSURFACE, DWORD);
    HRESULT (__stdcall *GetAttachedSurface)(IDirectDrawSurfaceImpl*, LPDDSCAPS, LPDIRECTDRAWSURFACE FAR *);
    HRESULT (__stdcall *GetBltStatus)(IDirectDrawSurfaceImpl*, DWORD);
    HRESULT (__stdcall *GetCaps)(IDirectDrawSurfaceImpl*, LPDDSCAPS);
    HRESULT (__stdcall *GetClipper)(IDirectDrawSurfaceImpl*, LPDIRECTDRAWCLIPPER FAR*);
    HRESULT (__stdcall *GetColorKey)(IDirectDrawSurfaceImpl*, DWORD, LPDDCOLORKEY);
    HRESULT (__stdcall *GetDC)(IDirectDrawSurfaceImpl*, HDC FAR *);
    HRESULT (__stdcall *GetFlipStatus)(IDirectDrawSurfaceImpl*, DWORD);
    HRESULT (__stdcall *GetOverlayPosition)(IDirectDrawSurfaceImpl*, LPLONG, LPLONG );
    HRESULT (__stdcall *GetPalette)(IDirectDrawSurfaceImpl*, LPDIRECTDRAWPALETTE FAR*);
    HRESULT (__stdcall *GetPixelFormat)(IDirectDrawSurfaceImpl*, LPDDPIXELFORMAT);
    HRESULT (__stdcall *GetSurfaceDesc)(IDirectDrawSurfaceImpl*, LPDDSURFACEDESC);
    HRESULT (__stdcall *Initialize)(IDirectDrawSurfaceImpl*, LPDIRECTDRAW, LPDDSURFACEDESC);
    HRESULT (__stdcall *IsLost)(IDirectDrawSurfaceImpl*);
    HRESULT (__stdcall *Lock)(IDirectDrawSurfaceImpl*, LPRECT,LPDDSURFACEDESC,DWORD,HANDLE);
    HRESULT (__stdcall *ReleaseDC)(IDirectDrawSurfaceImpl*, HDC);
    HRESULT (__stdcall *Restore)(IDirectDrawSurfaceImpl*);
    HRESULT (__stdcall *SetClipper)(IDirectDrawSurfaceImpl*, LPDIRECTDRAWCLIPPER);
    HRESULT (__stdcall *SetColorKey)(IDirectDrawSurfaceImpl*, DWORD, LPDDCOLORKEY);
    HRESULT (__stdcall *SetOverlayPosition)(IDirectDrawSurfaceImpl*, LONG, LONG );
    HRESULT (__stdcall *SetPalette)(IDirectDrawSurfaceImpl*, LPDIRECTDRAWPALETTE);
    HRESULT (__stdcall *Unlock)(IDirectDrawSurfaceImpl*, LPVOID);
    HRESULT (__stdcall *UpdateOverlay)(IDirectDrawSurfaceImpl*, LPRECT, LPDIRECTDRAWSURFACE,LPRECT,DWORD, LPDDOVERLAYFX);
    HRESULT (__stdcall *UpdateOverlayDisplay)(IDirectDrawSurfaceImpl*, DWORD);
    HRESULT (__stdcall *UpdateOverlayZOrder)(IDirectDrawSurfaceImpl*, DWORD, LPDIRECTDRAWSURFACE);
} IDirectDrawSurfaceImplVtbl;


#endif
