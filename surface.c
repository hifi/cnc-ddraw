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
HRESULT null();

DWORD WINAPI ogl_Thread(void *_This);
DWORD WINAPI dd_Thread(IDirectDrawSurfaceImpl *This);
void dump_ddsd(DWORD);
void dump_ddscaps(DWORD);

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
        if(This->dThread)
        {
            This->dRun = FALSE;
            SetEvent(This->flipEvent);
            WaitForSingleObject(This->dThread, INFINITE);
        }
        if(This->surface)
        {
            free(This->surface);
        }
#if USE_OPENGL
        if(This->glTex)
        {
            free(This->glTex);
        }
#endif
        if(This->palette)
        {
            IDirectDrawPalette_Release(This->palette);
        }
        free(This);
        return 0;
    }
    return This->Ref;
}

HRESULT __stdcall ddraw_surface_AddAttachedSurface(IDirectDrawSurfaceImpl *This, LPDIRECTDRAWSURFACE lpDDSurface)
{
    printf("DirectDrawSurface::AddAttachedSurface(This=%p, lpDDSurface=%p)\n", This, lpDDSurface);
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

    SetEvent(This->flipEvent);

    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetCaps(IDirectDrawSurfaceImpl *This, LPDDSCAPS lpDDSCaps)
{
    printf("DirectDrawSurface::GetCaps(This=%p, lpDDSCaps=%p)\n", This, lpDDSCaps);
    lpDDSCaps->dwCaps = This->caps;
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_GetPalette(IDirectDrawSurfaceImpl *This, LPDIRECTDRAWPALETTE FAR *lplpDDPalette)
{
    printf("DirectDrawSurface::GetPalette(This=%p, lplpDDPalette=%p)\n", This, lplpDDPalette);
    return DD_OK;
}

HRESULT __stdcall ddraw_surface_SetPalette(IDirectDrawSurfaceImpl *This, LPDIRECTDRAWPALETTE lpDDPalette)
{
    printf("DirectDrawSurface::SetPalette(This=%p, lpDDPalette=%p)\n", This, lpDDPalette);
    This->palette = (IDirectDrawPaletteImpl *)lpDDPalette;
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

    return DD_OK;
}

HRESULT __stdcall ddraw_surface_Unlock(IDirectDrawSurfaceImpl *This, LPVOID lpRect)
{
#if _DEBUG
    printf("DirectDrawSurface::Unlock(This=%p, lpRect=%p)\n", This, lpRect);
#endif

    if(This->caps & DDSCAPS_PRIMARYSURFACE)
    {
        SetEvent(This->flipEvent);
    }

    return DD_OK;
}

HRESULT __stdcall ddraw_surface_Restore(IDirectDrawSurfaceImpl *This)
{
    printf("DirectDrawSurface::Restore(This=%p)\n", This);
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
    ddraw_surface_Restore,
    null, // ddraw_surface_SetClipper
    null, // ddraw_surface_SetColorKey
    null, // ddraw_surface_SetOverlayPosition
    ddraw_surface_SetPalette,
    ddraw_surface_Unlock,
    null, // ddraw_surface_UpdateOverlay
    null, // ddraw_surface_UpdateOverlayDisplay
    null  // ddraw_surface_UpdateOverlayZOrder
};

HRESULT ddraw_CreateSurface(IDirectDrawImpl *This, LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE FAR *lpDDSurface, IUnknown FAR * unkOuter)
{
    printf("DirectDraw::CreateSurface(This=%p, lpDDSurfaceDesc=%p, lpDDSurface=%p, unkOuter=%p)\n", This, lpDDSurfaceDesc, lpDDSurface, unkOuter);

    dump_ddsd(lpDDSurfaceDesc->dwFlags);

    IDirectDrawSurfaceImpl *Surface = (IDirectDrawSurfaceImpl *)HeapAlloc(GetProcessHeap(), 0, sizeof(IDirectDrawSurfaceImpl));

    Surface->lpVtbl = &siface;

    /* private stuff */
    Surface->parent = This;
    Surface->bpp = This->bpp;
    Surface->surface = NULL;
    Surface->caps = 0;
    Surface->palette = NULL;
    Surface->dThread = NULL;
    Surface->dRun = TRUE;
#if USE_OPENGL
    Surface->hDC = NULL;
    Surface->glTex = NULL;
#endif

    if(lpDDSurfaceDesc->dwFlags & DDSD_CAPS)
    {
        if(lpDDSurfaceDesc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE)
        {
            Surface->width = This->width;
            Surface->height = This->height;
            Surface->hWnd = This->hWnd;
#if USE_OPENGL
            Surface->dThread = CreateThread(NULL, 0, ogl_Thread, (void *)Surface, 0, NULL);
#else
            Surface->dThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dd_Thread, (void *)Surface, 0, NULL);
#endif
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
        Surface->lPitch = Surface->width;
        Surface->lXPitch = Surface->bpp / 8;
        Surface->surface = malloc(Surface->width * Surface->height * Surface->lXPitch);
#if USE_OPENGL
        Surface->glTex = malloc(Surface->width * Surface->height * sizeof(int));
#endif
    }

    printf(" Surface = %p (%dx%d@%d)\n", Surface, (int)Surface->width, (int)Surface->height, (int)Surface->bpp);

    *lpDDSurface = (LPDIRECTDRAWSURFACE)Surface;

    Surface->Ref = 0;
    ddraw_surface_AddRef(Surface);

    return DD_OK;
}

#if USE_OPENGL
DWORD WINAPI ogl_Thread(IDirectDrawSurfaceImpl *This)
{
    int i,j;
    PIXELFORMATDESCRIPTOR pfd;

    This->hDC = GetDC(This->hWnd);

    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    SetPixelFormat( This->hDC, ChoosePixelFormat( This->hDC, &pfd ), &pfd );

    This->hRC = wglCreateContext( This->hDC );
    wglMakeCurrent( This->hDC, This->hRC );

    This->flipEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    while(This->dRun)
    {
        WaitForSingleObject(This->flipEvent, INFINITE);
        ResetEvent(This->flipEvent);

        /* convert ddraw surface to opengl texture */
        for(i=0; i<This->height; i++)
        {
            for(j=0; j<This->width; j++)
            {
                This->glTex[i*This->width+j] = This->palette->data[((unsigned char *)This->surface)[i*This->lPitch + j*This->lXPitch]];
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, This->width, This->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, This->glTex);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

        glEnable(GL_TEXTURE_2D);

        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0,0); glVertex2f(-1,  1);
        glTexCoord2f(1,0); glVertex2f( 1,  1);
        glTexCoord2f(1,1); glVertex2f( 1, -1);	
        glTexCoord2f(0,1); glVertex2f(-1, -1);
        glEnd();

        SwapBuffers(This->hDC);
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(This->hRC);
    ReleaseDC(This->hWnd, This->hDC);

    return 0;
}

#else // if USE_OPENGL

DWORD WINAPI dd_Thread(IDirectDrawSurfaceImpl *This)
{
    int i,j;
    DDSURFACEDESC ddsd;
    LPDIRECTDRAWSURFACE primary;
    LPDIRECTDRAWCLIPPER clipper;
    DWORD width;
    POINT pt;

    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    IDirectDraw_CreateSurface(This->parent->real_ddraw, &ddsd, &primary, NULL);

    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    IDirectDrawSurface_GetSurfaceDesc(primary, &ddsd);

    width = ddsd.dwWidth * ddsd.lPitch / (ddsd.dwWidth * ddsd.ddpfPixelFormat.dwRGBBitCount / 8);

    IDirectDraw_CreateClipper(This->parent->real_ddraw, 0, &clipper, NULL);
    IDirectDrawClipper_SetHWnd(clipper, 0, This->hWnd);
    IDirectDrawSurface_SetClipper(primary, clipper);

    This->flipEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    while(This->dRun)
    {
        WaitForSingleObject(This->flipEvent, INFINITE);

        if(!This->dRun)
            break;

        pt.x = pt.y = 0;
        //ClientToScreen(This->hWnd, &pt);

        if(IDirectDrawSurface_Lock(primary, NULL, &ddsd, DDLOCK_WRITEONLY|DDLOCK_WAIT, NULL) != DD_OK)
            continue;

        /* convert ddraw surface to opengl texture */
        for(i=0; i<This->height; i++)
        {
            for(j=0; j<This->width; j++)
            {
                ((int *)ddsd.lpSurface)[(i+pt.y)*width+(j+pt.x)] = This->palette->data[((unsigned char *)This->surface)[i*This->lPitch + j*This->lXPitch]];
            }
        }

        IDirectDrawSurface_Unlock(primary, NULL);

        ResetEvent(This->flipEvent);
    }

    IDirectDrawClipper_Release(clipper);
    IDirectDrawSurface_Release(primary);

    return 0;
}

#endif

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
