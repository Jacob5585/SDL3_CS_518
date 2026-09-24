#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "color.h"

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

        SDL_Window *win = SDL_CreateWindow(argv[0], orig_img->w, orig_img->h, 0);

        if(win != nullptr) {

            SDL_Surface *surf = SDL_GetWindowSurface(win);
            SDL_Surface *img = SDL_ConvertSurface(orig_img, surf->format);
            SDL_DestroySurface(orig_img);

            Uint32 *p = (Uint32 *)(img->pixels);
            int pixel_per_row = img->pitch / sizeof(Uint32);
            const SDL_PixelFormatDetails *pdf = SDL_GetPixelFormatDetails(img->format);

            SDL_BlitSurface(img, nullptr, surf, nullptr);
            SDL_UpdateWindowSurface(win);
            SDL_Event e;

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

                SDL_Color c;
                float h, s, v;
                float mx, my;
                SDL_GetMouseState(&mx, &my);
                Uint32 *nextp;
                for (int y = 0; y < img->h; y++) {
                    nextp = p + (y * pixel_per_row);
                    
                    for (int x = 0; x < img->w; x++) {
                        SDL_GetRGB(*nextp, pdf, nullptr, &c.r, &c.g, &c.b);
                        convertRGBtoHSV(c.r, c.g, c.b, &h, &s, &v);
                        
                        h += 10.0;
                        if (cnt % 30) {
                            v = my / img->h;
                            s = mx / img->w;
                        }

                        convertHSVtoRGB(h, s, v, &c.r, &c.g, &c.b);
                        
                        *nextp = SDL_MapRGB(pdf, nullptr, c.r, c.g, c.b);

                        nextp++;
                    }
                }

                SDL_BlitSurface(img, nullptr, surf, nullptr);
                SDL_UpdateWindowSurface(win);
            }

            SDL_DestroySurface(img);
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