#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <utility>
#include <cstring>

#include "color.h"

template <typename Buffer2D, typename neighbors1D>
void get_pixel_neighbors(neighbors1D &neighbors, const Buffer2D &buffer, int x, int y, int distance) {
    // int y_offset[] = {-1, -1, -1,  0, 0,  0, 1, 1, 1};
    // int x_offset[] = {-1,  0,  1, -1, 0, 1, -1, 0, 1};
    int row_length = buffer[0].size();
    int col_length = buffer.size();

    // for (int i = 0; i < 9; i++) {
    //     int x_pos = x + x_offset[i];
    //     int y_pos = y + y_offset[i];

    for (int dy = -distance; dy <= distance; dy++) {
        for (int dx = -distance; dx <= distance; dx++) {
            int x_pos = x + dx;
            int y_pos = y + dy;

            if (dx * dx + dy * dy <= distance * distance) {
                if (x_pos >= 0 && x_pos < row_length && y_pos >= 0 && y_pos < col_length) {
                    // neighbors.push_back(buffer[y_pos][x_pos]);
                    neighbors.push_back({y_pos, x_pos});
                }
            }
        }
    }
}

// 
// unintetinal bug where this undoes the glitch effect, I should fix this and
// make useing the right button Used to restore to the orginal or just undo the glitch but not fully restore
// 
void ripple(SDL_Surface *img, SDL_Window *win, SDL_Surface *surface, std::vector<std::vector<SDL_Color>> &buffer, int x, int y, unsigned distance) {
    
    float h, s, v;

    std::vector<std::pair<int, int>> neighbors;
    get_pixel_neighbors(neighbors, buffer, x, y, distance);
    
    for (auto neighbor : neighbors) {
        
        SDL_Color n = buffer[neighbor.first][neighbor.second];
        
        convertRGBtoHSV(n.r, n.g, n.b, &h, &s, &v);
        h = h + ((h + 10.0) - h) * 0.15; // hue with ineratia for the shift
        convertHSVtoRGB(h, s, v, &n.r, &n.g, &n.b);
        
        SDL_WriteSurfacePixel(img, neighbor.second, neighbor.first, n.r, n.g, n.b, n.a);

        buffer[neighbor.first][neighbor.second] = n;
    }

    SDL_BlitSurface(img, nullptr, surface, nullptr);
    SDL_UpdateWindowSurface(win);
}

// add in seed
void glitch(SDL_Surface *img, SDL_Window *win, SDL_Surface *surface, Uint32 *pixels, std::vector<std::vector<SDL_Color>> &buffer) {
    // randonly swap pixels on the screen
    
    int pixels_per_row = img->pitch / sizeof(Uint32);
    unsigned glitched_pixels = (img->h * img->w) * .05;
    
    for (int i = 0; i < glitched_pixels; i++) {
        unsigned x1 = SDL_rand(img->w);
        unsigned y1 = SDL_rand(img->h);
        unsigned x2 = SDL_rand(img->w);
        unsigned y2 = SDL_rand(img->h);
        
        std::swap(pixels[y1 * pixels_per_row + x1], pixels[y2 * pixels_per_row + x2]);
        std::swap(buffer[y1][x1], buffer[y2][x2]); // Update colorCoordate so ripple doesnt modify the non swapped pixels
    }
    
    SDL_BlitSurface(img, nullptr, surface, nullptr);
    SDL_UpdateWindowSurface(win);
}

void restore(SDL_Surface *img, SDL_Window *win, SDL_Surface *surface, Uint32 *pixels, SDL_Surface *orginal_img) {
    Uint32* orginal_pixels = (Uint32*)orginal_img->pixels;
    // pixels = (Uint32*)orginal_img->pixels;

    std::memcpy(pixels, orginal_pixels, img->h * img->pitch);

    SDL_BlitSurface(img, nullptr, surface, nullptr);
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

        if(!img) {
            SDL_Log("Could not convert: \"%s\"", SDL_GetError());
            return 1;
        }
        
        SDL_Window *win = SDL_CreateWindow("Prog 3: cormiej", img->w, img->h, 0);
        
        if(win != nullptr) {
            
            SDL_Surface *surface = SDL_GetWindowSurface(win);
            img = SDL_ConvertSurface(img, SDL_PIXELFORMAT_ABGR8888);
            SDL_Surface *orginal_img = SDL_DuplicateSurface(img);
            
            SDL_BlitSurface(img, nullptr, surface, nullptr);
            SDL_UpdateWindowSurface(win);
            SDL_Event e;

            // init varaibles
            float mouse_x, mouse_y;
            Uint32 mouse_hold_start_time;

            // Uint32 orginal_pixels = (Uint32)img->pixels;
            Uint32* pixels = (Uint32*)img->pixels;

            std::vector<std::vector<std::pair<int, int>>> pixelsCoordinateGrid(img->h, std::vector<std::pair<int, int>>(img->w));
            std::vector<std::vector<SDL_Color>> pixelsColorGrid(img->h, std::vector<SDL_Color>(img->w));
            for (int y = 0; y < img->h; y++) {
                for (int x = 0; x < img->w; x++) {
                    Uint8 r, g, b, a;
                    SDL_ReadSurfacePixel(surface, x, y, &r, &g, &b, &a);
                    pixelsColorGrid[y][x] = SDL_Color{r, g, b, a};
                    pixelsCoordinateGrid[y][x] = {y, x};
                }
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
                            
                            if(k.key == SDLK_SPACE) {

                            }

                            if(k.key == SDLK_G) {
                                glitch(img, win, surface, pixels, pixelsColorGrid);
                            }

                            if(k.key == SDLK_S) {
                                std::string file = "image" + std::to_string(imageIndex) + ".png";
                                bool status = IMG_SavePNG(surface, file.c_str());
                                if (!status) { SDL_Log("Error saving file: \"%s\"", SDL_GetError()); }
                                imageIndex++;
                            }
                        }
                        break;
                    
                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                        {
                            if (e.button.button == SDL_BUTTON_LEFT) {
                                mouse_hold_start_time = SDL_GetTicks();
                            }

                            if (e.button.button == SDL_BUTTON_RIGHT) {
                                restore(img, win, surface, pixels, orginal_img);
                            }
                        }

                    // case SDL_EVENT_MOUSE_BUTTON_UP:
                    //     {
                    //         if (e.button.button == SDL_BUTTON_LEFT) {
                    //         }
                    //     }
                    
                    default:
                        break;
                    }
                }
                // Left click being held down
                Uint32 button = SDL_GetMouseState(&mouse_x, &mouse_y); //Returns: a 32-bit bitmask of the button state that can be bitwise-compared against the SDL_BUTTON_MASK(X) macro.
                
                // button & SDL_BUTTON_LMASK is true left mouse is being clicked
                if (button & SDL_BUTTON_LMASK) {
                    // Calculate a distance based on relative to how long the button has been help
                    Uint32 hold_duration = SDL_GetTicks() - mouse_hold_start_time;
                    unsigned distance = 5 + (hold_duration / 100);

                    int pixel_coord_x = SDL_clamp(mouse_x, 0, img->w -1);
                    int pixel_coord_y = SDL_clamp(mouse_y, 0, img->h -1);

                    ripple(img, win, surface, pixelsColorGrid, pixel_coord_x, pixel_coord_y, distance);
                }

                // 
                SDL_BlitSurface(img, nullptr, surface, nullptr);
                SDL_UpdateWindowSurface(win);
            }
            SDL_DestroySurface(img);
            SDL_DestroySurface(orginal_img);
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