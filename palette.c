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
#include "palette.h"

/* from main */
HRESULT QueryInterface(void *This, REFIID riid, void **obj);
ULONG AddRef(void *This);
ULONG Release(void *This);
HRESULT null();

HRESULT ddraw_palette_SetEntries(void *_This, DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries);

HRESULT ddraw_CreatePalette(void *_This, DWORD dwFlags, LPPALETTEENTRY lpDDColorArray, LPDIRECTDRAWPALETTE FAR * lpDDPalette, IUnknown FAR * unkOuter)
{
    fakeDirectDrawPaletteObject *This = (fakeDirectDrawPaletteObject *)_This;

    printf("DirectDraw::CreatePalette(This=%p, dwFlags=%d, DDColorArray=%p, DDPalette=%p, unkOuter=%p)\n", This, (int)dwFlags, lpDDColorArray, lpDDPalette, unkOuter);

    fakeDirectDrawPaletteObject *Palette = (fakeDirectDrawPaletteObject *)malloc(sizeof(fakeDirectDrawPaletteObject));
    Palette->Ref = 1;
    Palette->Functions = &piface;
    printf(" Palette = %p\n", Palette);
    *lpDDPalette = (LPDIRECTDRAWPALETTE)Palette;

    ddraw_palette_SetEntries(Palette, dwFlags, 0, 256, lpDDColorArray);

    return DD_OK;
}

HRESULT ddraw_palette_GetEntries(void *This, DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries)
{
    printf("DirectDrawPalette::GetEntries(This=%p, dwFlags=%d, dwBase=%d, dwNumEntries=%d, lpEntries=%p)\n", This, (int)dwFlags, (int)dwBase, (int)dwNumEntries, lpEntries);
    return DD_OK;
}

HRESULT ddraw_palette_SetEntries(void *_This, DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries)
{
    int i;

    fakeDirectDrawPaletteObject *This = (fakeDirectDrawPaletteObject *)_This;

#if _DEBUG
    printf("DirectDrawPalette::SetEntries(This=%p, dwFlags=%d, dwStartingEntry=%d, dwCount=%d, lpEntries=%p)\n", This, (int)dwFlags, (int)dwStartingEntry, (int)dwCount, lpEntries);
#endif

    for(i=0;i<256;i++)
    {
        This->data[i] = (lpEntries[i].peBlue<<16)|(lpEntries[i].peGreen<<8)|lpEntries[i].peRed;
    }

    return DD_OK;
}

fakeDirectDrawPalette piface =
{
    /* IUnknown */
    QueryInterface,
    AddRef,
    Release,
    /* IDirectDrawPalette */
    null, // ddraw_palette_GetCaps
    ddraw_palette_GetEntries,
    null, // ddraw_palette_Initialize
    ddraw_palette_SetEntries
};
