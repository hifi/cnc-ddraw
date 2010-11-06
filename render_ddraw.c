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

struct render_ddraw_impl
{
    DWORD WINAPI (*main)(IDirectDrawSurfaceImpl *surface);
    HRESULT WINAPI (*Initialize)(void);
    HRESULT WINAPI (*SetDisplayMode)(DWORD width, DWORD height);
    HRESULT WINAPI (*RestoreDisplayMode)(void);

    int maxfps;
    int width;
    int height;
    int filter;

    HANDLE thread;
    BOOL run;
    HANDLE ev;

    LPDIRECTDRAW ddraw;
};

DWORD WINAPI render_ddraw_main(IDirectDrawSurfaceImpl *surface);
HRESULT WINAPI render_ddraw_Initialize();
HRESULT WINAPI render_ddraw_SetDisplayMode(DWORD width, DWORD height);
HRESULT WINAPI render_ddraw_RestoreDisplayMode(void);

struct render_ddraw_impl render_ddraw =
{
    render_ddraw_main,
    render_ddraw_Initialize,
    render_ddraw_SetDisplayMode,
    render_ddraw_RestoreDisplayMode,

    0,
    0,
    0,
    0,

    NULL,
    TRUE,
    NULL,

    NULL
};

HRESULT WINAPI render_ddraw_Initialize()
{
    if(ddraw->DirectDrawCreate(NULL, &render_ddraw.ddraw, NULL) != DD_OK)
    {
        return DDERR_GENERIC;
    }

    return DD_OK;
}

HRESULT WINAPI render_ddraw_SetDisplayMode(DWORD width, DWORD height)
{
    if(ddraw->windowed == FALSE)
    {
        return IDirectDraw_SetDisplayMode(render_ddraw.ddraw, width, height, 24);
    }
    return DD_OK;
}

HRESULT WINAPI render_ddraw_RestoreDisplayMode(void)
{
    if(ddraw->windowed == FALSE)
    {
        return IDirectDraw_RestoreDisplayMode(render_ddraw.ddraw);
    }
    return DD_OK;
}

DWORD WINAPI render_ddraw_main(IDirectDrawSurfaceImpl *surface)
{
    int i,j;
    DDSURFACEDESC ddsd;
    LPDIRECTDRAWSURFACE primary;
    LPDIRECTDRAWCLIPPER clipper;
    DWORD width;

    if(render_ddraw.ddraw == NULL)
    {
        return FALSE;
    }

    render_ddraw.run = TRUE;

    if(ddraw->windowed)
    {
        IDirectDraw_SetCooperativeLevel(render_ddraw.ddraw, ddraw->hWnd, DDSCL_NORMAL);
    }
    else
    {
        IDirectDraw_SetCooperativeLevel(render_ddraw.ddraw, ddraw->hWnd, DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN);
    }

    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    IDirectDraw_CreateSurface(render_ddraw.ddraw, &ddsd, &primary, NULL);

    memset(&ddsd, 0, sizeof(DDSURFACEDESC));
    ddsd.dwSize = sizeof(DDSURFACEDESC);
    IDirectDrawSurface_GetSurfaceDesc(primary, &ddsd);

    width = ddsd.dwWidth * ddsd.lPitch / (ddsd.dwWidth * ddsd.ddpfPixelFormat.dwRGBBitCount / 8);

    IDirectDraw_CreateClipper(render_ddraw.ddraw, 0, &clipper, NULL);
    IDirectDrawClipper_SetHWnd(clipper, 0, ddraw->hWnd);
    IDirectDrawSurface_SetClipper(primary, clipper);

    DWORD tick_start;
    DWORD tick_end;
    DWORD frame_len;

    if(render_ddraw.maxfps < 0)
    {
        render_ddraw.maxfps = ddraw->freq;
    }

    if(render_ddraw.maxfps > 0)
    {
        frame_len = 1000.0f / render_ddraw.maxfps;
    }

    render_ddraw.ev = CreateEvent(NULL, TRUE, FALSE, NULL);

    while(render_ddraw.run)
    {
        ResetEvent(render_ddraw.ev);

        if(render_ddraw.maxfps > 0)
        {
            tick_start = GetTickCount();
        }

        IDirectDrawSurface_Lock(primary, NULL, &ddsd, DDLOCK_WRITEONLY|DDLOCK_WAIT, NULL);

        if(surface->palette)
        {
            for(i=0; i<surface->height; i++)
            {
                for(j=0; j<surface->width; j++)
                {
                    ((int *)ddsd.lpSurface)[(i+ddraw->winpos.y)*width+(j+ddraw->winpos.x)] = surface->palette->data_rgb[((unsigned char *)surface->surface)[i*surface->lPitch + j*surface->lXPitch]];
                }
            }
        }

        IDirectDrawSurface_Unlock(primary, NULL);

        if(render_ddraw.maxfps > 0)
        {
            tick_end = GetTickCount();

            if(tick_end - tick_start < frame_len)
            {
                Sleep( frame_len - (tick_end - tick_start) );
            }
        }

        SetEvent(render_ddraw.ev);
    }

    CloseHandle(render_ddraw.ev);

    IDirectDrawClipper_Release(clipper);
    IDirectDrawSurface_Release(primary);
    IDirectDraw_Release(render_ddraw.ddraw);

    return 0;
}
