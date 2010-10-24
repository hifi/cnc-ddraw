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

#define MAX_NOPS 16

struct game
{
    DWORD check;            /* memory address of one GetCursorPos call (for game identifying, 2E FF 15) */
    DWORD hook;             /* memory address where the GetCursorPos call pointer is */
    DWORD nops[MAX_NOPS];   /* points where a 1+7 byte import call is NOP'd (ClipCursor, ShowCursor), first byte is a PUSH that has to be removed */
};

struct game games[] =
{
    /* Command & Conquer */
    {
        0x004C894A, /* address should contain 2E FF 15 */
        0x005B0184, /* GetCursorPos thunk addr */
        {
            0x004C88CB, /* ClipCursor */
            0x004C88DF, /* ClipCursor */
            0x0041114E, /* ShowCursor */
            0x00411197, /* ShowCursor */
            0x0045448F, /* ShowCursor */
            0x004CCE61, /* SetCursor */
            0x00000000
        },
    },
    /* Red Alert */
    {
        0x005B39C0,
        0x005E6848,
        {
            0x005C194C, /* ClipCursor */
            0x005C196C, /* ClipCursor */
            0x004F839F, /* ShowCursor */
            0x005B3A25, /* ShowCursor */
            0x005B3A72, /* ShowCursor */
            0x005A02CC, /* SetCursor */
            0x005A0309, /* SetCursor */
            0x005A0323, /* SetCursor */
            0x00000000
        },
    },
    {
        0x00000000,
        0x00000000,
        {
            0x00000000
        },
    },
};

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
    int i;

    unsigned char buf[8];

    GetWindowThreadProcessId(hWnd, &tmp);
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, tmp);

    tmp = (DWORD)fake_GetCursorPos;

    struct game *ptr = &games[0];

    while(ptr->check)
    {
        ReadProcessMemory(hProcess, (void *)ptr->check, buf, 3, &dwWritten);
        if(buf[0] == 0x2E && buf[1] == 0xFF && buf[2] == 0x15)
        {
            WriteProcessMemory(hProcess, (void *)ptr->hook, &tmp, 4, &dwWritten);

            memset(buf, 0x90, 8);
            for(i=0;i<MAX_NOPS;i++)
            {
                if(!ptr->nops[i])
                    break;

                WriteProcessMemory(hProcess, (void *)ptr->nops[i], buf, 8, &dwWritten);
            }

            return;
        }
        ptr++;
    }
}
