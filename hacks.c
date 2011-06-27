#include "main.h"
#include "loader.h"
#include "sdl_keys.h"

BOOL WINAPI fake_GetCursorPos(LPPOINT lpPoint)
{
    lpPoint->x = (int)ddraw->cursor.x;
    lpPoint->y = (int)ddraw->cursor.y;
    return TRUE;
}

SHORT WINAPI fake_GetAsyncKeyState(int vKey)
{
    if (vKey == VK_LBUTTON)
    {
        return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1);
    }

    if (vKey == VK_RBUTTON)
    {
        return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(2);
    }

    if (vKey == VK_SHIFT)
    {
        return SDL_GetModState() & KMOD_SHIFT;
    }

    if (vKey == VK_CONTROL)
    {
        return SDL_GetModState() & KMOD_CTRL;
    }

    if (vKey == VK_MENU)
    {
        return SDL_GetModState() & KMOD_ALT;
    }

    if (vKey == VK_CAPITAL)
    {
        return SDL_GetModState() & KMOD_CAPS;
    }

    if (vKey == VK_NUMLOCK)
    {
        return SDL_GetModState() & KMOD_NUM;
    }

    Uint8 *keys = SDL_GetKeyState(NULL);

    return keys[VKey_to_SDLKey[vKey]];
}

SHORT WINAPI fake_GetKeyState(int vKey)
{
    return fake_GetAsyncKeyState(vKey);
}

BOOL WINAPI fake_ClipCursor(const RECT *lpRect)
{
    return TRUE;
}

int WINAPI fake_ShowCursor(BOOL bShow)
{
    SDL_ShowCursor(bShow);
    return TRUE;
}

HCURSOR WINAPI fake_SetCursor(HCURSOR hCursor)
{
    return NULL;
}

BOOL WINAPI fake_ShowWindow(HWND hWnd, int nCmdShow)
{
    ShowWindow(hWnd, SW_HIDE);
    return 0;
}

struct iat_table hacks_iat[] =
{
    {
        "user32.dll",
        {
            { 0, "GetCursorPos", fake_GetCursorPos },
            { 0, "ClipCursor", fake_ClipCursor },
            { 0, "ShowCursor", fake_ShowCursor },
            { 0, "SetCursor", fake_SetCursor } ,
            { 0, "GetAsyncKeyState", fake_GetAsyncKeyState } ,
            { 0, "ShowWindow", fake_ShowWindow} ,
            { 0, "GetKeyState", fake_GetKeyState } ,
            { 0, "", NULL }
        }
    },
    {
        "",
        {
            { 0, "", NULL }
        }
    }
};

void hacks()
{
    loader(&hacks_iat[0]);
}
