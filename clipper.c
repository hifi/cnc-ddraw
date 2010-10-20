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
#include "clipper.h"

/* from main */
HRESULT null();

HRESULT ddraw_clipper_QueryInterface(void *This, REFIID riid, void **obj)
{
    printf("DirectDrawClipper::QueryInterface(This=%p, riid=%08X, obj=%p)\n", This, (unsigned int)riid, obj);
    return S_OK;
}

ULONG ddraw_clipper_AddRef(void *_This)
{
    fakeDirectDrawClipperObject *This = (fakeDirectDrawClipperObject *)_This;

    printf("DirectDrawClipper::AddRef(This=%p)\n", This);

    This->Ref++;

    return This->Ref;
}

ULONG ddraw_clipper_Release(void *_This)
{
    fakeDirectDrawClipperObject *This = (fakeDirectDrawClipperObject *)_This;

    printf("DirectDrawClipper::Release(This=%p)\n", This);

    This->Ref--;

    if(This->Ref == 0)
    {
        free(This);
        return 0;
    }

    return This->Ref;
}

fakeDirectDrawClipper ciface =
{
    /* IUnknown */
    ddraw_clipper_QueryInterface,
    ddraw_clipper_AddRef,
    ddraw_clipper_Release,
    /* IDirectDrawClipper */
    null, // ddraw_clipper_GetClipList
    null, // ddraw_clipper_GetHWnd
    null, // ddraw_clipper_Initialize
    null, // ddraw_clipper_IsClipListChanged
    null, // ddraw_clipper_SetClipList
    null  // ddraw_clipper_SetHwnd
};

HRESULT ddraw_CreateClipper(void *_This, DWORD dwFlags, LPDIRECTDRAWCLIPPER FAR *lplpDDClipper, IUnknown FAR *pUnkOuter )
{
    fakeDirectDrawClipperObject *This = (fakeDirectDrawClipperObject *)_This;

    printf("DirectDraw::CreateClipper(This=%p, dwFlags=%d, DDClipper=%p, unkOuter=%p)\n", This, (int)dwFlags, lplpDDClipper, pUnkOuter);

    fakeDirectDrawClipperObject *Clipper = (fakeDirectDrawClipperObject *)malloc(sizeof(fakeDirectDrawClipperObject));
    Clipper->Functions = &ciface;
    printf(" Clipper = %p\n", Clipper);
    *lplpDDClipper = (LPDIRECTDRAWCLIPPER)Clipper;

    Clipper->Ref = 0;
    ddraw_clipper_AddRef(Clipper);

    return DD_OK;
}
