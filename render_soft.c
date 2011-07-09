/*
 * Copyright (c) 2011 Toni Spets <toni.spets@iki.fi>
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

DWORD WINAPI render_soft_main(void)
{
    PBITMAPINFO bmi = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);

    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = ddraw->width;
    bmi->bmiHeader.biHeight = -ddraw->height;
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = ddraw->bpp;
    bmi->bmiHeader.biCompression = BI_RGB;

    DWORD dst_top = 0;
    DWORD dst_left = 0;
    DWORD dst_width = ddraw->render.width;
    DWORD dst_height = ddraw->render.height;

    DWORD tick_start = 0;
    DWORD tick_end = 0;
    DWORD frame_len = 0;

    if (ddraw->boxing)
    {
        dst_width = ddraw->width;
        dst_height = ddraw->height;

        /* test if we can double scale the window */
        if (ddraw->width * 2 <= ddraw->render.width && ddraw->height * 2 <= ddraw->render.height)
        {
            dst_width *= 2;
            dst_height *= 2;
        }

        dst_top = ddraw->render.height / 2 - dst_height / 2;
        dst_left = ddraw->render.width / 2 - dst_width / 2;
    }

    if(ddraw->render.maxfps < 0)
    {
        ddraw->render.maxfps = ddraw->mode.dmDisplayFrequency;
    }

    if(ddraw->render.maxfps > 0)
    {
        frame_len = 1000.0f / ddraw->render.maxfps;
    }

    while (ddraw->render.run && WaitForSingleObject(ddraw->render.sem, 100) != WAIT_FAILED)
    {
        if(ddraw->render.maxfps > 0)
        {
            tick_start = GetTickCount();
        }

        EnterCriticalSection(&ddraw->cs);

        if (ddraw->primary && (ddraw->primary->palette || ddraw->bpp == 16))
        {
            if (ddraw->primary->palette && ddraw->primary->palette->data_rgb == NULL)
            {
                ddraw->primary->palette->data_rgb = &bmi->bmiColors[0];
            }

            if (ddraw->render.width != ddraw->width || ddraw->render.height != ddraw->height)
            {
                StretchDIBits(ddraw->render.hDC, dst_left, dst_top, dst_width, dst_height, 0, 0, ddraw->width, ddraw->height, ddraw->primary->surface, bmi, DIB_RGB_COLORS, SRCCOPY);
            }
            else
            {
                SetDIBitsToDevice(ddraw->render.hDC, 0, 0, ddraw->width, ddraw->height, 0, 0, 0, ddraw->height, ddraw->primary->surface, bmi, DIB_RGB_COLORS);
            }
        }
        LeaveCriticalSection(&ddraw->cs);

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

    HeapFree(GetProcessHeap(), 0, bmi);

    return TRUE;
}
