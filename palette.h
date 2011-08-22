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
#include "main.h"

struct IDirectDrawPaletteImpl;
struct IDirectDrawPaletteImplVtbl;

typedef struct IDirectDrawPaletteImpl
{
    struct IDirectDrawPaletteImplVtbl *lpVtbl;

    ULONG Ref;

    int data_bgr[256];
    RGBQUAD *data_rgb;

} IDirectDrawPaletteImpl;

struct IDirectDrawPaletteImplVtbl
{
    /* IUnknown */
    HRESULT (__stdcall *QueryInterface)(IDirectDrawPaletteImpl*, REFIID, void**);
    ULONG (__stdcall *AddRef)(IDirectDrawPaletteImpl*);
    ULONG (__stdcall *Release)(IDirectDrawPaletteImpl*);

    /* IDirectDrawPalette */
    HRESULT (__stdcall *GetCaps)(IDirectDrawPaletteImpl*, LPDWORD);
    HRESULT (__stdcall *GetEntries)(IDirectDrawPaletteImpl*, DWORD, DWORD, DWORD, LPPALETTEENTRY);
    HRESULT (__stdcall *Initialize)(IDirectDrawPaletteImpl*, LPDIRECTDRAW, DWORD, LPPALETTEENTRY);
    HRESULT (__stdcall *SetEntries)(IDirectDrawPaletteImpl*, DWORD, DWORD, DWORD, LPPALETTEENTRY);

} IDirectDrawPaletteImplVtbl;

HRESULT __stdcall ddraw_CreatePalette(IDirectDrawImpl *This, DWORD dwFlags, LPPALETTEENTRY DDColorArray, LPDIRECTDRAWPALETTE FAR * DDPalette, IUnknown FAR * unkOuter);

#endif
