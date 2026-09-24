#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

template <typename T>
void swap(T &a, T &b) {
    T tmp = a;
    a = b;
    b = tmp;
}

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
        const int size = orig_img->h * orig_img->w;


        SDL_Window *win = SDL_CreateWindow(argv[0], orig_img->w, orig_img->h, 0);

        if(win != nullptr) {

            SDL_Surface *s = SDL_GetWindowSurface(win);
            SDL_Surface *img = SDL_ConvertSurface(orig_img, s->format);
            SDL_DestroySurface(orig_img);
            Uint32 *p = (Uint32 *)(img->pixels);

            SDL_BlitSurface(img, nullptr, s, nullptr);
            SDL_UpdateWindowSurface(win);
            SDL_Event e;

            int *pos = (int *)SDL_malloc(sizeof(int) * size);
            for(int i = 0; i < size; i++) {
                pos[i] = i;
            }

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

                /* shuffle the position order */
                for(int i = 0; i < size; i++) {
                    int j = SDL_rand(size);
                    swap(pos[i], pos[j]);
                }

                for(int i = 0; i < 50'000; i++) {
                    swap(p[pos[i]], p[pos[size - i - 1]]);
                }

                SDL_BlitSurface(img, nullptr, s, nullptr);
                SDL_UpdateWindowSurface(win);
            }

            SDL_free(pos);
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