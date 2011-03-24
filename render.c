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

BOOL detect_cutscene();

DWORD WINAPI render_main(void)
{
    int i,j;
    HGLRC hRC;

    int tex_width = ddraw->render.width > 1024 ? ddraw->render.width : 1024;
    int tex_height = ddraw->render.height > 1024 ? ddraw->render.height : 1024;
    float scale_w = 1.0f;
    float scale_h = 1.0f;
    int *tex = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, tex_width * tex_height * sizeof(int));

    hRC = wglCreateContext( ddraw->render.hDC );
    wglMakeCurrent( ddraw->render.hDC, hRC );

    char *glext = (char *)glGetString(GL_EXTENSIONS);

    if(glext && strstr(glext, "WGL_EXT_swap_control"))
    {
        BOOL (APIENTRY *wglSwapIntervalEXT)(int) = (BOOL (APIENTRY *)(int))wglGetProcAddress("wglSwapIntervalEXT");
        if(wglSwapIntervalEXT)
        {
            if(ddraw->vsync)
            {
                wglSwapIntervalEXT(1);
            }
            else
            {
                wglSwapIntervalEXT(0);
            }
        }
    }

    DWORD tick_start = 0;
    DWORD tick_end = 0;
    DWORD frame_len = 0;

    if(ddraw->render.maxfps < 0)
    {
        ddraw->render.maxfps = ddraw->mode.dmDisplayFrequency;
    }

    if(ddraw->render.maxfps > 0)
    {
        frame_len = 1000.0f / ddraw->render.maxfps;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    glViewport(0, 0, ddraw->render.width, ddraw->render.height);

    if(ddraw->render.filter)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    }

    glEnable(GL_TEXTURE_2D);

    while(ddraw->render.run)
    {
        scale_w = (float)ddraw->width/tex_width;
        scale_h = (float)ddraw->height/tex_height;

        if(ddraw->render.maxfps > 0)
        {
            tick_start = GetTickCount();
        }

        /* convert ddraw surface to opengl texture */
        EnterCriticalSection(&ddraw->cs);
        if(ddraw->primary && ddraw->primary->palette)
        {
            if(ddraw->vhack && detect_cutscene())
            {
                scale_w *= 640.0f / ddraw->width;
                scale_h *= 400.0f / ddraw->height;
            }

            for(i=0; i<ddraw->height; i++)
            {
                for(j=0; j<ddraw->width; j++)
                {
                    tex[i*ddraw->width+j] = ddraw->primary->palette->data_bgr[((unsigned char *)ddraw->primary->surface)[i*ddraw->primary->lPitch + j*ddraw->primary->lXPitch]];
                }
            }
        }
        LeaveCriticalSection(&ddraw->cs);

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ddraw->width, ddraw->height, GL_RGBA, GL_UNSIGNED_BYTE, tex);

        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0,0);              glVertex2f(-1,  1);
        glTexCoord2f(scale_w,0);        glVertex2f( 1,  1);
        glTexCoord2f(scale_w,scale_h);  glVertex2f( 1, -1);	
        glTexCoord2f(0,scale_h);        glVertex2f(-1, -1);
        glEnd();

        SwapBuffers(ddraw->render.hDC);

        if(ddraw->render.maxfps > 0)
        {
            tick_end = GetTickCount();

            if(tick_end - tick_start < frame_len)
            {
                Sleep( frame_len - (tick_end - tick_start) );
            }
        }

        SetEvent(ddraw->render.ev);
    }

    HeapFree(GetProcessHeap(), 0, tex);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);

    return 0;
}

static unsigned char getPixel(int x, int y)
{
    return ((unsigned char *)ddraw->primary->surface)[y*ddraw->primary->lPitch + x*ddraw->primary->lXPitch];
}

BOOL detect_cutscene()
{
    if(ddraw->width <= 640 || ddraw->height <= 480)
        return FALSE;

    return getPixel(641, 0) == 0 || getPixel(645, 1) == 0 ? TRUE : FALSE;
}
