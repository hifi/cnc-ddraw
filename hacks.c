#include "main.h"
#include "loader.h"

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
        return (SHORT)ddraw->ldown;
    }

    if (vKey == VK_RBUTTON)
    {
        return (SHORT)ddraw->rdown;
    }

    return 0;
}

BOOL WINAPI fake_ClipCursor(const RECT *lpRect)
{
    if (lpRect)
    {
        //SDL_WM_GrabInput(SDL_GRAB_ON);
    }
    else
    {
        //SDL_WM_GrabInput(SDL_GRAB_OFF);
    }
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
    printf("ShowWindow captured\n");
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
