#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "framerate.h"
#include "assets.h"

int main(int argc, char *argv[]) {

    if(SDL_Init(SDL_INIT_VIDEO)) {

        // background
        SDL_IOStream *background_io = SDL_IOFromConstMem(backdrop_jpeg, backdrop_jpeg_len);
        if (!background_io) {
            SDL_Log("Could not create SDL_IOStream for background");
            return 1;
        }

        SDL_Surface *background_img = IMG_Load_IO(background_io, true);
        if (!background_img) {
            SDL_Log("Could not load background_img from memory: %s", SDL_GetError());
            return 1;
        }

        // pizza
        SDL_IOStream *pizza_io = SDL_IOFromConstMem(pizza_png, pizza_png_len);
        if (!pizza_io) {
            SDL_Log("Could not create SDL_IOStream for background");
            return 1;
        }

        SDL_Surface *pizza_img = IMG_Load_IO(pizza_io, true);
        if (!pizza_img) {
            SDL_Log("Could not load background_img from memory: %s", SDL_GetError());
            return 1;
        }

        // pusheen
        SDL_IOStream *pusheen_io = SDL_IOFromConstMem(pusheen_png, pusheen_png_len);
        if (!pusheen_io) {
            SDL_Log("Could not create SDL_IOStream for background");
            return 1;
        }

        SDL_Surface *pusheen_img = IMG_Load_IO(pusheen_io, true);
        if (!pusheen_img) {
            SDL_Log("Could not load background_img from memory: %s", SDL_GetError());
            return 1;
        }

        SDL_Window *win = nullptr;
        SDL_Renderer *rend = nullptr;
        FrameRate(30);

        if(SDL_CreateWindowAndRenderer(argv[0], background_img->w, background_img->h, 0, &win, &rend)) {

            // Background
            SDL_Texture *img = SDL_CreateTextureFromSurface(rend, background_img);
            SDL_DestroySurface(background_img);
            SDL_RenderTexture(rend, img, nullptr, nullptr);

            
            // Pizza
            Uint8 cursor_r, cursor_g, cursor_b, cursor_a;
            SDL_ReadSurfacePixel(pizza_img, 0, 0, &cursor_r, &cursor_g, &cursor_b, &cursor_a);
            Uint32 colorkey = SDL_MapSurfaceRGB(pizza_img, cursor_r, cursor_g, cursor_b);

            SDL_SetSurfaceColorKey(pizza_img, true, colorkey);
            SDL_Cursor *pizza_cursor = SDL_CreateColorCursor(pizza_img, 0, 0);
            if (pizza_cursor) {
                SDL_SetCursor(pizza_cursor);
            }
            SDL_DestroySurface(pizza_img);
            // 

            // Pusheen
            SDL_ReadSurfacePixel(pusheen_img, 0, 0, &cursor_r, &cursor_g, &cursor_b, &cursor_a);
            Uint32 colorkeyPusheen = SDL_MapSurfaceRGB(pusheen_img, cursor_r, cursor_g, cursor_b);
            SDL_SetSurfaceColorKey(pusheen_img, true, colorkeyPusheen);

            SDL_Texture *pusheen_texture = SDL_CreateTextureFromSurface(rend, pusheen_img);
            SDL_DestroySurface(pusheen_img);
            SDL_SetTextureBlendMode(pusheen_texture, SDL_BLENDMODE_BLEND);
            
            float pusheen_width, pusheen_height;
            SDL_GetTextureSize(pusheen_texture, &pusheen_width, &pusheen_height);
            float currentPusheenPosX, currentPusheenPosY;
            SDL_GetMouseState(&currentPusheenPosX, &currentPusheenPosY);
            
            const Uint8 pusheenHistorySize = 30;
            Uint8 pusheenHistoryHead = 0;
            SDL_FPoint pusheenPositionHistory[pusheenHistorySize] = {currentPusheenPosX, currentPusheenPosY};
            float pusheenEaseRate = 0.10;

            bool pusheenHappy = false;
            int pusheenArrival = 0;
            float pusheenAngle = 0.0;
            // 

            // SDL_RenderTexture(rend, pusheen_texture, nullptr, nullptr);
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

                // Pusheen ease
                currentPusheenPosX += (mx - currentPusheenPosX) * pusheenEaseRate;
                currentPusheenPosY += (my - currentPusheenPosY) * pusheenEaseRate;
                pusheenPositionHistory[pusheenHistoryHead] = {currentPusheenPosX, currentPusheenPosY};
                pusheenHistoryHead = (pusheenHistoryHead + 1) % pusheenHistorySize;
                //

                // pusheen pizza happy
                float pusheen2pizza = ((mx - currentPusheenPosX) * (mx - currentPusheenPosX) + (my - currentPusheenPosY) * (my - currentPusheenPosY));
                if (pusheen2pizza < 16) {
                    pusheenArrival++;

                    if (pusheenArrival > 60) {
                        pusheenHappy = true;
                        pusheenAngle += 10;
                    }
                }
                else {
                    pusheenArrival = 0;
                    pusheenAngle = 0.0;
                    pusheenHappy = false;
                    
                }
                // 

                SDL_RenderClear(rend);
                SDL_RenderTexture(rend, img, nullptr, nullptr);
                
                // Pusheen trail
                for (int i = 0; i < pusheenHistorySize; ++i) {
                    int currentIndex = (pusheenHistoryHead + i) % pusheenHistorySize;

                    float opacity = (float)(i + 1) / (float)pusheenHistorySize;
                    SDL_SetTextureAlphaModFloat(pusheen_texture, opacity);
                    
                    float renderX = pusheenPositionHistory[currentIndex].x - (float)(pusheen_width * 0.15);
                    float renderY = pusheenPositionHistory[currentIndex].y - (float)(pusheen_height * 0.3);
                    
                    SDL_FRect destRect = {
                        renderX,
                        renderY,
                        pusheen_width,
                        pusheen_height,
                    };
                    
                    if (pusheenHappy) {
                        SDL_FPoint center = { pusheen_width * 0.5f, pusheen_height * 0.5f };
                        SDL_RenderTextureRotated(rend, pusheen_texture, nullptr, &destRect, pusheenAngle, &center, SDL_FLIP_NONE);
                    }
                    else {
                        SDL_RenderTexture(rend, pusheen_texture, nullptr, &destRect);
                    }
                }
                // 

                SDL_RenderPresent(rend);

                fr.delay();
            }

            SDL_DestroyTexture(img);
            SDL_DestroyTexture(pusheen_texture);
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