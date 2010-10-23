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

/* This is a special mouse coordinate fix for games that use GetCursorPos and expect to be in fullscreen */

#include <windows.h>
#include <stdio.h>
#include "main.h"
#include "surface.h"

BOOL WINAPI fake_GetCursorPos(LPPOINT lpPoint)
{
    lpPoint->x = ddraw->cursor.x;
    lpPoint->y = ddraw->cursor.y;
    return TRUE;
}

void mouse_lock()
{
    if(!ddraw->locked)
    {
        ddraw->locked = TRUE;
        ClipCursor(&ddraw->cursorclip);
        while(ShowCursor(FALSE) > 0);
        SetEvent(ddraw_primary->flipEvent);
    }
}

void mouse_unlock()
{
    if(ddraw->locked)
    {
        ShowCursor(TRUE);
    }

    ddraw->locked = FALSE;
    ClipCursor(NULL);
    ddraw->cursor.x = ddraw->width / 2;
    ddraw->cursor.y = ddraw->height / 2;

    if(ddraw_primary)
    {
        SetEvent(ddraw_primary->flipEvent);
    }
}

void mouse_init(HWND hWnd)
{
    DWORD tmp;
    HANDLE hProcess;
    DWORD dwWritten;

    GetWindowThreadProcessId(hWnd, &tmp);
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tmp);

    tmp = (DWORD)fake_GetCursorPos;

    // Command & Conquer
    //WriteProcessMemory(hProcess, (void *)0x005B0184, &tmp, 4, &dwWritten);

    // Red Alert
    //WriteProcessMemory(hProcess, (void *)0x005E6848, &tmp, 4, &dwWritten);
}
