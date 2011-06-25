#include "main.h"
#include "surface.h"

WPARAM key2win(SDLKey sym)
{
    switch (sym)
    {
        case SDLK_UP:       return VK_UP;
        case SDLK_DOWN:     return VK_DOWN;
        case SDLK_LEFT:     return VK_LEFT;
        case SDLK_RIGHT:    return VK_RIGHT;
        case SDLK_RETURN:   return VK_RETURN;
        case SDLK_ESCAPE:   return VK_ESCAPE;
        case SDLK_0:        return 0x30;
        case SDLK_1:        return 0x31;
        case SDLK_2:        return 0x32;
        case SDLK_3:        return 0x33;
        case SDLK_4:        return 0x34;
        case SDLK_5:        return 0x35;
        case SDLK_6:        return 0x36;
        case SDLK_7:        return 0x37;
        case SDLK_8:        return 0x38;
        case SDLK_9:        return 0x39;
        case SDLK_a:        return 0x41;
        case SDLK_b:        return 0x42;
        case SDLK_c:        return 0x43;
        case SDLK_d:        return 0x44;
        case SDLK_e:        return 0x45;
        case SDLK_f:        return 0x46;
        case SDLK_g:        return 0x47;
        case SDLK_h:        return 0x48;
        case SDLK_i:        return 0x49;
        case SDLK_j:        return 0x4A;
        case SDLK_k:        return 0x4B;
        case SDLK_l:        return 0x4C;
        case SDLK_m:        return 0x4D;
        case SDLK_n:        return 0x4E;
        case SDLK_o:        return 0x4F;
        case SDLK_p:        return 0x50;
        case SDLK_q:        return 0x51;
        case SDLK_r:        return 0x52;
        case SDLK_s:        return 0x53;
        case SDLK_t:        return 0x54;
        case SDLK_u:        return 0x55;
        case SDLK_v:        return 0x56;
        case SDLK_w:        return 0x57;
        case SDLK_x:        return 0x58;
        case SDLK_y:        return 0x59;
        case SDLK_z:        return 0x5A;
        default:            return 0;
    }
}

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
    //SDL_WM_GrabInput(SDL_GRAB_ON);

    SDL_AddTimer(1000.0f / 60, (SDL_NewTimerCallback)SDL_Renderer, NULL);

    while (SDL_WaitEvent(&ev))
    {
        switch (ev.type)
        {
            case SDL_MOUSEMOTION:
                ddraw->cursor.x = ev.motion.x;
                ddraw->cursor.y = ev.motion.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                ddraw->cursor.x = ev.button.x;
                ddraw->cursor.y = ev.button.y;

                if (ev.button.button == SDL_BUTTON_LEFT)
                {
                    ddraw->ldown = (ev.type == SDL_MOUSEBUTTONDOWN);
                    ddraw->WndProc(ddraw->hWnd, (ev.type == SDL_MOUSEBUTTONDOWN ? WM_LBUTTONDOWN : WM_LBUTTONUP), 0, MAKELPARAM(ddraw->cursor.x, ddraw->cursor.y));
                }
                else if (ev.button.button == SDL_BUTTON_RIGHT)
                {
                    ddraw->rdown = (ev.type == SDL_MOUSEBUTTONDOWN);
                    ddraw->WndProc(ddraw->hWnd, (ev.type == SDL_MOUSEBUTTONDOWN ? WM_RBUTTONDOWN : WM_RBUTTONUP), 0, MAKELPARAM(ddraw->cursor.x, ddraw->cursor.y));
                }
                break;
            case SDL_KEYDOWN:
                ddraw->WndProc(ddraw->hWnd, WM_KEYDOWN, key2win(ev.key.keysym.sym), 0);
                break;

            case SDL_KEYUP:
                ddraw->WndProc(ddraw->hWnd, WM_KEYUP, key2win(ev.key.keysym.sym), 0);
                break;

            case SDL_QUIT:
                ddraw->WndProc(ddraw->hWnd, WM_DESTROY, 0, 0);
                break;
        }
    }

    return 0;
}
