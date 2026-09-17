#include <SDL3/SDL.h>

const int width = 600;
const int height = 400;
const int size = width * height;
const float alpha = 0.90;

int main(int argc, char *argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Window *win = SDL_CreateWindow(argv[0], width, height, 0);

        if(win != nullptr) {

            SDL_Surface *s = SDL_GetWindowSurface(win);

            /*
             * Start off with a bunch of random colors for each pixel
             */
            Uint32 *p = (Uint32 *)(s->pixels);
            for(auto i = 0; i < size; i++) {
                // p[i] = SDL_rand_bits();

                // grayscale
                Uint8 c = SDL_rand(256);
                p[i] = SDL_MapSurfaceRGB(s, c, c, c);
            }

            SDL_UpdateWindowSurface(win);
            SDL_Event e;

            bool quit = false;
            while(!quit) {
                while(SDL_PollEvent(&e)) {
                    switch(e.type) {
                    case SDL_EVENT_QUIT:
                        quit = true;
                        break;
                    case SDL_EVENT_KEY_DOWN:
                        {
                            SDL_KeyboardEvent k = e.key;
                            if(k.key == SDLK_ESCAPE) {
                                quit = true;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }

                /*
                 * Modify each pixel to clone one of its neighbors to the
                 * left or right in memory.  Every once in a while, though,
                 * just pick a new color for the pixel to ensure the resulting
                 * color bands don't eventually reach a singularity :-)
                 */
                for(auto i = 1; i < size - 1; i++) {
                    Uint8 c1;
                    SDL_GetRGB(p[i], SDL_GetPixelFormatDetails(s->format), nullptr, &c1, nullptr, nullptr);
                    Uint8 c2 = SDL_rand(256);
                    c1 = alpha * c1 + (1.0 - alpha) * c2;
                    p[i] = SDL_MapSurfaceRGB(s, c1, c1, c1);

                    // if(SDL_rand(1000) > 0) {
                    //     /* randomly choose the pixel to the left or right */
                    //     // color
                    //     // Uint32 c = p[i + SDL_rand(2) * 2 - 1];
                    //     // p[i] = c;

                    //     // grayscale
                    //     Uint8 c = SDL_rand(256);
                    //     p[i] = SDL_MapSurfaceRGB(s, c, c, c);
                    // }
                    // else {
                    //     p[i] = SDL_rand_bits();
                    // }
                }

                SDL_UpdateWindowSurface(win);
            }

            SDL_DestroyWindow(win);

        }
        else {
            SDL_Log("%s", SDL_GetError());
        }

        SDL_Quit();
    }
    else {
        SDL_Log("%s", SDL_GetError());
    }
    return 0;
}