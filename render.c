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

/*
Edits by Ben Lankamp

Added pillar box rendering for OpenGL
*/
#include <windows.h>
#include <stdio.h>

#include "main.h"
#include "surface.h"

#define CUTSCENE_WIDTH 640
#define CUTSCENE_HEIGHT 400

#define ASPECT_RATIO 4/3

BOOL detect_cutscene();

DWORD WINAPI render_main(void)
{
    int i,j,prevRow,nextRow;
    HGLRC hRC;

    // fixed: texture not square but ASPECT RATIO scaled
    int tex_width = ddraw->width > 1024 ? ddraw->width : 1024;
    int tex_height = ddraw->height > 768 ? ddraw->height : 768;
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

    // define screen width and height
    // define projected width and height (aspect ratio preserved)
    int screenWidth = ddraw->render.width;
    int screenHeight = ddraw->render.height;

    // define projection width and height depending on the aspect ratio
    // this effectively sets a pillar box view
    int projectedHeight = screenHeight;
    int projectedWidth = projectedHeight * ASPECT_RATIO;
    int projectedLeft, projectedTop;

    if(ddraw->boxing)
    {
        projectedLeft = (screenWidth - projectedWidth) / 2;
        projectedTop = (screenHeight - projectedHeight) / 2;

        glViewport(projectedLeft, projectedTop, projectedWidth, projectedHeight);
    }
    else
    {
        projectedWidth = screenWidth;
        projectedHeight = screenHeight;
        glViewport(0, 0, screenWidth, screenHeight);
    }

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
	
	timeBeginPeriod(1);
	


    while(ddraw->render.run && WaitForSingleObject(ddraw->render.sem, INFINITE) != WAIT_FAILED)
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
                scale_w *= (float)CUTSCENE_WIDTH / ddraw->width;
                scale_h *= (float)CUTSCENE_HEIGHT / ddraw->height;

                if (ddraw->cursorclip.width != CUTSCENE_WIDTH || ddraw->cursorclip.height != CUTSCENE_HEIGHT)
                {
                    ddraw->cursorclip.width = CUTSCENE_WIDTH;
                    ddraw->cursorclip.height = CUTSCENE_HEIGHT;
                    ddraw->cursor.x = CUTSCENE_WIDTH / 2;
                    ddraw->cursor.y = CUTSCENE_HEIGHT / 2;
                }
            }
            else
            {
                if (ddraw->cursorclip.width != ddraw->width || ddraw->cursorclip.height != ddraw->height)
                {
                    ddraw->cursorclip.width = ddraw->width;
                    ddraw->cursorclip.height = ddraw->height;
                    ddraw->cursor.x = ddraw->width / 2;
                    ddraw->cursor.y = ddraw->height / 2;
                }
            }

            // regular paint
            for(i=0; i<ddraw->height; i++)
            {
                for(j=0; j<ddraw->width; j++)
                {
                    tex[i*ddraw->width+j] = ddraw->primary->palette->data_bgr[((unsigned char *)ddraw->primary->surface)[i*ddraw->primary->lPitch + j*ddraw->primary->lXPitch]];
                }
            }

            // poor man's deinterlace
            if(ddraw->vhack && detect_cutscene())
            {
                for(i = 1; i < (ddraw->height - 1); i += 2)
                {
                    for(j=0; j<ddraw->width; j++)
                    {
                        if(tex[i*ddraw->width+j] == 0)
                        {
                            prevRow = tex[(i-1)*ddraw->width+j];
                            nextRow = tex[(i+1)*ddraw->width+j];

                            tex[i*ddraw->width+j] = (prevRow+nextRow)/2;
                        }
                    }
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

        if((ddraw->render.maxfps > 0))
        {        
			tick_end = GetTickCount();
			
           if(tick_end - tick_start < frame_len)
           {
				Sleep( frame_len - (tick_end - tick_start));
            }
        }

        SetEvent(ddraw->render.ev);
    }
	timeEndPeriod(1);
		
    HeapFree(GetProcessHeap(), 0, tex);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);

    return 0;
}
