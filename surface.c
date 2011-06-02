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

void dump_ddscaps(DWORD dwCaps);
void dump_ddsd(DWORD dwFlags);

HRESULT __stdcall ddraw_surface_QueryInterface(IDirectDrawSurfaceImpl *This, REFIID riid, void **obj)
{
    printf("DirectDrawSurface::QueryInterface(This=%p, riid=%08X, obj=%p)\n", This, (unsigned int)riid, obj);
    return S_OK;
}

ULONG __stdcall ddraw_surface_AddRef(IDirectDrawSurfaceImpl *This)
{
    printf("DirectDrawSurface::AddRef(This=%p)\n", This);
    This->Ref++;
    return This->Ref;
}

ULONG __stdcall ddraw_surface_Release(IDirectDrawSurfaceImpl *This)
{
    printf("DirectDrawSurface::Release(This=%p)\n", This);

    This->Ref--;

    if(This->Ref == 0)
    {
        if(This->caps == DDSCAPS_PRIMARYSURFACE)
        {
            EnterCriticalSection(&ddraw->cs);
            ddraw->primary = NULL;
            LeaveCriticalSection(&ddraw->cs);
        }
        if(This->surface)
        {
            HeapFree(GetProcessHeap(), 0, This->surface);
        }
        if(This->palette)
        {
            IDirectDrawPalette_Release(This->palette);
        }
        HeapFree(GetProcessHeap(), 0, This);
        return 0;
    }
    return This->Ref;
}

HRESULT __stdcall ddraw_surface_AddAttachedSurface(IDirectDrawSurfaceImpl *This, LPDIRECTDRAWSURFACE lpDDSurface)
{
    printf("DirectDrawSurface::AddAttachedSurface(This=%p, lpDDSurface=%p)\n", This, lpDDSurface);
    IDirectDrawSurface_AddRef(lpDDSurface);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_AddOverlayDirtyRect(IDirectDrawSurfaceImpl *This, LPRECT a)
{
    printf("DirectDrawSurface::AddOverlayDirtyRect(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_Blt(IDirectDrawSurfaceImpl *This, LPRECT lpDestRect, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
    IDirectDrawSurfaceImpl *Source = (IDirectDrawSurfaceImpl *)lpDDSrcSurface;

#if _DEBUG
    printf("DirectDrawSurface::Blt(This=%p, lpDestRect=%p, lpDDSrcSurface=%p, lpSrcRect=%p, dwFlags=%d, lpDDBltFx=%p)\n", This, lpDestRect, lpDDSrcSurface, lpSrcRect, (int)dwFlags, lpDDBltFx);
    if(lpDestRect)
    {
        printf(" dest: l: %d t: %d r: %d b: %d\n", (int)lpDestRect->left, (int)lpDestRect->top, (int)lpDestRect->right, (int)lpDestRect->bottom);
    }
    if(lpSrcRect)
    {
        printf("  src: l: %d t: %d r: %d b: %d\n", (int)lpSrcRect->left, (int)lpSrcRect->top, (int)lpSrcRect->right, (int)lpSrcRect->bottom);
    }
#endif

    if(This->caps & DDSCAPS_PRIMARYSURFACE && ddraw->render.run && !ddraw->render.flip)
    {
        WaitForSingleObject(ddraw->render.ev, INFINITE);
        ResetEvent(ddraw->render.ev);
    }

    if(Source)
    {
        int dx=0,dy=0; 
        if (lpDestRect)
        {
            dx=lpDestRect->left;
            dy=lpDestRect->top; 
        }
        int x0=0,y0=0,x1=Source->width,y1=Source->height; 
        if (lpSrcRect)
        { 
            x0 = max(x0, lpSrcRect->left); 
            x1 = min(x1, lpSrcRect->right); 
            y0 = max(y0, lpSrcRect->top); 
            y1 = min(y1, lpSrcRect->bottom); 
        } 
        unsigned char* to=This->surface + dy*This->width + dx; 
        unsigned char* from=Source->surface + y0*Source->width + x0; 
        int s = x1-x0; 

        int y;
        for(y=y0; y<y1; ++y, to+=This->width, from+=Source->width)
        { 
            memcpy(to, from, s); 
        } 
    }

    return DD_OK;
}

HRESULT __stdcall ddraw_surface_BltBatch(IDirectDrawSurfaceImpl *This, LPDDBLTBATCH a, DWORD b, DWORD c)
{
    printf("IDirectDrawSurface::BltBatch(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_BltFast(IDirectDrawSurfaceImpl *This, DWORD a, DWORD b, LPDIRECTDRAWSURFACE c, LPRECT d, DWORD e)
{
    printf("IDirectDrawSurface::BltFast(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_DeleteAttachedSurface(IDirectDrawSurfaceImpl *This, DWORD dwFlags, LPDIRECTDRAWSURFACE lpDDSurface)
{
    printf("IDirectDrawSurface::DeleteAttachedSurface(This=%p, dwFlags=%d, lpDDSurface=%p)\n", This, (int)dwFlags, lpDDSurface);
    IDirectDrawSurface_Release(lpDDSurface);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetSurfaceDesc(IDirectDrawSurfaceImpl *, LPDDSURFACEDESC);

HRESULT __stdcall ddraw_surface_EnumAttachedSurfaces(IDirectDrawSurfaceImpl *This, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
{
    printf("IDirectDrawSurface::EnumAttachedSurfaces(This=%p, lpContest=%p, lpEnumSurfacesCallback=%p)\n", This, lpContext, lpEnumSurfacesCallback);

    /* this is not actually complete, but Carmageddon seems to call EnumAttachedSurfaces instead of GetSurfaceDesc to get the main surface description */
    LPDDSURFACEDESC lpDDSurfaceDesc = malloc(sizeof(DDSURFACEDESC));
    ddraw_surface_GetSurfaceDesc(This, lpDDSurfaceDesc);
    free(lpDDSurfaceDesc);
    lpEnumSurfacesCallback((LPDIRECTDRAWSURFACE)This, lpDDSurfaceDesc, lpContext);

    return DD_OK;
}

HRESULT __stdcall ddraw_surface_EnumOverlayZOrders(IDirectDrawSurfaceImpl *This, DWORD a, LPVOID b, LPDDENUMSURFACESCALLBACK c)
{
    printf("IDirectDrawSurface::EnumOverlayZOrders(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_Flip(IDirectDrawSurfaceImpl *This, LPDIRECTDRAWSURFACE a, DWORD b)
{
#if _DEBUG
    printf("DirectDrawSurface::Flip(This=%p, ...)\n", This);
#endif
    DWORD frame_len = 1000.0f / 60;
    DWORD tick_end = GetTickCount();
    static DWORD tick_start = 0;
    if(This->caps & DDSCAPS_PRIMARYSURFACE && ddraw->render.run)
    {
        if (!ddraw->render.flip)
        {
            printf("DirectDrawSurface::Flip: Detected sane renderer, honoring flip\n");
            ddraw->render.flip = TRUE;
        }
        else
        {
            SetEvent(ddraw->render.ev);

            tick_end = GetTickCount();
            if(tick_end - tick_start < frame_len)
            {
                Sleep( frame_len - (tick_end - tick_start) );
            }

            tick_start = tick_end;
        }
    }
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetAttachedSurface(IDirectDrawSurfaceImpl *This, LPDDSCAPS a, LPDIRECTDRAWSURFACE FAR *b)
{
    printf("IDirectDrawSurface::GetAttachedSurface(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetBltStatus(IDirectDrawSurfaceImpl *This, DWORD a)
{
#if _DEBUG
    printf("IDirectDrawSurface::GetBltStatus(This=%p, ...)\n", This);
#endif
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetCaps(IDirectDrawSurfaceImpl *This, LPDDSCAPS lpDDSCaps)
{
    printf("DirectDrawSurface::GetCaps(This=%p, lpDDSCaps=%p)\n", This, lpDDSCaps);
    lpDDSCaps->dwCaps = This->caps;
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetClipper(IDirectDrawSurfaceImpl *This, LPDIRECTDRAWCLIPPER FAR *a)
{
    printf("IDirectDrawSurface::GetClipper(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetColorKey(IDirectDrawSurfaceImpl *This, DWORD a, LPDDCOLORKEY b)
{
    printf("IDirectDrawSurface::GetColorKey(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetDC(IDirectDrawSurfaceImpl *This, HDC FAR *a)
{
    printf("IDirectDrawSurface::GetDC(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetFlipStatus(IDirectDrawSurfaceImpl *This, DWORD a)
{
    printf("IDirectDrawSurface::GetFlipStatus(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetOverlayPosition(IDirectDrawSurfaceImpl *This, LPLONG a, LPLONG b)
{
    printf("IDirectDrawSurface::GetOverlayPosition(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetPalette(IDirectDrawSurfaceImpl *This, LPDIRECTDRAWPALETTE FAR *lplpDDPalette)
{
    printf("DirectDrawSurface::GetPalette(This=%p, lplpDDPalette=%p)\n", This, lplpDDPalette);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetPixelFormat(IDirectDrawSurfaceImpl *This, LPDDPIXELFORMAT a)
{
    printf("IDirectDrawSurface::GetPixelFormat(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetSurfaceDesc(IDirectDrawSurfaceImpl *This, LPDDSURFACEDESC a)
{
    printf("IDirectDrawSurface::GetSurfaceDesc(This=%p, ...)\n", This);

    a->dwFlags = DDSD_WIDTH|DDSD_HEIGHT|DDSD_PITCH|DDSD_PIXELFORMAT;
    a->dwWidth = This->width;
    a->dwHeight = This->height;
    a->lPitch = This->lPitch;
    a->ddpfPixelFormat.dwFlags = DDPF_RGB;
    a->ddpfPixelFormat.dwRGBBitCount = This->bpp;
    /* RGB 555 */
    a->ddpfPixelFormat.dwRBitMask = 0x7C00;
    a->ddpfPixelFormat.dwGBitMask = 0x03E0;
    a->ddpfPixelFormat.dwBBitMask = 0x001F;

    return DD_OK;
}

HRESULT __stdcall ddraw_surface_Initialize(IDirectDrawSurfaceImpl *This, LPDIRECTDRAW a, LPDDSURFACEDESC b)
{
    printf("IDirectDrawSurface::Initialize(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_IsLost(IDirectDrawSurfaceImpl *This)
{
#if _DEBUG
    printf("IDirectDrawSurface::IsLost(This=%p)\n", This);
#endif
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_Lock(IDirectDrawSurfaceImpl *This, LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
{
#if _DEBUG
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
#endif

    lpDDSurfaceDesc->dwSize = sizeof(DDSURFACEDESC);
    lpDDSurfaceDesc->dwFlags = DDSD_LPSURFACE|DDSD_PITCH;
    lpDDSurfaceDesc->lpSurface = This->surface;
    lpDDSurfaceDesc->lPitch = This->lPitch;
    lpDDSurfaceDesc->ddpfPixelFormat.dwFlags = DDPF_RGB;
    lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount = This->bpp;
    /* RGB 555 */
    lpDDSurfaceDesc->ddpfPixelFormat.dwRBitMask = 0x7C00;
    lpDDSurfaceDesc->ddpfPixelFormat.dwGBitMask = 0x03E0;
    lpDDSurfaceDesc->ddpfPixelFormat.dwBBitMask = 0x001F;

    return DD_OK;
}

HRESULT __stdcall ddraw_surface_ReleaseDC(IDirectDrawSurfaceImpl *This, HDC a)
{
    printf("DirectDrawSurface::ReleaseDC(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_Restore(IDirectDrawSurfaceImpl *This)
{
    printf("DirectDrawSurface::Restore(This=%p)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_SetClipper(IDirectDrawSurfaceImpl *This, LPDIRECTDRAWCLIPPER a)
{
    printf("DirectDrawSurface::SetClipper(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_SetColorKey(IDirectDrawSurfaceImpl *This, DWORD a, LPDDCOLORKEY b)
{
    printf("DirectDrawSurface::SetColorKey(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_SetOverlayPosition(IDirectDrawSurfaceImpl *This, LONG a, LONG b)
{
    printf("DirectDrawSurface::SetOverlayPosition(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_SetPalette(IDirectDrawSurfaceImpl *This, LPDIRECTDRAWPALETTE lpDDPalette)
{
    printf("DirectDrawSurface::SetPalette(This=%p, lpDDPalette=%p)\n", This, lpDDPalette);

    IDirectDrawPalette_AddRef(lpDDPalette);

    if(This->palette)
    {
        IDirectDrawPalette_Release(This->palette);
    }

    This->palette = (IDirectDrawPaletteImpl *)lpDDPalette;

    return DD_OK;
}

HRESULT __stdcall ddraw_surface_Unlock(IDirectDrawSurfaceImpl *This, LPVOID lpRect)
{
#if _DEBUG
    printf("DirectDrawSurface::Unlock(This=%p, lpRect=%p)\n", This, lpRect);
#endif

    return DD_OK;
}

HRESULT __stdcall ddraw_surface_UpdateOverlay(IDirectDrawSurfaceImpl *This, LPRECT a, LPDIRECTDRAWSURFACE b, LPRECT c, DWORD d, LPDDOVERLAYFX e)
{
    printf("DirectDrawSurface::UpdateOverlay(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_UpdateOverlayDisplay(IDirectDrawSurfaceImpl *This, DWORD a)
{
    printf("DirectDrawSurface::UpdateOverlayDisplay(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_UpdateOverlayZOrder(IDirectDrawSurfaceImpl *This, DWORD a, LPDIRECTDRAWSURFACE b)
{
    printf("DirectDrawSurface::UpdateOverlayZOrder(This=%p, ...)\n", This);
    return DD_OK;
}

struct IDirectDrawSurfaceImplVtbl siface =
{
    /* IUnknown */
    ddraw_surface_QueryInterface,
    ddraw_surface_AddRef,
    ddraw_surface_Release,
    /* IDirectDrawSurface */
    ddraw_surface_AddAttachedSurface,
    ddraw_surface_AddOverlayDirtyRect,
    ddraw_surface_Blt,
    ddraw_surface_BltBatch,
    ddraw_surface_BltFast,
    ddraw_surface_DeleteAttachedSurface,
    ddraw_surface_EnumAttachedSurfaces,
    ddraw_surface_EnumOverlayZOrders,
    ddraw_surface_Flip,
    ddraw_surface_GetAttachedSurface,
    ddraw_surface_GetBltStatus,
    ddraw_surface_GetCaps,
    ddraw_surface_GetClipper,
    ddraw_surface_GetColorKey,
    ddraw_surface_GetDC,
    ddraw_surface_GetFlipStatus,
    ddraw_surface_GetOverlayPosition,
    ddraw_surface_GetPalette,
    ddraw_surface_GetPixelFormat,
    ddraw_surface_GetSurfaceDesc,
    ddraw_surface_Initialize,
    ddraw_surface_IsLost,
    ddraw_surface_Lock,
    ddraw_surface_ReleaseDC,
    ddraw_surface_Restore,
    ddraw_surface_SetClipper,
    ddraw_surface_SetColorKey,
    ddraw_surface_SetOverlayPosition,
    ddraw_surface_SetPalette,
    ddraw_surface_Unlock,
    ddraw_surface_UpdateOverlay,
    ddraw_surface_UpdateOverlayDisplay,
    ddraw_surface_UpdateOverlayZOrder
};

HRESULT __stdcall ddraw_CreateSurface(IDirectDrawImpl *This, LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE FAR *lpDDSurface, IUnknown FAR * unkOuter)
{
    printf("DirectDraw::CreateSurface(This=%p, lpDDSurfaceDesc=%p, lpDDSurface=%p, unkOuter=%p)\n", This, lpDDSurfaceDesc, lpDDSurface, unkOuter);

    dump_ddsd(lpDDSurfaceDesc->dwFlags);

    IDirectDrawSurfaceImpl *Surface = (IDirectDrawSurfaceImpl *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(IDirectDrawSurfaceImpl));

    Surface->lpVtbl = &siface;

    /* private stuff */
    Surface->bpp = This->bpp;

    if(lpDDSurfaceDesc->dwFlags & DDSD_CAPS)
    {
        if(lpDDSurfaceDesc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE)
        {
            ddraw->primary = Surface;

            Surface->width = This->width;
            Surface->height = This->height;
        }

        dump_ddscaps(lpDDSurfaceDesc->ddsCaps.dwCaps);
        Surface->caps = lpDDSurfaceDesc->ddsCaps.dwCaps;
    }

    if( !(lpDDSurfaceDesc->dwFlags & DDSD_CAPS) || !(lpDDSurfaceDesc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) )
    {
        Surface->width = lpDDSurfaceDesc->dwWidth;
        Surface->height = lpDDSurfaceDesc->dwHeight;
    }

    if(Surface->width && Surface->height)
    {
        Surface->lXPitch = Surface->bpp / 8;
        Surface->lPitch = Surface->width * Surface->lXPitch;
        Surface->surface = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, Surface->lPitch * Surface->height * Surface->lXPitch);
    }

    printf(" Surface = %p (%dx%d@%d)\n", Surface, (int)Surface->width, (int)Surface->height, (int)Surface->bpp);

    *lpDDSurface = (LPDIRECTDRAWSURFACE)Surface;

    Surface->Ref = 0;
    ddraw_surface_AddRef(Surface);

    return DD_OK;
}

void dump_ddscaps(DWORD dwCaps)
{
    if(dwCaps & DDSCAPS_PRIMARYSURFACE)
    {
        printf("    DDSCAPS_PRIMARYSURFACE\n");
    }
    if(dwCaps & DDSCAPS_OFFSCREENPLAIN)
    {
        printf("    DDSCAPS_OFFSCREENPLAIN\n");
    }
    if(dwCaps & DDSCAPS_VIDEOMEMORY)
    {
        printf("    DDSCAPS_VIDEOMEMORY\n");
    }
    if(dwCaps & DDSCAPS_LOCALVIDMEM)
    {
        printf("    DDSCAPS_LOCALVIDMEM\n");
    }
}

void dump_ddsd(DWORD dwFlags)
{
    if(dwFlags & DDSD_CAPS)
    {
        printf("    DDSD_CAPS\n");
    }
    if(dwFlags & DDSD_HEIGHT)
    {
        printf("    DDSD_HEIGHT\n");
    }
    if(dwFlags & DDSD_WIDTH)
    {
        printf("    DDSD_WIDTH\n");
    }
    if(dwFlags & DDSD_PITCH)
    {
        printf("    DDSD_PITCH\n");
    }
    if(dwFlags & DDSD_BACKBUFFERCOUNT)
    {
        printf("    DDSD_BACKBUFFERCOUNT\n");
    }
    if(dwFlags & DDSD_ZBUFFERBITDEPTH)
    {
        printf("    DDSD_ZBUFFERBITDEPTH\n");
    }
    if(dwFlags & DDSD_ALPHABITDEPTH)
    {
        printf("    DDSD_ALPHABITDEPTH\n");
    }
    if(dwFlags & DDSD_LPSURFACE)
    {
        printf("    DDSD_LPSURFACE\n");
    }
    if(dwFlags & DDSD_PIXELFORMAT)
    {
        printf("    DDSD_PIXELFORMAT\n");
    }
    if(dwFlags & DDSD_CKDESTOVERLAY)
    {
        printf("    DDSD_CKDESTOVERLAY\n");
    }
    if(dwFlags & DDSD_CKDESTBLT)
    {
        printf("    DDSD_CKDESTBLT\n");
    }
    if(dwFlags & DDSD_CKSRCOVERLAY)
    {
        printf("    DDSD_CKSRCOVERLAY\n");
    }
    if(dwFlags & DDSD_CKSRCBLT)
    {
        printf("    DDSD_CKSRCBLT\n");
    }
    if(dwFlags & DDSD_MIPMAPCOUNT)
    {
        printf("    DDSD_MIPMAPCOUNT\n");
    }
    if(dwFlags & DDSD_REFRESHRATE)
    {
        printf("    DDSD_REFRESHRATE\n");
    }
    if(dwFlags & DDSD_LINEARSIZE)
    {
        printf("    DDSD_LINEARSIZE\n");
    }
    if(dwFlags & DDSD_ALL)
    {
        printf("    DDSD_ALL\n");
    }
}
