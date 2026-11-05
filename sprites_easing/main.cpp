#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "framerate.h"

const char *font_file = "font_22x32.png";
const char *ball_file = "ball.bmp";

const int width = 1000;
const int height = 300;

const char s[] = "Move your     , if you want to freak";
const char s_font[] = "FONT";
/* FONT goes at index 10 of the above string */
const float s_font_index = 10;

float ease1(float x) {
    return x < 0.5f ? 16 * x * x * x * x * x : 1.0f - SDL_powf(-2 * x + 2, 5) / 2.0f;
}


float ease2(float x) {
    const float n1 = 7.5625;
    const float d1 = 2.75;

    if (x < 1 / d1) {
        return n1 * x * x;
    } else if (x < 2 / d1) {
        x -= 1.5 / d1;
        return n1 * (x) * x + 0.75;
    } else if (x < 2.5 / d1) {
        x -= 2.25 / d1;
        return n1 * (x) * x + 0.9375;
    } else {
        x -= 2.625 / d1;
        return n1 * (x) * x + 0.984375;
    }
}


int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Surface *tmp_img;

        tmp_img = IMG_Load(font_file);
        if (!tmp_img)
        {
            SDL_Log("Could not load \"%s\"", font_file);
            return 1;
        }

        SDL_Window *win = nullptr;
        SDL_Renderer *rend = nullptr;

        if (SDL_CreateWindowAndRenderer(argv[0], width, height, 0, &win, &rend))
        {

            SDL_SetSurfaceColorKey(tmp_img, true, *(Uint32 *)tmp_img->pixels);
            SDL_Texture *font = SDL_CreateTextureFromSurface(rend, tmp_img);
            SDL_DestroySurface(tmp_img);

            tmp_img = IMG_Load(ball_file);
            SDL_Texture *ball = SDL_CreateTextureFromSurface(rend, tmp_img);
            SDL_DestroySurface(tmp_img);

            const int fwidth = font->w / 16;
            const int fheight = font->h / 8;

            const int cx = width / 2;
            const int cy = height / 2;

            const int len = SDL_strlen(s);
            const int str_width = len * fwidth;
            const int &str_height = fheight;

            float str_x = cx - (str_width / 2);
            float str_y = cy - (str_height / 2);

            float str_font_x = str_x + (s_font_index * fwidth);
            float &str_font_y = str_y;

            SDL_Event e;

            FrameRate fr;

            SDL_SetRenderDrawColor(rend, 0xdd, 0xbb, 0xff, SDL_ALPHA_OPAQUE);

            int cnt = 0;
            bool quit = false;
            while (!quit)
            {
                cnt++;
                while (SDL_PollEvent(&e))
                {
                    switch (e.type)
                    {
                    case SDL_EVENT_QUIT:
                        quit = true;
                        break;
                    case SDL_EVENT_KEY_DOWN:
                    {
                        SDL_KeyboardEvent k = e.key;
                        if (k.key == SDLK_ESCAPE)
                        {
                            quit = true;
                        }
                    }
                    break;
                    default:
                        break;
                    }
                }

                SDL_RenderClear(rend);

                float mx, my;
                SDL_GetMouseState(&mx, &my);

                SDL_FRect ball_dst = {
                    .x = ease2(mx / (float)width) * (width - ball->w),
                    .y = 50.0f,
                    .w = (float)ball->w,
                    .h = (float)ball->h,
                };

                SDL_RenderTextureRotated(rend, ball, nullptr, &ball_dst, ball_dst.x, nullptr, SDL_FLIP_NONE);

                /* Draw main background string */
                for(int i = 0; i < len; i++) {
                    SDL_FRect src_rect = {
                        .x = (float)((int)s[i] % 16) * fwidth,
                        .y = (float)((int)s[i] / 16) * fheight,
                        .w = (float)fwidth,
                        .h = (float)fheight,
                    };

                    SDL_FRect dst_rect = {
                        .x = str_x + i * fwidth,
                        .y = str_y,
                        .w = (float)fwidth,
                        .h = (float)fheight,
                    };

                    SDL_RenderTextureRotated(rend, font, &src_rect, &dst_rect, 0, nullptr, SDL_FLIP_HORIZONTAL_AND_VERTICAL);
                }

                /* Move your FONT!!!! */
                for(int i = 0; i < 4; i++) {
                    int xmove = SDL_rand(7) - 3;
                    int ymove = SDL_rand(7) - 3;

                    SDL_FRect src_rect = {
                        .x = (float)((int)s_font[i] % 16) * fwidth,
                        .y = (float)((int)s_font[i] / 16) * fheight,
                        .w = (float)fwidth,
                        .h = (float)fheight,
                    };

                    SDL_FRect dst_rect = {
                        .x = xmove + str_font_x + i * fwidth,
                        .y = ymove + str_font_y,
                        .w = (float)fwidth,
                        .h = (float)fheight,
                    };

                    SDL_RenderTexture(rend, font, &src_rect, &dst_rect);
                }

                SDL_RenderPresent(rend);

                fr.delay();
            }

            SDL_DestroyTexture(font);
            SDL_DestroyRenderer(rend);
            SDL_DestroyWindow(win);
        }
        else
        {
            SDL_Log("%s", SDL_GetError());
        }

        SDL_Quit();
    }
    else
    {
        SDL_Log("%s", SDL_GetError());
    }

    return 0;
}