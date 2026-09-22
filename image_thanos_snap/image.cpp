#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <utility>
// #include <iostream>

void thanos_snap(SDL_Surface *img, SDL_Window *win, SDL_Surface *s){
    // shuffle pixel order
    std::vector<std::pair<int, int>> pixelOrder;
    for (int y = 0; y < img->h; y++) {
        for (int x = 0; x < img->w; x++) {
            pixelOrder.push_back({x, y});
            }
        }

        // Fisher Yates Shuffle
        for (int i = pixelOrder.size() - 1; i > 0; i--) {
            int j = SDL_rand(i + 1);
            std::swap(pixelOrder[i], pixelOrder[j]);
        }
                                
        Uint32* pixels = (Uint32*)img->pixels;
        int pitch = img->pitch / sizeof(Uint32); // img->pitch is needed for memory offset, int pitch is the number of pixels per row

        // vist each pixel in the shuffled order
        for (const auto& [x, y] : pixelOrder) {
            
            // Do not index out of bounds on the edges
            std::vector<std::pair<int, int>> neighbors;
            if (x - 1 > -1 && y - 1 > -1         )   { neighbors.push_back({x - 1, y - 1}); }   //top left
            if (y - 1 > -1                       )   { neighbors.push_back({x, y - 1});     }   //top
            if (x + 1 < img->w && y - 1 > -1     )   { neighbors.push_back({x + 1, y - 1}); }   //top right
            if (x - 1 > -1                       )   { neighbors.push_back({x - 1, y});     }   //left
            if (x + 1 < img->w                   )   { neighbors.push_back({x + 1, y});     }   //rigth
            if (x - 1 > -1 && y + 1 < img->h     )   { neighbors.push_back({x - 1, y + 1}); }   //bottom left
            if (y + 1 < img->h                   )   { neighbors.push_back({x, y + 1});     }   //bottom
            if (x + 1 < img->w && y + 1 < img->h )   { neighbors.push_back({x + 1, y + 1}); }   //bottom right

            // select a adjacent pixel at random
            int index = SDL_rand(static_cast<int>(neighbors.size()));
            auto [p2_x, p2_y] = neighbors[index];
                                    
            // swap Uint32 pixel values of P1 and P2 (DO NOT SWAP PIXELS ON THE WINDOW SURFACE DIRECTLY)
            std::swap(
                pixels[y * pitch + x], // y * ptich give row + x give location in row
                pixels[p2_y * pitch + p2_x] // y * ptich give row + x give location in row
            );
        }

        // Update Image
        SDL_BlitSurface(img, nullptr, s, nullptr);
        SDL_UpdateWindowSurface(win);
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        SDL_Log("Need a filepath of an image");
        return 1;
    }

    if(SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Surface *img;
        int imageIndex = 0;      

        img = IMG_Load(argv[1]);
        if(!img) {
            SDL_Log("Could not load \"%s\"", argv[1]);
            return 1;
        }

        // Convert to 32 bit image for standarization
        img = SDL_ConvertSurface(img, SDL_PIXELFORMAT_ARGB8888);

        if(!img) {
            SDL_Log("Could not convert: \"%s\"", SDL_GetError());
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
                                thanos_snap(img, win, s);
                            }

                            if(k.key == SDLK_S) {
                                std::string file = "image" + std::to_string(imageIndex) + ".png";
                                // IMG_SavePNG(s, file);
                                bool status = SDL_SavePNG(s, file.c_str());
                                if (!status) { SDL_Log("Error saving file: \"%s\"", SDL_GetError()); }
                                imageIndex++;
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