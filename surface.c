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
    null, // AddAttachedSurface
    null, // AddOverlayDirtyRect
    null, // Blt
    null, // BltBatch
    null, // BltFast
    null, // DeleteAttachedSurface
    null, // EnumAttachedSurfaces
    null, // EnumOverlayZOrders
    null, // Flip
    null, // GetAttachedSurface
    null, // GetBltStatus
    null, // GetCaps
    null, // GetClipper
    null, // GetColorKey
    null, // GetDC
    null, // GetFlipStatus
    null, // GetOverlayPosition
    null, // GetPalette
    null, // GetPixelFormat
    null, // GetSurfaceDesc
    null, // Initialize
    null, // IsLost
    null, // Lock
    null, // ReleaseDC
    null, // Restore
    null, // SetClipper
    null, // SetColorKey
    null, // SetOverlayPosition
    null, // SetPalette
    null, // Unlock
    null, // UpdateOverlay
    null, // UpdateOverlayDisplay
    null  // UpdateOverlayZOrder
};
