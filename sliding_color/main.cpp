#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

Uint8 update_color(Uint8 color) {
    if (color == 0) {
        color++;
    }
    else if (color == 255) {
        color--;
    }
    else {
        Uint8 direction = SDL_rand(2);

        if (direction == 0) {
            color++;
        }
        else if (direction == 1) {
            color--;
        }
    }
    
    return color;
}

int main(int argc, char *argv[]) {
    Uint32 delay = 5;
    if (argc > 1) {
        delay = SDL_strtoul(argv[1], NULL, 10);
    }

    if (SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Window *win = SDL_CreateWindow("Prog 1: cormiej", 300, 300, 0);
        
        if (win != nullptr) {
            SDL_Surface *s = SDL_GetWindowSurface(win);
            
            // Do stuff with the surface
            Uint8 r_color = SDL_rand(256);
            Uint8 g_color = SDL_rand(256);
            Uint8 b_color = SDL_rand(256);
            Uint32 color = SDL_MapSurfaceRGB(s, r_color, g_color, b_color);
            SDL_FillSurfaceRect(s, NULL, color);
            
            SDL_UpdateWindowSurface(win);

            SDL_Event e;
            
            while (SDL_PollEvent(&e) == false || e.type != SDL_EVENT_QUIT) {
                // Update color each loop
                r_color = update_color(r_color);
                g_color = update_color(g_color);
                b_color = update_color(b_color);
                
                color = SDL_MapSurfaceRGB(s, r_color, g_color, b_color);
                SDL_FillSurfaceRect(s, NULL, color);

                // Delay to slow color change
                SDL_Delay(delay);
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