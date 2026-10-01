#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

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

        SDL_Window *win = nullptr;
        SDL_Renderer *rend = nullptr;


        if(SDL_CreateWindowAndRenderer(argv[0], orig_img->w, orig_img->h, 0, &win, &rend)) {

            SDL_Texture *img = SDL_CreateTextureFromSurface(rend, orig_img);
            SDL_DestroySurface(orig_img);

            SDL_RenderTexture(rend, img, nullptr, nullptr);
            SDL_RenderPresent(rend);
            SDL_Event e;

            FrameRate fr(40);

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

                float mx = 1.0, my = 1.0;
                SDL_GetMouseState(&mx, &my);


//                SDL_RenderPresent(rend);

                fr.delay();
            }

            SDL_DestroyTexture(img);
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