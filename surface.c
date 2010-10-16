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

#include "main.h"
#include "surface.h"

/* from main */
HRESULT QueryInterface(void *This, REFIID riid, void **obj);
ULONG AddRef(void *This);
HRESULT null();

void dump_surface_desc_flags(LPDDSURFACEDESC lpDDSurfaceDesc);

ULONG ddraw_surface_AddRef(void *_This)
{
    fakeDirectDrawSurfaceObject *This = (fakeDirectDrawSurfaceObject *)_This;

    printf("DirectDrawSurface::AddRef(This=%p)\n", This);
    This->Ref++;
    return This->Ref;
}

ULONG ddraw_surface_Release(void *_This)
{
    fakeDirectDrawSurfaceObject *This = (fakeDirectDrawSurfaceObject *)_This;

    printf("DirectDrawSurface::Release(This=%p)\n", ((fakeDirectDrawSurfaceObject *)This));

    This->Ref--;

    if(This->Ref == 0)
    {
        if(This->surface)
        {
            free(This->surface);
        }
        free(This);
        return 0;
    }
    return This->Ref;
}

HRESULT ddraw_CreateSurface(void *_This, LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE FAR *lpDDSurface, IUnknown FAR * unkOuter)
{
    fakeDirectDrawObject *This = (fakeDirectDrawObject *)_This;

    printf("DirectDraw::CreateSurface(This=%p, lpDDSurfaceDesc=%p, lpDDSurface=%p, unkOuter=%p)\n", This, lpDDSurfaceDesc, lpDDSurface, unkOuter);

    dump_surface_desc_flags(lpDDSurfaceDesc);

    fakeDirectDrawSurfaceObject *Surface = (fakeDirectDrawSurfaceObject *)malloc(sizeof(fakeDirectDrawSurfaceObject));

    if(lpDDSurfaceDesc->dwFlags & DDSD_CAPS)
    {
        if(lpDDSurfaceDesc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE)
        {
            printf("  DDSCAPS_PRIMARYSURFACE\n");
            Surface->width = This->width;
            Surface->height = This->height;
        }
        if(lpDDSurfaceDesc->ddsCaps.dwCaps & DDSCAPS_OFFSCREENPLAIN)
        {
            printf("  DDSCAPS_OFFSCREENPLAIN\n");
        }
        if(lpDDSurfaceDesc->ddsCaps.dwCaps & DDSCAPS_VIDEOMEMORY)
        {
            printf("  DDSCAPS_VIDEOMEMORY\n");
        }
        if(lpDDSurfaceDesc->ddsCaps.dwCaps & DDSCAPS_LOCALVIDMEM)
        {
            printf("  DDSCAPS_LOCALVIDMEM\n");
        }
    }

    if( !(lpDDSurfaceDesc->dwFlags & DDSD_CAPS) || !(lpDDSurfaceDesc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) )
    {
        Surface->width = lpDDSurfaceDesc->dwWidth;
        Surface->height = lpDDSurfaceDesc->dwHeight;
    }

    Surface->Functions = &siface;

    /* private stuff */
    Surface->bpp = This->bpp;
    Surface->surface = NULL;

    if(Surface->width && Surface->height)
    {
        Surface->surface = malloc(Surface->width * Surface->height * Surface->bpp / 8);
    }

    printf(" Surface = %p (%dx%d@%d)\n", Surface, (int)Surface->width, (int)Surface->height, (int)Surface->bpp);

    Surface->Ref = 0;
    ddraw_surface_AddRef(Surface);

    *lpDDSurface = (LPDIRECTDRAWSURFACE)Surface;

    return DD_OK;
}

HRESULT ddraw_surface_Blt(void *This, LPRECT lpDestRect, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
    printf("DirectDrawSurface::Blt(This=%p, lpDestRect=%p, lpDDSrcSurface=%p, lpSrcRect=%p, dwFlags=%d, lpDDBltFx=%p)\n", This, lpDestRect, lpDDSrcSurface, lpSrcRect, (int)dwFlags, lpDDBltFx);
    return DD_OK;
}

HRESULT ddraw_surface_GetCaps(void *_This, LPDDSCAPS lpDDSCaps)
{
    printf("DirectDrawSurface::GetCaps(This=%p, lpDDSCaps=%p)\n", _This, lpDDSCaps);
    lpDDSCaps->dwCaps = 0x00000000l;
    return DD_OK;
}

HRESULT ddraw_surface_GetPalette(void *_This, LPDIRECTDRAWPALETTE FAR *lplpDDPalette)
{
    printf("DirectDrawSurface::GetPalette(This=%p, lplpDDPalette=%p)\n", _This, lplpDDPalette);
    return DD_OK;
}

HRESULT ddraw_surface_SetPalette(void *_This, LPDIRECTDRAWPALETTE lpDDPalette)
{
    printf("DirectDrawSurface::SetPalette(This=%p, lpDDPalette=%p)\n", _This, lpDDPalette);
    return DD_OK;
}

HRESULT ddraw_surface_Lock(void *_This, LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
{
    fakeDirectDrawSurfaceObject *This = (fakeDirectDrawSurfaceObject *)_This;

    printf("DirectDrawSurface::Lock(This=%p, lpDestRect=%p, lpDDSurfaceDesc=%p, dwFlags=%d, hEvent=%p)\n", This, lpDestRect, lpDDSurfaceDesc, (int)dwFlags, hEvent);

    if(dwFlags & DDLOCK_SURFACEMEMORYPTR)
    {
        printf(" dwFlags: DDLOCK_SURFACEMEMORYPTR\n");
    }
    if(dwFlags & DDLOCK_WAIT)
    {
        printf(" dwFlags: DDLOCK_WAIT\n");
    }
    if(dwFlags & DDLOCK_EVENT)
    {
        printf(" dwFlags: DDLOCK_EVENT\n");
    }
    if(dwFlags & DDLOCK_READONLY)
    {
        printf(" dwFlags: DDLOCK_READONLY\n");
    }
    if(dwFlags & DDLOCK_WRITEONLY)
    {
        printf(" dwFlags: DDLOCK_WRITEONLY\n");
    }

    lpDDSurfaceDesc->dwSize = sizeof(DDSURFACEDESC);
    lpDDSurfaceDesc->dwFlags = DDSD_LPSURFACE;
    lpDDSurfaceDesc->lpSurface = This->surface;

    return DD_OK;
}

HRESULT ddraw_surface_Unlock(void *This, LPVOID lpRect)
{
    printf("DirectDrawSurface::Unlock(This=%p, lpRect=%p)\n", This, lpRect);
    return DD_OK;
}

fakeDirectDrawSurface siface =
{
    /* IUnknown */
    QueryInterface,
    ddraw_surface_AddRef,
    ddraw_surface_Release,
    /* IDirectDrawSurface */
    null, // ddraw_surface_AddAttachedSurface
    null, // ddraw_surface_AddOverlayDirtyRect
    ddraw_surface_Blt,
    null, // ddraw_surface_BltBatch
    null, // ddraw_surface_BltFast
    null, // ddraw_surface_DeleteAttachedSurface
    null, // ddraw_surface_EnumAttachedSurfaces
    null, // ddraw_surface_EnumOverlayZOrders
    null, // ddraw_surface_Flip
    null, // ddraw_surface_GetAttachedSurface
    null, // ddraw_surface_GetBltStatus
    ddraw_surface_GetCaps,
    null, // ddraw_surface_GetClipper
    null, // ddraw_surface_GetColorKey
    null, // ddraw_surface_GetDC
    null, // ddraw_surface_GetFlipStatus
    null, // ddraw_surface_GetOverlayPosition
    ddraw_surface_GetPalette,
    null, // ddraw_surface_GetPixelFormat
    null, // ddraw_surface_GetSurfaceDesc
    null, // ddraw_surface_Initialize
    null, // ddraw_surface_IsLost
    ddraw_surface_Lock,
    null, // ddraw_surface_ReleaseDC
    null, // ddraw_surface_Restore
    null, // ddraw_surface_SetClipper
    null, // ddraw_surface_SetColorKey
    null, // ddraw_surface_SetOverlayPosition
    ddraw_surface_SetPalette,
    ddraw_surface_Unlock,
    null, // ddraw_surface_UpdateOverlay
    null, // ddraw_surface_UpdateOverlayDisplay
    null  // ddraw_surface_UpdateOverlayZOrder
};

void dump_surface_desc_flags(LPDDSURFACEDESC lpDDSurfaceDesc)
{
    if(lpDDSurfaceDesc->dwFlags & DDSD_CAPS)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_CAPS\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_HEIGHT)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_HEIGHT\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_WIDTH)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_WIDTH\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_PITCH)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_PITCH\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_BACKBUFFERCOUNT)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_BACKBUFFERCOUNT\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_ZBUFFERBITDEPTH)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_ZBUFFERBITDEPTH\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_ALPHABITDEPTH)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_ALPHABITDEPTH\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_LPSURFACE)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_LPSURFACE\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_PIXELFORMAT)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_PIXELFORMAT\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_CKDESTOVERLAY)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_CKDESTOVERLAY\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_CKDESTBLT)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_CKDESTBLT\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_CKSRCOVERLAY)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_CKSRCOVERLAY\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_CKSRCBLT)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_CKSRCBLT\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_MIPMAPCOUNT)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_MIPMAPCOUNT\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_REFRESHRATE)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_REFRESHRATE\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_LINEARSIZE)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_LINEARSIZE\n");
    }
    if(lpDDSurfaceDesc->dwFlags & DDSD_ALL)
    {
        printf(" lpDDSurfaceDesc->dwFlags: DDSD_ALL\n");
    }
}
