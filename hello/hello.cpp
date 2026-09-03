#include <cstdlib>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Window * win = SDL_CreateWindow(argv[0], 600, 300, 0);
        
        if (win != nullptr) {
            SDL_Event e;
            
            while (SDL_PollEvent(&e) == false || e.type != SDL_EVENT_QUIT) {
                
            }
        }
        else {
            SDL_Log("%s", SDL_GetError());
        }
    }
    else {
        SDL_Log("%s", SDL_GetError());
    }

    SDL_Quit();

    return EXIT_SUCCESS;
}