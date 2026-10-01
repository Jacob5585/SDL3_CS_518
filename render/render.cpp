#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "color.h"
#include "framerate.h"

int main(int argc, char *argv[]) {

    if(argc != 2) {
        SDL_Log("Need a filepath of an image");
        return 1;
    }

    if(SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Surface *orig_img;

        orig_img = IMG_Load(argv[1]);
        if(!orig_img) {
            SDL_Log("Could not load \"%s\"", argv[1]);
            return 1;
        }

        // SDL_Window *win = SDL_CreateWindow(argv[0], orig_img->w, orig_img->h, 0);
        
        SDL_Window *win = nullptr;
        SDL_Renderer *rend = nullptr;

        if(SDL_CreateWindowAndRenderer(argv[0], orig_img->w, orig_img->h, 0, &win, &rend)) {

            // SDL_Surface *surf = SDL_GetWindowSurface(win);
            // SDL_Surface *img = SDL_ConvertSurface(orig_img, surf->format);
            SDL_Texture *img = SDL_CreateTextureFromSurface(rend, orig_img);
            SDL_DestroySurface(orig_img);

            // Uint32 *p = (Uint32 *)(img->pixels);
            // int pixel_per_row = img->pitch / sizeof(Uint32);
            const SDL_PixelFormatDetails *pdf = SDL_GetPixelFormatDetails(img->format);

            // SDL_BlitSurface(img, nullptr, surf, nullptr);
            SDL_RenderTexture(rend, img, nullptr, nullptr);
            SDL_UpdateWindowSurface(win);
            SDL_Event e;

            FrameRate fr;

            int cnt = 0;
            bool quit = false;
            while(!quit) {
                cnt++;
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

                float mx, my;
                SDL_GetMouseState(&mx, &my);


                // SDL_BlitSurface(img, nullptr, surf, nullptr);
                SDL_UpdateWindowSurface(win);
                fr.delay();
            }

            SDL_DestroySurface(img);
            SDL_DestroyRenderer(rend);
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