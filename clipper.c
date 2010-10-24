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

HRESULT __stdcall ddraw_clipper_QueryInterface(IDirectDrawClipperImpl *This, REFIID riid, void **obj)
{
    printf("DirectDrawClipper::QueryInterface(This=%p, riid=%08X, obj=%p)\n", This, (unsigned int)riid, obj);
    return S_OK;
}

ULONG __stdcall ddraw_clipper_AddRef(IDirectDrawClipperImpl *This)
{
    printf("DirectDrawClipper::AddRef(This=%p)\n", This);

    This->Ref++;

    return This->Ref;
}

ULONG __stdcall ddraw_clipper_Release(IDirectDrawClipperImpl *This)
{
    printf("DirectDrawClipper::Release(This=%p)\n", This);

    This->Ref--;

    if(This->Ref == 0)
    {
        free(This);
        return 0;
    }

    return This->Ref;
}

HRESULT __stdcall ddraw_clipper_GetClipList(IDirectDrawClipperImpl *This, LPRECT a, LPRGNDATA b, LPDWORD c)
{
    printf("IDirectDrawClipper::GetClipList(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_clipper_GetHWnd(IDirectDrawClipperImpl *This, HWND FAR *a)
{
    printf("IDirectDrawClipper::GetHWnd(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_clipper_Initialize(IDirectDrawClipperImpl *This, LPDIRECTDRAW a, DWORD b)
{
    printf("IDirectDrawClipper::Initialize(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_clipper_IsClipListChanged(IDirectDrawClipperImpl *This, BOOL FAR *a)
{
    printf("IDirectDrawClipper::IsClipListChanged(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_clipper_SetClipList(IDirectDrawClipperImpl *This, LPRGNDATA a, DWORD b)
{
    printf("IDirectDrawClipper::SetClipList(This=%p, ...)\n", This);
    return DD_OK;
}

HRESULT __stdcall ddraw_clipper_SetHWnd(IDirectDrawClipperImpl *This, DWORD a, HWND b)
{
    printf("IDirectDrawClipper::SetHWnd(This=%p, ...)\n", This);
    return DD_OK;
}

struct IDirectDrawClipperImplVtbl ciface =
{
    /* IUnknown */
    ddraw_clipper_QueryInterface,
    ddraw_clipper_AddRef,
    ddraw_clipper_Release,
    /* IDirectDrawClipper */
    ddraw_clipper_GetClipList,
    ddraw_clipper_GetHWnd,
    ddraw_clipper_Initialize,
    ddraw_clipper_IsClipListChanged,
    ddraw_clipper_SetClipList,
    ddraw_clipper_SetHWnd
};

HRESULT __stdcall ddraw_CreateClipper(IDirectDrawImpl *This, DWORD dwFlags, LPDIRECTDRAWCLIPPER FAR *lplpDDClipper, IUnknown FAR *pUnkOuter )
{
    printf("DirectDraw::CreateClipper(This=%p, dwFlags=%d, DDClipper=%p, unkOuter=%p)\n", This, (int)dwFlags, lplpDDClipper, pUnkOuter);

    IDirectDrawClipperImpl *Clipper = (IDirectDrawClipperImpl *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(IDirectDrawClipperImpl));
    Clipper->lpVtbl = &ciface;
    printf(" Clipper = %p\n", Clipper);
    *lplpDDClipper = (LPDIRECTDRAWCLIPPER)Clipper;

    ddraw_clipper_AddRef(Clipper);

    return DD_OK;
}
