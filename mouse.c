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
            0x004C88CC, /* ClipCursor */
            0x004C88E0, /* ClipCursor */
            0x0041114F, /* ShowCursor */
            0x00411198, /* ShowCursor */
            0x00454490, /* ShowCursor */
            0x004CCE62, /* SetCursor */
            0x00000000
        },
    },
    /* Red Alert */
    {
        0x005B39C0,
        0x005E6848,
        {
            0x005C194D, /* ClipCursor */
            0x005C196D, /* ClipCursor */
            0x004F83A0, /* ShowCursor */
            0x005B3A26, /* ShowCursor */
            0x005B3A73, /* ShowCursor */
            0x005A02CD, /* SetCursor */
            0x005A030A, /* SetCursor */
            0x005A0324, /* SetCursor */
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
    lpPoint->x = (int)ddraw->cursor.x;
    lpPoint->y = (int)ddraw->cursor.y;
    return TRUE;
}

BOOL mouse_active = FALSE;

void mouse_lock()
{
    if(mouse_active && !ddraw->locked)
    {
        ddraw->locked = TRUE;
        ClipCursor(&ddraw->cursorclip);
        while(ShowCursor(FALSE) > 0);
    }
}

void mouse_unlock()
{
    if(!mouse_active)
    {
        return;
    }

    if(ddraw->locked)
    {
        ShowCursor(TRUE);
    }

    ddraw->locked = FALSE;
    ClipCursor(NULL);
    ddraw->cursor.x = ddraw->width / 2;
    ddraw->cursor.y = ddraw->height / 2;
}

void mouse_init(HWND hWnd)
{
    DWORD tmp;
    HANDLE hProcess;
    DWORD dwWritten;
    int i;

    unsigned char buf[7];

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

            memset(buf, 0x90, 7);   // NOP
            buf[0] = 0x58;          // POP EAX
            buf[1] = 0x33;          // XOR EAX,EAX
            buf[2] = 0xC0;          // ^
            for(i=0;i<MAX_NOPS;i++)
            {
                if(!ptr->nops[i])
                    break;

                WriteProcessMemory(hProcess, (void *)ptr->nops[i], buf, 7, &dwWritten);
            }

            mouse_active = TRUE;
            return;
        }
        ptr++;
    }
}
