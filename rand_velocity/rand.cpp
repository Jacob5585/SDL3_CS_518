#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Window *win = SDL_CreateWindow(argv[0], 600, 400, 0);
        
        if (win != nullptr) {
            SDL_Surface *s = SDL_GetWindowSurface(win);
            // Do stuff with the surface
            

            Uint32 *p = (Uint32 *)(s->pixels);
            int size = 600 * 400;
            for (auto i = 0; i < size; i++) {
                p[i] = SDL_rand_bits();
            }

            SDL_UpdateWindowSurface(win);

            SDL_Event e;
            bool quit = false;
            
            while (!quit) {
                while(SDL_PollEvent(&e)) {
                    switch (e.type) {
                        case SDL_EVENT_QUIT:
                            {
                                quit = true;
                                break;
                            }
                        case SDL_EVENT_KEY_DOWN:
                            {
                                SDL_KeyboardEvent k = e.key;
                                if (k.key == SDLK_ESCAPE) {
                                    quit = true;
                                }
                                break;
                            }
                        default:
                            {
                                break;
                            }
                        }
                    }

                for (auto i = 1; i < size - 1; i++) {
                    p[i] = 0.5 * p[i] + .5 * p[i];
                    // p[i] = 0.8 * p[i] + .2 * p[i];
                }
                
                SDL_UpdateWindowSurface(win);
            }
        }
        else {
            SDL_DestroyWindowSurface(win);
            SDL_Log("%s", SDL_GetError());
        }
    }
    else {
        SDL_Log("%s", SDL_GetError());
    }

    SDL_Quit();

    return 0;
}