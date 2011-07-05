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
    HDC memDC = CreateCompatibleDC(ddraw->render.hDC);
    HBITMAP surface = CreateCompatibleBitmap(ddraw->render.hDC, ddraw->width, ddraw->height);

    PBITMAPINFO bmi = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * 256) + 1024);

    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = ddraw->width;
    bmi->bmiHeader.biHeight = -ddraw->height;
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = ddraw->bpp;
    bmi->bmiHeader.biCompression = BI_RGB;

    SelectObject(memDC, surface);

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

            SetDIBits(memDC, surface, 0, ddraw->height, ddraw->primary->surface, bmi, DIB_RGB_COLORS);

            if (ddraw->render.width != ddraw->width || ddraw->render.height != ddraw->height)
            {
                StretchBlt(ddraw->render.hDC, 0, 0, ddraw->render.width, ddraw->render.height, memDC, 0, 0, ddraw->width, ddraw->height, SRCCOPY);
            }
            else
            {
                BitBlt(ddraw->render.hDC, 0, 0, ddraw->width, ddraw->height, memDC, 0, 0, SRCCOPY);
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
    DeleteObject(surface);
    DeleteDC(memDC);
    return TRUE;
}
