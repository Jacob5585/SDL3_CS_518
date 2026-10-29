#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "framerate.h"

const int width = 800;
const int height = 5300;

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {

        SDL_Window *win = nullptr;
        SDL_Renderer *rend = nullptr;

        if (SDL_CreateWindowAndRenderer(argv[0], width, height, 0, &win, &rend))
        {

            SDL_Event e;

            FrameRate fr;

            SDL_SetRenderDrawColor(rend, 0xdd, 0xbb, 0xff, SDL_ALPHA_OPAQUE);

            int cnt = 0;
            bool quit = false;
            while (!quit)
            {
                cnt++;
                while (SDL_PollEvent(&e))
                {
                    switch (e.type)
                    {
                    case SDL_EVENT_QUIT:
                        quit = true;
                        break;
                    case SDL_EVENT_KEY_DOWN:
                    {
                        SDL_KeyboardEvent k = e.key;
                        if (k.key == SDLK_ESCAPE)
                        {
                            quit = true;
                        }
                    }
                    break;
                    default:
                        break;
                    }
                }

                SDL_RenderClear(rend);

                // Draw stuff
                
                // Draw stuff

                SDL_RenderPresent(rend);

                fr.delay();
            }

            SDL_DestroyRenderer(rend);
            SDL_DestroyWindow(win);
        }
        else
        {
            SDL_Log("%s", SDL_GetError());
        }

        SDL_Quit();
    }
    else
    {
        SDL_Log("%s", SDL_GetError());
    }

    return 0;
}