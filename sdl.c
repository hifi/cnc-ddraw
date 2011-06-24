#include "main.h"
#include "surface.h"

int SDL_Renderer(Uint32 interval, void *ptr)
{
    if (ddraw->primary)
    {
        SDL_BlitSurface(ddraw->primary->surface, NULL, ddraw->surface, NULL);
        SDL_Flip(ddraw->surface);
    }
    return interval;
}

int SDL_Main(IDirectDrawImpl *ddraw)
{
    SDL_Event ev;

    ddraw->surface = SDL_SetVideoMode(ddraw->width, ddraw->height, 16, SDL_HWSURFACE);

    SDL_AddTimer(1000.0f / 60, SDL_Renderer, NULL);

    while (SDL_WaitEvent(&ev))
    {
        if (ev.type == SDL_MOUSEMOTION)
        {
            ddraw->cursor.x = ev.motion.x;
            ddraw->cursor.y = ev.motion.y;
        }

        if (ev.type == SDL_MOUSEBUTTONDOWN)
        {
            ddraw->WndProc(ddraw->hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(ddraw->cursor.x, ddraw->cursor.y));
        }

        if (ev.type == SDL_MOUSEBUTTONUP)
        {
            ddraw->WndProc(ddraw->hWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(ddraw->cursor.x, ddraw->cursor.y));
        }

        if (ev.type == SDL_QUIT)
        {
            ddraw->WndProc(ddraw->hWnd, WM_DESTROY, 0, 0);
        }
    }

    return 0;
}
