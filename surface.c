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

void dump_ddsd(DWORD);
void dump_ddscaps(DWORD);

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

    dump_ddsd(lpDDSurfaceDesc->dwFlags);

    fakeDirectDrawSurfaceObject *Surface = (fakeDirectDrawSurfaceObject *)malloc(sizeof(fakeDirectDrawSurfaceObject));

    Surface->Functions = &siface;

    /* private stuff */
    Surface->hDC = NULL;
    Surface->bpp = This->bpp;
    Surface->surface = NULL;
    Surface->caps = 0;

    if(lpDDSurfaceDesc->dwFlags & DDSD_CAPS)
    {
        if(lpDDSurfaceDesc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE)
        {
            PIXELFORMATDESCRIPTOR pfd;

            printf("Creating primary surface and initializing OpenGL\n");

            Surface->width = This->width;
            Surface->height = This->height;

            Surface->hDC = GetDC(This->hWnd);

            memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
            pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.cColorBits = 24;
            pfd.cDepthBits = 16;
            pfd.iLayerType = PFD_MAIN_PLANE;
            SetPixelFormat( Surface->hDC, ChoosePixelFormat( Surface->hDC, &pfd ), &pfd );
                
            Surface->hRC = wglCreateContext( Surface->hDC );
            wglMakeCurrent( Surface->hDC, Surface->hRC );
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
        Surface->surface = malloc(Surface->width * Surface->height * Surface->bpp / 8);
    }

    printf(" Surface = %p (%dx%d@%d)\n", Surface, (int)Surface->width, (int)Surface->height, (int)Surface->bpp);

    *lpDDSurface = (LPDIRECTDRAWSURFACE)Surface;

    Surface->Ref = 0;
    ddraw_surface_AddRef(Surface);

    return DD_OK;
}

HRESULT ddraw_surface_AddAttachedSurface(void *_This, LPDIRECTDRAWSURFACE lpDDSurface)
{
    printf("DirectDrawSurface::AddAttachedSurface(This=%p, lpDDSurface=%p)\n", _This, lpDDSurface);
    return DD_OK;
}

HRESULT ddraw_surface_Blt(void *This, LPRECT lpDestRect, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
    printf("DirectDrawSurface::Blt(This=%p, lpDestRect=%p, lpDDSrcSurface=%p, lpSrcRect=%p, dwFlags=%d, lpDDBltFx=%p)\n", This, lpDestRect, lpDDSrcSurface, lpSrcRect, (int)dwFlags, lpDDBltFx);
    if(lpDestRect)
    {
        printf(" dest: l: %d t: %d r: %d b: %d\n", (int)lpDestRect->left, (int)lpDestRect->top, (int)lpDestRect->right, (int)lpDestRect->bottom);
    }
    if(lpSrcRect)
    {
        printf("  src: l: %d t: %d r: %d b: %d\n", (int)lpSrcRect->left, (int)lpSrcRect->top, (int)lpSrcRect->right, (int)lpSrcRect->bottom);
    }
    return DD_OK;
}

HRESULT ddraw_surface_GetCaps(void *_This, LPDDSCAPS lpDDSCaps)
{
    fakeDirectDrawSurfaceObject *This = (fakeDirectDrawSurfaceObject *)_This;
    printf("DirectDrawSurface::GetCaps(This=%p, lpDDSCaps=%p)\n", _This, lpDDSCaps);
    lpDDSCaps->dwCaps = This->caps;
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
    lpDDSurfaceDesc->dwFlags = DDSD_LPSURFACE;
    lpDDSurfaceDesc->lpSurface = This->surface;

    return DD_OK;
}

HRESULT ddraw_surface_Unlock(void *_This, LPVOID lpRect)
{
    fakeDirectDrawSurfaceObject *This = (fakeDirectDrawSurfaceObject *)_This;
#if _DEBUG
    printf("DirectDrawSurface::Unlock(This=%p, lpRect=%p)\n", This, lpRect);
#endif

    if(This->caps & DDSCAPS_PRIMARYSURFACE)
    {
        /* clearing the screen for now */
        glClearColor( 0.0f, 0.0f, 255.0f, 0.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        SwapBuffers( This->hDC );
    }

    return DD_OK;
}

fakeDirectDrawSurface siface =
{
    /* IUnknown */
    QueryInterface,
    ddraw_surface_AddRef,
    ddraw_surface_Release,
    /* IDirectDrawSurface */
    ddraw_surface_AddAttachedSurface,
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
