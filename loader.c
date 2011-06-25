/*
 * Copyright (c) 2010, 2011 Toni Spets <toni.spets@iki.fi>
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

#include "loader.h"

#include <windows.h>

void loader(struct iat_table *hck)
{
    int i;
    char buf[32];
    struct iat_hook *hk;
    DWORD dwWritten;
    IMAGE_DOS_HEADER dos_hdr;
    IMAGE_NT_HEADERS nt_hdr;
    IMAGE_IMPORT_DESCRIPTOR *dir;
    IMAGE_THUNK_DATA thunk;
    PDWORD ptmp;

    HMODULE base = GetModuleHandle(NULL);
    HANDLE hProcess = GetCurrentProcess();

    ReadProcessMemory(hProcess, (void *)base, &dos_hdr, sizeof(IMAGE_DOS_HEADER), &dwWritten);
    ReadProcessMemory(hProcess, (void *)base+dos_hdr.e_lfanew, &nt_hdr, sizeof(IMAGE_NT_HEADERS), &dwWritten);
    dir = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (nt_hdr.OptionalHeader.DataDirectory[1].Size));
    ReadProcessMemory(hProcess, (void *)base+nt_hdr.OptionalHeader.DataDirectory[1].VirtualAddress, dir, nt_hdr.OptionalHeader.DataDirectory[1].Size, &dwWritten);

    while(dir->Name > 0)
    {
        memset(buf, 0, 32);
        ReadProcessMemory(hProcess, (void *)base+dir->Name, buf, 32, &dwWritten);
        if(stricmp(buf, hck->name) == 0)
        {
            ptmp = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DWORD) * 64);
            ReadProcessMemory(hProcess, (void *)base+dir->Characteristics, ptmp, sizeof(DWORD) * 64, &dwWritten);
            i=0;
            while(*ptmp)
            {
                memset(buf, 0, 32);
                ReadProcessMemory(hProcess, (void *)base+(*ptmp)+2, buf, 32, &dwWritten);

                hk = &hck->hooks[0];
                while(hk->func)
                {
                    if ((*ptmp & 0x80000000 && (*ptmp & ~0x80000000) == hk->ord) || (hk->ord == 0 && stricmp(hk->name, buf) == 0))
                    {
                        thunk.u1.Function = (DWORD)hk->func;
                        thunk.u1.Ordinal = (DWORD)hk->func;
                        thunk.u1.AddressOfData = (DWORD)hk->func;
                        VirtualProtectEx(hProcess, (void *)base+dir->FirstThunk+(sizeof(IMAGE_THUNK_DATA) * i), sizeof(IMAGE_THUNK_DATA), PAGE_EXECUTE_READWRITE, &dwWritten);
                        WriteProcessMemory(hProcess, (void *)base+dir->FirstThunk+(sizeof(IMAGE_THUNK_DATA) * i), &thunk, sizeof(IMAGE_THUNK_DATA), &dwWritten);
                    }
                    hk++;
                }

                ptmp++;
                i++;
            }
        }
        dir++;
    }

    CloseHandle(hProcess);
}
