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
#include "surface.h"

/* from main */
HRESULT QueryInterface(void *This, REFIID riid, void **obj);
ULONG AddRef(void *This);
ULONG Release(void *This);
HRESULT null();

fakeDirectDrawSurface siface =
{
    /* IUnknown */
    QueryInterface,
    AddRef,
    Release,
    /* IDirectDrawSurface */
    null, // ddraw_surface_AddAttachedSurface
    null, // ddraw_surface_AddOverlayDirtyRect
    null, // ddraw_surface_Blt
    null, // ddraw_surface_BltBatch
    null, // ddraw_surface_BltFast
    null, // ddraw_surface_DeleteAttachedSurface
    null, // ddraw_surface_EnumAttachedSurfaces
    null, // ddraw_surface_EnumOverlayZOrders
    null, // ddraw_surface_Flip
    null, // ddraw_surface_GetAttachedSurface
    null, // ddraw_surface_GetBltStatus
    null, // ddraw_surface_GetCaps
    null, // ddraw_surface_GetClipper
    null, // ddraw_surface_GetColorKey
    null, // ddraw_surface_GetDC
    null, // ddraw_surface_GetFlipStatus
    null, // ddraw_surface_GetOverlayPosition
    null, // ddraw_surface_GetPalette
    null, // ddraw_surface_GetPixelFormat
    null, // ddraw_surface_GetSurfaceDesc
    null, // ddraw_surface_Initialize
    null, // ddraw_surface_IsLost
    null, // ddraw_surface_Lock
    null, // ddraw_surface_ReleaseDC
    null, // ddraw_surface_Restore
    null, // ddraw_surface_SetClipper
    null, // ddraw_surface_SetColorKey
    null, // ddraw_surface_SetOverlayPosition
    null, // ddraw_surface_SetPalette
    null, // ddraw_surface_Unlock
    null, // ddraw_surface_UpdateOverlay
    null, // ddraw_surface_UpdateOverlayDisplay
    null  // ddraw_surface_UpdateOverlayZOrder
};
