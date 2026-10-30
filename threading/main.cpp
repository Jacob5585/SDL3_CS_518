#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

// #include <iostream>

#include "framerate.h"

const int width = 800;
const int height = 500;

bool quit = false;

static int SDLCALL run_me(void *data) {

    int i = 42;
    // for (int i = 0; i < 20; i++) {
    for (;;) {

        if (quit) { break; }

        SDL_Log("%d\n", i);
    }

    return 0;
}

static int SDLCALL many_print(void *data) {
    int N = (int)(intptr_t)data;

    for (;;) {

        if (quit) { break; }

        SDL_Log("%d", N);
    }

    return 0;
}

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

            // threading
            // SDL_Thread *thread = SDL_CreateThread(run_me, "TestThread", nullptr);
            // int thread_return_value;
            
            SDL_Thread *threads[10];
            
            for (int i = 0; i < 10; i++) {
                threads[i] = SDL_CreateThread(many_print, "many_print", (void*)(intptr_t) i);
            }
            int thread_return_value;
            
            // threading

            int cnt = 0;
            // bool quit = false;
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

            // SDL_WaitThread(thread, nullptr);
            
            for (int i = 0; i < 10; i++) {
                SDL_WaitThread(threads[i], nullptr);
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