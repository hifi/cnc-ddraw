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
#include "surface.h"

HRESULT __stdcall ddraw_palette_GetEntries(IDirectDrawPaletteImpl *This, DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries)
{
    printf("DirectDrawPalette::GetEntries(This=%p, dwFlags=%d, dwBase=%d, dwNumEntries=%d, lpEntries=%p)\n", This, (int)dwFlags, (int)dwBase, (int)dwNumEntries, lpEntries);
    return DD_OK;
}

HRESULT __stdcall ddraw_palette_SetEntries(IDirectDrawPaletteImpl *This, DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries)
{
#if _DEBUG
    printf("DirectDrawPalette::SetEntries(This=%p, dwFlags=%d, dwStartingEntry=%d, dwCount=%d, lpEntries=%p)\n", This, (int)dwFlags, (int)dwStartingEntry, (int)dwCount, lpEntries);
#endif

    if (ddraw->primary && ddraw->primary->surface)
        SDL_SetPalette(ddraw->primary->surface, SDL_LOGPAL, (SDL_Color *)lpEntries, dwStartingEntry, dwCount);

    return DD_OK;
}

HRESULT __stdcall ddraw_palette_QueryInterface(IDirectDrawPaletteImpl *This, REFIID riid, void **obj)
{
    printf("DirectDrawPalette::QueryInterface(This=%p, riid=%08X, obj=%p)\n", This, (unsigned int)riid, obj);
    return S_OK;
}

ULONG __stdcall ddraw_palette_AddRef(IDirectDrawPaletteImpl *This)
{
    printf("DirectDrawPalette::AddRef(This=%p)\n", This);

    This->Ref++;

    return This->Ref;
}

ULONG __stdcall ddraw_palette_Release(IDirectDrawPaletteImpl *This)
{
    printf("DirectDrawPalette::Release(This=%p)\n", This);

    This->Ref--;

    if(This->Ref == 0)
    {
        HeapFree(GetProcessHeap(), 0, This);
        return 0;
    }

    return This->Ref;
}

HRESULT __stdcall ddraw_palette_GetCaps(IDirectDrawPaletteImpl *This, LPDWORD caps)
{
    printf("DirectDrawPalette::GetCaps(This=%p, caps=%p)\n", This, caps);
    return DD_OK;
}

HRESULT __stdcall ddraw_palette_Initialize(IDirectDrawPaletteImpl *This, LPDIRECTDRAW lpDD, DWORD dw, LPPALETTEENTRY paent)
{
    printf("DirectDrawPalette::Initialize(This=%p, ...)\n", This);
    return DD_OK;
}

struct IDirectDrawPaletteImplVtbl piface =
{
    /* IUnknown */
    ddraw_palette_QueryInterface,
    ddraw_palette_AddRef,
    ddraw_palette_Release,
    /* IDirectDrawPalette */
    ddraw_palette_GetCaps,
    ddraw_palette_GetEntries,
    ddraw_palette_Initialize,
    ddraw_palette_SetEntries
};

HRESULT __stdcall ddraw_CreatePalette(IDirectDrawImpl *This, DWORD dwFlags, LPPALETTEENTRY lpDDColorArray, LPDIRECTDRAWPALETTE FAR * lpDDPalette, IUnknown FAR * unkOuter)
{
    printf("DirectDraw::CreatePalette(This=%p, dwFlags=%d, DDColorArray=%p, DDPalette=%p, unkOuter=%p)\n", This, (int)dwFlags, lpDDColorArray, lpDDPalette, unkOuter);

    IDirectDrawPaletteImpl *Palette = (IDirectDrawPaletteImpl *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(IDirectDrawPaletteImpl));
    Palette->lpVtbl = &piface;
    printf(" Palette = %p\n", Palette);
    *lpDDPalette = (LPDIRECTDRAWPALETTE)Palette;

    Palette->data_rgb = NULL;

    ddraw_palette_SetEntries(Palette, dwFlags, 0, 256, lpDDColorArray);

    ddraw_palette_AddRef(Palette);

    return DD_OK;
}

