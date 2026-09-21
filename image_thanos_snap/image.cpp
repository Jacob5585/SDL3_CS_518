#include <SDL3/SDL.h>
// #include <SDL3/SDL_image.h>

int main(int argc, char *argv[]) {

    if(argc != 2) {
        SDL_Log("Need a filepath of an image");
        return 1;
    }

    if(SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Surface *img;

        (img = SDL_LoadBMP(argv[1])) || (img = SDL_LoadPNG(argv[1]));
        if(!img) {
            SDL_Log("Could not load \"%s\"", argv[1]);
            return 1;
        }

        SDL_Window *win = SDL_CreateWindow(argv[0], img->w, img->h, 0);

        if(win != nullptr) {

            SDL_Surface *s = SDL_GetWindowSurface(win);

            SDL_BlitSurface(img, nullptr, s, nullptr);
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
                            
                            if(k.key == SDLK_SPACE) {
                                
                            }

                            if(k.key == SDLK_S) {
                                char file[] = "image0.png";
                                // IMG_SavePNG(s, file);
                                SDL_SavePNG(s, file);
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }

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