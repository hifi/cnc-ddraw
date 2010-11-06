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

struct render_opengl_impl
{
    DWORD WINAPI (*main)(IDirectDrawSurfaceImpl *surface);
    HRESULT WINAPI (*Initialize)(void);
    HRESULT WINAPI (*SetDisplayMode)(DWORD width, DWORD height);
    HRESULT WINAPI (*RestoreDisplayMode)(void);

    HANDLE thread;
    BOOL run;
    HANDLE ev;
};

DWORD WINAPI render_opengl_main(IDirectDrawSurfaceImpl *surface);
HRESULT WINAPI render_opengl_Initialize();
HRESULT WINAPI render_opengl_SetDisplayMode(DWORD width, DWORD height);
HRESULT WINAPI render_opengl_RestoreDisplayMode(void);

struct render_opengl_impl render_opengl =
{
    render_opengl_main,
    render_opengl_Initialize,
    render_opengl_SetDisplayMode,
    render_opengl_RestoreDisplayMode,

    NULL,
    TRUE,
    NULL
};

HRESULT WINAPI render_opengl_Initialize()
{
    return DD_OK;
}

HRESULT WINAPI render_opengl_SetDisplayMode(DWORD width, DWORD height)
{
    return DD_OK;
}

HRESULT WINAPI render_opengl_RestoreDisplayMode(void)
{
    return DD_OK;
}

DWORD WINAPI render_opengl_main(IDirectDrawSurfaceImpl *surface)
{
    int i,j;
    PIXELFORMATDESCRIPTOR pfd;
    HDC hDC;
    HGLRC hRC;

    hDC = GetDC(ddraw->hWnd);

    int *glTex = malloc(surface->width * surface->height * sizeof(int));

    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    SetPixelFormat( hDC, ChoosePixelFormat( hDC, &pfd ), &pfd );

    hRC = wglCreateContext( hDC );
    wglMakeCurrent( hDC, hRC );

#ifdef FRAME_LIMIT
    DWORD tick_start;
    DWORD tick_end;
    DWORD frame_len = 1000.0f / ddraw->freq;
#endif

    render_opengl.ev = CreateEvent(NULL, TRUE, FALSE, NULL);

    while(render_opengl.run)
    {
#ifdef FRAME_LIMIT
        tick_start = GetTickCount();
#endif

        /* convert ddraw surface to opengl texture */
        for(i=0; i<surface->height; i++)
        {
            for(j=0; j<surface->width; j++)
            {
                glTex[i*surface->width+j] = surface->palette->data_bgr[((unsigned char *)surface->surface)[i*surface->lPitch + j*surface->lXPitch]];
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->width, surface->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, glTex);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

        glEnable(GL_TEXTURE_2D);

        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0,0); glVertex2f(-1,  1);
        glTexCoord2f(1,0); glVertex2f( 1,  1);
        glTexCoord2f(1,1); glVertex2f( 1, -1);	
        glTexCoord2f(0,1); glVertex2f(-1, -1);
        glEnd();

        SwapBuffers(hDC);

#ifdef FRAME_LIMIT
        tick_end = GetTickCount();

        if(tick_end - tick_start < frame_len)
        {
            Sleep( frame_len - (tick_end - tick_start) );
        }
#endif

        SetEvent(render_opengl.ev);
    }

    CloseHandle(render_opengl.ev);

    free(glTex);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(ddraw->hWnd, hDC);

    return 0;
}
