#include "main.h"
#include "surface.h"
#include "loader.h"
#include "sdl_keys.h"

int SDL_Renderer(IDirectDrawImpl *ddraw)
{
    while (ddraw->running && SDL_SemWait(ddraw->event) > -1)
    {
        if (ddraw->primary)
        {
            SDL_BlitSurface(ddraw->primary->surface, NULL, ddraw->surface, NULL);
            SDL_Flip(ddraw->surface);
        }
    }

    return 0;
}

int SDL_Main(IDirectDrawImpl *ddraw)
{
    SDL_Event ev;
    BOOL grabbed = FALSE;
    int vkey;

    ddraw->surface = SDL_SetVideoMode(ddraw->width, ddraw->height, ddraw->sdl_bpp, ddraw->sdl_flags);

    ddraw->event = SDL_CreateSemaphore(0);
    SDL_CreateThread((int (*)(void *))SDL_Renderer, ddraw);

    while (ddraw->running && SDL_WaitEvent(&ev))
    {
        switch (ev.type)
        {
            case SDL_MOUSEMOTION:
                ddraw->cursor.x = ev.motion.x;
                ddraw->cursor.y = ev.motion.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                if (!grabbed)
                {
                    grabbed = SDL_WM_GrabInput(SDL_GRAB_ON);
                }

                ddraw->cursor.x = ev.button.x;
                ddraw->cursor.y = ev.button.y;

                if (ev.button.button == SDL_BUTTON_LEFT)
                {
                    ddraw->WndProc(ddraw->hWnd, (ev.type == SDL_MOUSEBUTTONDOWN ? WM_LBUTTONDOWN : WM_LBUTTONUP), 0, MAKELPARAM(ddraw->cursor.x, ddraw->cursor.y));
                }
                else if (ev.button.button == SDL_BUTTON_RIGHT)
                {
                    ddraw->WndProc(ddraw->hWnd, (ev.type == SDL_MOUSEBUTTONDOWN ? WM_RBUTTONDOWN : WM_RBUTTONUP), 0, MAKELPARAM(ddraw->cursor.x, ddraw->cursor.y));
                }
                break;
            case SDL_KEYDOWN:

                /* remove input grab on alt-tab, second alt-tab should work */
                if (grabbed && ev.key.keysym.sym == SDLK_TAB && SDL_GetModState() & KMOD_ALT)
                {
                    grabbed = SDL_WM_GrabInput(SDL_GRAB_OFF);
                    break;
                }

                /* fullscreen toggle */
                if (grabbed && ev.key.keysym.sym == SDLK_RETURN && SDL_GetModState() & KMOD_ALT)
                {
                    ddraw->sdl_flags ^= SDL_FULLSCREEN;
                    ddraw->surface = SDL_SetVideoMode(ddraw->width, ddraw->height, ddraw->sdl_bpp, ddraw->sdl_flags);
                    if (ddraw->surface == NULL)
                    {
                        ddraw->sdl_flags ^= SDL_FULLSCREEN;
                        ddraw->surface = SDL_SetVideoMode(ddraw->width, ddraw->height, ddraw->sdl_bpp, ddraw->sdl_flags);
                    }
                    break;
                }

                vkey = SDLKey_to_VKey[ev.key.keysym.sym];
                if (vkey)
                    ddraw->WndProc(ddraw->hWnd, WM_KEYDOWN, vkey, 0);
                else
                    printf("Warning: SDLKey \"%s\" %d (0x%04X) was not translated\n", SDL_GetKeyName(ev.key.keysym.sym), ev.key.keysym.sym, ev.key.keysym.sym);
                break;

            case SDL_KEYUP:
                vkey = SDLKey_to_VKey[ev.key.keysym.sym];
                if (vkey)
                    ddraw->WndProc(ddraw->hWnd, WM_KEYUP, vkey, 0);
                break;

            case SDL_QUIT:
                ddraw->WndProc(ddraw->hWnd, WM_DESTROY, 0, 0);
                break;
        }
    }

    return 0;
}
