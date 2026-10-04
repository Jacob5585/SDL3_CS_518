/*
    Jacob Cormier MultiMedia Programming project 3
    This program loads an image tacken as a command line argument, automaticallty spawns a firecracker effect every few seconds,
    allows for randonly swapping pixels, shifting the hue in a ripple effect, and restoring the image back to the oringal

    Right mouse click to restore the image back to the orignal
    Left mouse click to create a hue shifting ripple that grows the longer the mouse is held down, hue change uses ineratia
    G randomly swaps pixels
    Firecracker effect randonly appears on screen every few seconds
*/

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include <utility>
#include <cstring>

#include "color.h"

struct FireCrackerPartical {
    float pos_x, pos_y;
    int prev_x, prev_y;
    float vel_x, vel_y;
    float decay;
    float time;
    float hue;
    SDL_Color background_color;
    bool first_frame{true};
    bool active{false};
};

void get_pixel_neighbors(std::vector<std::pair<int, int>> &neighbors, const std::vector<std::vector<SDL_Color>> &buffer, int x, int y, int distance) {
    int row_length = buffer[0].size();
    int col_length = buffer.size();

    for (int dy = -distance; dy <= distance; dy++) {
        for (int dx = -distance; dx <= distance; dx++) {
            int x_pos = x + dx;
            int y_pos = y + dy;

            if (dx * dx + dy * dy <= distance * distance) {
                if (x_pos >= 0 && x_pos < row_length && y_pos >= 0 && y_pos < col_length) {
                    neighbors.push_back({y_pos, x_pos});
                }
            }
        }
    }
}

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

void glitch(SDL_Surface *img, SDL_Window *win, SDL_Surface *surface, Uint32 *pixels, std::vector<std::vector<SDL_Color>> &buffer) {
    int pixels_per_row = img->pitch / sizeof(Uint32);
    unsigned glitched_pixels = (img->h * img->w) * .05;
    
    for (unsigned i = 0; i < glitched_pixels; i++) {
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

void restore(SDL_Surface *img, SDL_Window *win, SDL_Surface *surface, Uint32 *pixels, SDL_Surface *orginal_img, std::vector<std::vector<SDL_Color>> &pixelsColorGrid) {
    Uint32* orginal_pixels = (Uint32*)orginal_img->pixels;

    std::memcpy(pixels, orginal_pixels, img->h * img->pitch);
    
    SDL_BlitSurface(img, nullptr, surface, nullptr);
    SDL_UpdateWindowSurface(win);

    for (int y = 0; y < img->h; y++) {
        for (int x = 0; x < img->w; x++) {
            Uint8 r, g, b, a;
            SDL_ReadSurfacePixel(surface, x, y, &r, &g, &b, &a);
            pixelsColorGrid[y][x] = SDL_Color{r, g, b, a};
        }
    }
}

void firecracker_init(int x, int y, std::vector<FireCrackerPartical> &firecracker_particles, std::vector<std::vector<SDL_Color>> &pixelsColorGrid, Uint32 interval_ms, SDL_Surface *img) {
    float interval_seconds = interval_ms / 1000.0;
    
    for (auto &particle : firecracker_particles) {

        if (particle.active && particle.prev_x >= 0 && particle.prev_x < img->w && particle.prev_y >= 0 && particle.prev_y < img->h) {
            SDL_Color origal_color = pixelsColorGrid[particle.prev_y][particle.prev_x];
            SDL_WriteSurfacePixel(img, particle.prev_x, particle.prev_y, origal_color.r, origal_color.g, origal_color.b, origal_color.a);
        }

        particle.pos_x = x;
        particle.pos_y = y;

        particle.prev_x = -1;
        particle.prev_y = -1;

        float angle = SDL_randf() * ((2.0 * SDL_PI_F));
        float speed = 30.0f + SDL_randf() * 90.0f;
        particle.vel_x = SDL_cosf(angle) * speed;
        particle.vel_y = SDL_sinf(angle) * speed;

        particle.decay = interval_seconds * (0.8f + SDL_randf() * 0.4f);
        particle.time = particle.decay;

        particle.hue = SDL_randf() * 35;
        particle.background_color = pixelsColorGrid[y][x];
        particle.active = true;
        particle.first_frame = true;
    }
}

void firecracker_update(SDL_Surface *img, std::vector<FireCrackerPartical> &firecracker_particles, std::vector<std::vector<SDL_Color>> &pixelsColorGrid, float delta_time) {
    for (auto &particle : firecracker_particles) {
        if (!particle.active) { continue; }

        // Restore background color
        if ((particle.prev_x >= 0 && particle.prev_x < img->w) && (particle.prev_y >= 0 && particle.prev_y < img->h)) {
            SDL_Color origal_color = pixelsColorGrid[particle.prev_y][particle.prev_x];
            SDL_WriteSurfacePixel(img, particle.prev_x, particle.prev_y, origal_color.r, origal_color.g, origal_color.b, origal_color.a);
            particle.prev_x = -1;
            particle.prev_y = -1;
        }

        // Movement scaled by delta time
        if (!particle.first_frame) {
            particle.pos_x += particle.vel_x * delta_time;
            particle.pos_y += particle.vel_y * delta_time;
            particle.vel_y += 15.0f * delta_time;
            particle.time -= delta_time;
        } else {
            particle.first_frame = false;
        }
        
        int new_pos_x = (int)particle.pos_x;
        int new_pos_y = (int)particle.pos_y;

        // Check bounds and lifetime
        if (particle.time <= 0.0f || new_pos_x < 0 || new_pos_x >= img->w || new_pos_y < 0 || new_pos_y >= img->h) {            
            particle.active = false;
            continue;
        }

        particle.prev_x = new_pos_x;
        particle.prev_y = new_pos_y;

        Uint8 r, g, b;
        convertHSVtoRGB(particle.hue, 1.0f, (particle.time / particle.decay), &r, &g, &b);
        SDL_Color particle_color = SDL_Color {r, g, b, 255};
        SDL_WriteSurfacePixel(img, new_pos_x, new_pos_y, particle_color.r, particle_color.g, particle_color.b, particle_color.a);
    }
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

            std::vector<FireCrackerPartical> firecracker_particles(2500);
            Uint32 firecracker_interval = 1000;
            Uint32 last_firecracker_spawn = SDL_GetTicks();
            Uint32 last_frame_time = SDL_GetTicks();

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
                                restore(img, win, surface, pixels, orginal_img, pixelsColorGrid);
                            }
                        }

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

                Uint32 time_ms = SDL_GetTicks();
                float delta_time = (time_ms - last_frame_time) / 1000.0;
                last_frame_time = time_ms;
                if (time_ms - last_firecracker_spawn > firecracker_interval) {
                    last_firecracker_spawn = time_ms;
                    int firecracker_x = SDL_rand(img->w);
                    int firecracker_y = SDL_rand(img->h);
                    firecracker_init(firecracker_x, firecracker_y, firecracker_particles, pixelsColorGrid, firecracker_interval, img);
                }

                firecracker_update(img, firecracker_particles, pixelsColorGrid, delta_time);
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

