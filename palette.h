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

#ifndef PALETTE_H
#define PALETTE_H

#include <windows.h>
#include "ddraw.h"

typedef struct
{
    /* IUnknown */
    HRESULT (*QueryInterface)(void *, REFIID, void **);
    ULONG (*AddRef)(void *);
    ULONG (*Release)(void *);

    /* IDirectDrawPalette */
    HRESULT (*GetCaps)(void *, LPDWORD);
    HRESULT (*GetEntries)(void *, DWORD, DWORD, DWORD, LPPALETTEENTRY);
    HRESULT (*Initialize)(void *, LPDIRECTDRAW, DWORD, LPPALETTEENTRY);
    HRESULT (*SetEntries)(void *, DWORD, DWORD, DWORD, LPPALETTEENTRY);
} fakeDirectDrawPalette;

typedef struct
{
    fakeDirectDrawPalette *Functions;

    int data[256];

    ULONG Ref;

} fakeDirectDrawPaletteObject;

extern fakeDirectDrawPalette piface;

HRESULT ddraw_CreatePalette(void *This, DWORD dwFlags, LPPALETTEENTRY DDColorArray, LPDIRECTDRAWPALETTE FAR * DDPalette, IUnknown FAR * unkOuter);

#endif
