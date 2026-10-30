#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <cmath>

#include "framerate.h"
#include "assets.h"

const int width = 1400;
const int height = 800;

// score
int score = 0;
int best_score = 0;
// 

const char text[] = "Click to hit, SPACE to lob";
const char score_text[] = "Score: ";
const char best_text[] = "Best: ";
const char s_font[] = "FONT";
/* FONT goes at index 10 of the above string */
const float s_font_index = 10;

struct ball_state {
    float ball_x;
    float ball_y;
    float ball_vx;
    float ball_vy;
    float ball_width;
    float ball_height;
};

void draw_font(SDL_Renderer *rend, const char str[], SDL_Texture* font, const int posX, const int posY){
    const int fontWidth = font->w / 16;
    const int fontHeight = font->h / 8;

    const int len = SDL_strlen(str);
    const int str_width = len * fontWidth;
    const int &str_height = fontHeight;

    float str_x = posX - (str_width / 2);
    float str_y = posY - (str_height / 2);

    float str_font_x = str_x + (s_font_index * fontWidth);
    float &str_font_y = str_y;
    
    for(int i = 0; i < len; i++) {
        SDL_FRect src_rect = {
            .x = (float)((int)str[i] % 16) * fontWidth,
            .y = (float)((int)str[i] / 16) * fontHeight,
            .w = (float)fontWidth,
            .h = (float)fontHeight,
        };

        SDL_FRect dst_rect = {
            .x = str_x + i * fontWidth,
            .y = str_y,
            .w = (float)fontWidth,
            .h = (float)fontHeight,
        };


        SDL_RenderTexture(rend, font, &src_rect, &dst_rect);
        // SDL_RenderTextureRotated(rend, font, &src_rect, &dst_rect, 0, nullptr, SDL_FLIP_HORIZONTAL_AND_VERTICAL);
    }
}

void lob_ball(struct ball_state &ball_state) {
    score = 0;
    ball_state.ball_vx = ((SDL_rand(100) % 10) - 5.0) * 2.0;
    ball_state.ball_vy = -15.0 - (SDL_rand(100) % 5);
}

void hit_ball(struct ball_state &ball_state, float character_x, float character_height) {
    float ball_center_x = ball_state.ball_x + (ball_state.ball_width  / 2.0);
    float ball_center_y = ball_state.ball_y + (ball_state.ball_height / 2.0);

    float character_center_x = character_x;
    float character_center_y = (height - character_height) + (character_height / 2.0);

    ball_state.ball_vx = (ball_center_x - character_center_x) * 0.15;
    ball_state.ball_vy = (ball_center_y - character_center_y) * 0.15;

    score++;
    if (score > best_score) {
        best_score = score;
    }
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_IOStream *font_io = SDL_IOFromConstMem(font_22x32_png, font_22x32_png_len);
        if (!font_io) {
            SDL_Log("Could not create SDL_IOStream for font");
            return 1;
        }
     
        SDL_Surface *tmp_img = IMG_Load_IO(font_io, true);
        if (!tmp_img) {
            SDL_Log("Could not load font from memory: %s", SDL_GetError());
            return 1;
        }
        
        SDL_IOStream *ball_io = SDL_IOFromConstMem(ball_bmp, ball_bmp_len);
        if (!ball_io) {
            SDL_Log("Could not create SDL_IOStream for ball");
            return 1;
        }
        SDL_Surface *tmp_ball = IMG_Load_IO(ball_io, true);
        if (!tmp_ball) {
            SDL_Log("Could not load ball from memory: %s", SDL_GetError());
            return 1;
        }

        SDL_IOStream *running_man_io = SDL_IOFromConstMem(running_man_bmp, running_man_bmp_len);
        if (!running_man_io) {
            SDL_Log("Could not create SDL_IOStream for running_man");
            return 1;
        }
        SDL_Surface *tmp_character = IMG_Load_IO(running_man_io, true);
        if (!tmp_character) {
            SDL_Log("Could not load running_man from memory: %s", SDL_GetError());
            return 1;
        }

        SDL_IOStream *boogie_man_io = SDL_IOFromConstMem(boogie_man_bmp, boogie_man_bmp_len);
        if (!boogie_man_io) {
            SDL_Log("Could not create SDL_IOStream for boogie_man");
            return 1;
        }
        SDL_Surface *tmp_idle = IMG_Load_IO(boogie_man_io, true);
        if (!tmp_idle) {
            SDL_Log("Could not load boogie_man from memory: %s", SDL_GetError());
            return 1;
        }

        SDL_IOStream *background_io = SDL_IOFromConstMem(beach_background_jpg, beach_background_jpg_len);
        if (!background_io) {
            SDL_Log("Could not create SDL_IOStream for background");
            return 1;
        }
        SDL_Surface *tmp_background = IMG_Load_IO(background_io, true);
        if (!tmp_background) {
            SDL_Log("Could not load background from memory: %s", SDL_GetError());
            return 1;
        }

        SDL_Window *win = nullptr;
        SDL_Renderer *rend = nullptr;

        if (SDL_CreateWindowAndRenderer(argv[0], width, height, 0, &win, &rend))
        {
            SDL_SetSurfaceColorKey(tmp_img, true, *(Uint32 *)tmp_img->pixels);
            SDL_Texture *font = SDL_CreateTextureFromSurface(rend, tmp_img);
            SDL_DestroySurface(tmp_img);
            
            SDL_SetSurfaceColorKey(tmp_ball, true, *(Uint32 *)tmp_ball->pixels);
            SDL_Texture *ball = SDL_CreateTextureFromSurface(rend, tmp_ball);
            SDL_DestroySurface(tmp_ball);
            
            SDL_Texture *character = SDL_CreateTextureFromSurface(rend, tmp_character);
            SDL_DestroySurface(tmp_character);
            
            SDL_Texture *idle_character = SDL_CreateTextureFromSurface(rend, tmp_idle);
            SDL_DestroySurface(tmp_idle);

            SDL_Texture *background = SDL_CreateTextureFromSurface(rend, tmp_background);
            SDL_DestroySurface(tmp_background);

            // init
            // character
            float character_width = character->w / 7;
            float character_height = character->h / 2;
            float idle_width = idle_character->w / 7;
            float idle_height = idle_character->h;
            SDL_Texture* active_character_texture = nullptr;
            float active_character_width = idle_width;
            float active_character_height = idle_height;
            static float character_x = width / 2;
            float character_y = height - character_height;
            static int direction = 0;

            // ball
            float ball_width = ball->w;
            float ball_height = ball->h;

            ball_state ball_state;
            ball_state.ball_x = width/2;
            ball_state.ball_y = height - ball_width;
            ball_state.ball_vx = 0.0;
            ball_state.ball_vy = 0.0;
            ball_state.ball_width = ball_width;
            ball_state.ball_height = ball_height;
            // init

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
                    {                        
                        quit = true;
                        break;
                    }

                    case SDL_EVENT_KEY_DOWN:
                    {
                        SDL_KeyboardEvent k = e.key;
                        if (k.key == SDLK_ESCAPE)
                        {
                            quit = true;
                        }
                                
                        if (k.key == SDLK_SPACE)
                        {
                            score = 0;
                            lob_ball(ball_state);
                        }

                        break;
                    }

                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    {
                        if (e.button.button == SDL_BUTTON_LEFT) {
                            SDL_FRect character_rect {
                                .x = character_x - (character_width / 2.0),
                                .y = character_y,
                                .w = character_width,
                                .h = height - active_character_height,
                            };

                            SDL_FRect ball_rect {
                                ball_state.ball_x,
                                ball_state.ball_y,
                                ball_width,
                                ball_height,
                            };
                            
                            if (ball_rect.x < character_rect.x + character_rect.w &&
                                ball_rect.x + ball_rect.w > character_rect.x &&
                                ball_rect.y < character_rect.y + character_rect.h &&
                                ball_rect.y + ball_rect.h > character_rect.y)
                            {    
                                hit_ball(ball_state, character_x, active_character_height);
                            }
                        }
                    }

                    break;
                    default:
                        break;
                    }
                }

                SDL_RenderClear(rend);

                // Game loop
                SDL_RenderTexture(rend, background, nullptr, nullptr);

                // ball
                ball_state.ball_vy += 0.5f;
                ball_state.ball_y += ball_state.ball_vy;
                ball_state.ball_vx *= 0.99f; // air friction
                ball_state.ball_x += ball_state.ball_vx;
                // ball_state.ball_angle += ball_state.ball_vx * 0.5f; // spin

                if (ball_state.ball_y >= height - ball_height) {
                    ball_state.ball_y = height - ball_height;
                    ball_state.ball_vy = -ball_state.ball_vy * 0.7f;
                    ball_state.ball_vx *= 0.9f;
                    score = 0;
                }

                if (ball_state.ball_x <= 0) {
                    ball_state.ball_x = 0;
                    ball_state.ball_vx = -ball_state.ball_vx * 0.8f;
                }

                if (ball_state.ball_x >= width - ball_width) {
                    ball_state.ball_x = width - ball_width;
                    ball_state.ball_vx = -ball_state.ball_vx * 0.8f;
                }

                SDL_FRect ball_dest = {
                    .x = ball_state.ball_x,
                    .y = ball_state.ball_y,
                    .w = ball_width,
                    .h = ball_height,
                };

                SDL_FPoint ball_center = {
                    (int)(ball_width / 2.0f),
                    (int)(ball_height / 2.0f)
                };

                // SDL_RenderTexture(rend, ball, nullptr, &ball_dest);
                SDL_RenderTextureRotated(rend, ball, nullptr, &ball_dest, ball_state.ball_x, &ball_center, SDL_FLIP_NONE);
                // ball
                
                // mouse
                float mx, my;
                SDL_GetMouseState(&mx, &my);
                // mouse

                // character
                float distance = mx - character_x;
                character_x += distance * .20; // easing
                               
                if (distance > 0.5) {
                    direction = 0;
                }
                else if (distance < -0.5) {
                    direction = 1;
                }

                int character_frame = 0;
                bool is_running = std::fabsf(distance) > 5.0; // IDK seems good
                
                if (is_running) {
                    active_character_texture = character;
                    character_frame = (int)(SDL_GetTicks() / 50) % 4; // IDK seems good
                    active_character_width = character_width;
                    active_character_height = character_height;
                }
                else {
                    active_character_texture = idle_character;
                    direction = 0;
                    character_frame = (int)(SDL_GetTicks() / 100) % 4; // IDK seems good

                }

                SDL_FRect character_src = {
                    .x = character_frame * character_width,
                    .y = direction * character_height,
                    .w = character_width,
                    .h = active_character_height,
                };

                SDL_FRect character_dest = {
                    .x = character_x - (character_width / 2.0),
                    .y = character_y,
                    .w = character_width,
                    .h = active_character_height,
                };

                SDL_RenderTexture(rend, active_character_texture, &character_src, &character_dest);
                // character

                // font
                draw_font(rend, text, font, 336, 30);
                draw_font(rend, score_text, font, 98, 75);
                draw_font(rend, best_text, font, 88, 120);
                
                char score_number[16];
                SDL_snprintf(score_number, sizeof(score_number), "%d", score);
                draw_font(rend, score_number, font, 200, 75);
                
                char best_number[16];
                SDL_snprintf(best_number, sizeof(best_number), "%d", best_score);
                draw_font(rend, best_number, font, 200, 120);
                // font

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