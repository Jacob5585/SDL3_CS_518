#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "framerate.h"
#include "assets.h"

const int width = 800;
const int height = 500;

bool quit = false;
bool race_finished = false;
bool race_start = false;
int winner_id = -1;
SDL_Mutex *race_mutex = nullptr;

struct Racer {
    int id;
    SDL_Color color;
    float distance_moved;
    float speed_increment;
    float (*easingFunc)(float);
    int width, height;
    int yPos;
    float eased_value;
};

static int SDLCALL racer_thread(void *data) {
    Racer *racer = (Racer *)data;

    while (true) {
        if (quit) { break; }
        SDL_LockMutex(race_mutex);
        if (race_start) { SDL_UnlockMutex(race_mutex); break; };
        SDL_UnlockMutex(race_mutex);
        SDL_Delay(1);
    }

    // const int race_interations = 500;
    const int race_interations = SDL_rand(5000);
    // const int race_interations = SDL_min(500, SDL_rand(1000));
    racer->speed_increment = 1.0 / (float)race_interations;

    for (int i = 0; i < race_interations; i++) {
        if (quit) { break; }
        
        SDL_LockMutex(race_mutex);
        if (race_finished) { SDL_UnlockMutex(race_mutex); break; }
        SDL_UnlockMutex(race_mutex);
        
        racer->distance_moved += racer->speed_increment;
        
        SDL_LockMutex(race_mutex);
        racer->eased_value = racer->easingFunc(racer->distance_moved);
        SDL_UnlockMutex(race_mutex);
        
        if (racer->eased_value >= 1.0) {
            
            SDL_LockMutex(race_mutex);
            if (!race_finished) {
                race_finished = true;
                winner_id = racer->id;
            }
            SDL_UnlockMutex(race_mutex);
            break;
        }
        SDL_Delay(100);
    }

    return 0;
}

void draw_car(SDL_Renderer *rend, float x, float y, int width, int height, SDL_Color color) {
    SDL_FRect car = {
        x,
        y + 4.0,
        width, 
        height - 8.0,
    };
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(rend, &car);
}

void draw_track(SDL_Renderer *rend, int num_racers, int height) {
    SDL_SetRenderDrawColor(rend, 45, 45, 50, 255);
    SDL_RenderClear(rend);

    int laneHeight = height / (num_racers + 1);

    // Lane divider dashed lines
    SDL_SetRenderDrawColor(rend, 200, 200, 200, 100);
    for (int i = 1; i <= num_racers; i++) {
        for (int x = 50; x < 740; x += 30) {
            SDL_FRect dash = { (float)x, (float)(i * laneHeight - 2), 15.0, 2.0 };
            SDL_RenderFillRect(rend, &dash);
        }
    }

    // Start Line
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_FRect startLine = { 80.0, 20.0, 6.0, (float)(height - 40) };
    SDL_RenderFillRect(rend, &startLine);

    // Finish Line
    SDL_SetRenderDrawColor(rend, 220, 40, 40, 255);
    SDL_FRect finishLine = { 720.0, 20.0, 6.0, (float)(height - 40) };
    SDL_RenderFillRect(rend, &finishLine);
}

// easing
float ease1(float x) {
    return x < 0.5f ? 16 * x * x * x * x * x : 1.0 - SDL_powf(-2 * x + 2, 5) / 2.0;
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

float easeOutElastic(float x) {
    const float c4 = (2 * SDL_PI_F) / 3;

    if (x == 0) {
        return x;
    }
    else if (x == 1) {
        return x;
    }
    else {
        return SDL_pow(2, -10 * x) * SDL_sin((x * 10 - 0.75) * c4) + 1;
    }
}

float easeInElastic(float x) {
    const float c4 = (2 * SDL_PI_F) / 3;

    if (x == 0) {
        return x;
    }
    else if (x == 1) {
        return x;
    }
    else {
        return -SDL_pow(2, 10 * x - 10) * SDL_sin((x * 10 - 10.75) * c4);
    }
}

float easeInQuart(float x) {
    return x * x * x * x;
}

float easeInBounce(float x) {
    return 1 - ease2(1 - x);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {

        SDL_Window *win = nullptr;
        SDL_Renderer *rend = nullptr;

        if (SDL_CreateWindowAndRenderer(argv[0], width, height, 0, &win, &rend))
        {
            SDL_Event e;

            FrameRate fr;

            SDL_SetRenderDrawColor(rend, 0xdd, 0xbb, 0xff, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(rend);

            // init
            const int NUM_RACERS = 6;
            SDL_Color colors[NUM_RACERS] = {
                {230, 50,  50,  255},   // Red
                {50,  150, 255, 255},   // Blue
                {50,  220, 50,  255},   // Green
                {255, 165, 0,   255},   // Orange
                {200, 50,  250, 255},   // Purple
                {0,   220, 220, 255},   // Cyan
            };

            float (*easingList[NUM_RACERS])(float) = { ease1, ease2, easeOutElastic, easeInElastic, easeInQuart, easeInBounce };
            for (int i = 5; i > 0; i--) {
                int j = SDL_rand(100) % (i + 1);
                auto temp = easingList[i];
                easingList[i] = easingList[j];
                easingList[j] = temp;
            }

            Racer racers[NUM_RACERS];
            int lane = height / (NUM_RACERS + 1);
            for (int i = 0; i < NUM_RACERS; i++) {
                racers[i] = {
                    .id = i,
                    .color = colors[i],
                    .distance_moved = 0.0,
                    .speed_increment = 0.0,
                    .easingFunc = easingList[i],
                    .width = 60 + (i * 5),
                    .height = 24,
                    .yPos = (i + 1) * lane,
                    .eased_value = 0,
                };
            }
            // init

            // audio
            SDL_AudioSpec start_race_spec;
            Uint8 *start_race_buf = nullptr;
            Uint32 start_race_len = 0;
            
            SDL_AudioSpec race_loop_spec;
            Uint8 *race_loop_buf = nullptr;
            Uint32 race_loop_len = 0;
            
            SDL_AudioSpec finish_race_spec;
            Uint8 *finish_race_buf = nullptr;
            Uint32 finish_race_len = 0;
            
            // SDL_LoadWAV("splash.wav", &start_race_spec, &start_race_buf, &start_race_len);
            SDL_LoadWAV_IO(
                SDL_IOFromMem((void *)race_start_wav, race_start_wav_len),
                true,
                &start_race_spec,
                &start_race_buf,
                &start_race_len
            );

            SDL_AudioStream *start_race_as = SDL_OpenAudioDeviceStream(
                SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                &start_race_spec,
                nullptr,
                nullptr
            );

            // SDL_LoadWAV("loop_piano.wav", &race_loop_spec, &race_loop_buf, &race_loop_len);
            SDL_LoadWAV_IO(
                SDL_IOFromMem((void *)race_loop_wav, race_loop_wav_len),
                true,
                &race_loop_spec,
                &race_loop_buf,
                &race_loop_len
            );

            SDL_AudioStream *race_loop_as = SDL_OpenAudioDeviceStream(
                SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                &race_loop_spec,
                nullptr,
                nullptr
            );

            // SDL_LoadWAV("splash.wav", &finish_race_spec, &finish_race_buf, &finish_race_len);
            SDL_LoadWAV_IO(
                SDL_IOFromMem((void *)race_finish_wav, race_finish_wav_len),
                true,
                &finish_race_spec,
                &finish_race_buf,
                &finish_race_len
            );

            SDL_AudioStream *finish_race_as = SDL_OpenAudioDeviceStream(
                SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                &finish_race_spec,
                nullptr,
                nullptr
            );
            // audio
            
            // threading
            race_mutex = SDL_CreateMutex();

            SDL_Thread *threads[NUM_RACERS];            
            for (int i = 0; i < NUM_RACERS; i++) {
                threads[i] = SDL_CreateThread(racer_thread, "racer_thread", &racers[i]);
            }
            SDL_LockMutex(race_mutex);
            race_start = true;
            SDL_UnlockMutex(race_mutex);
            // threading
            
            SDL_ClearAudioStream(start_race_as);
            SDL_PutAudioStreamData(start_race_as, start_race_buf, start_race_len);
            SDL_ResumeAudioStreamDevice(start_race_as);

            SDL_ClearAudioStream(race_loop_as);
            SDL_PutAudioStreamData(race_loop_as, race_loop_buf, race_loop_len);
            SDL_ResumeAudioStreamDevice(race_loop_as);

            while (!quit)
            {
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

                if(SDL_GetAudioStreamAvailable(race_loop_as) == 0 && winner_id == -1) {
                    SDL_PutAudioStreamData(race_loop_as, race_loop_buf, race_loop_len);
                }

                SDL_SetRenderDrawColor(rend, 0xdd, 0xbb, 0xff, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(rend);
                draw_track(rend, NUM_RACERS, height);

                // Race
                float startX = 86.0;
                float finishX = 720.0;
                float totalTrackDistance = finishX - startX;

                for (int i = 0; i < NUM_RACERS; i++) {
                
                    SDL_LockMutex(race_mutex);
                    float currentX = startX + (float)(racers[i].eased_value * totalTrackDistance) - racers[i].width;
                    currentX = SDL_max(startX - racers[i].width, SDL_min(currentX, finishX - racers[i].width));

                    draw_car(rend, currentX, racers[i].yPos - 12, racers[i].width, racers[i].height, racers[i].color);
                    SDL_UnlockMutex(race_mutex);
                }

                if (race_finished && winner_id != -1) {
                    SDL_LockMutex(race_mutex);
                    float winner_eased = racers[winner_id].eased_value;
                    float width = racers[winner_id].width;
                    float height = racers[winner_id].height;
                    float yPos = racers[winner_id].yPos;
                    SDL_UnlockMutex(race_mutex);

                    float currentX = startX + (float)(winner_eased * totalTrackDistance) - width;
                    currentX = SDL_max(startX - width, SDL_min(currentX, finishX - width));

                    SDL_Color final_color;
                    if (((SDL_GetTicks() / 150) % 2) == 0) {
                        final_color = {255, 255, 255, 255}; // White flash
                    } else {
                        final_color = {255, 215, 0, 255};   // Gold flash
                    }

                    draw_car(rend, currentX, yPos - 12, width, height, final_color);

                    // Banner background behind the trophy
                    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
                    SDL_FRect bannerRect = {340.0, 40.0, 120.0, 50.0};
                    SDL_SetRenderDrawColor(rend, 15, 15, 15, 180); // Dark semi-transparent box
                    SDL_RenderFillRect(rend, &bannerRect);
                    SDL_SetRenderDrawColor(rend, 255, 215, 0, 220); // Gold border
                    SDL_RenderRect(rend, &bannerRect);

                    SDL_FRect trophyCup   = {390.0, 48.0, 20.0, 20.0};
                    SDL_RenderFillRect(rend, &trophyCup);
                    SDL_FRect trophyBase  = {394.0, 68.0, 12.0, 5.0};
                    SDL_RenderFillRect(rend, &trophyBase);
                    SDL_FRect trophyStand = {384.0, 73.0, 32.0, 4.0};
                    SDL_RenderFillRect(rend, &trophyStand);

                    SDL_PauseAudioStreamDevice(race_loop_as);
                    SDL_ClearAudioStream(race_loop_as);
                    if (((SDL_GetTicks() / 100) % 2) == 0) {
                        SDL_ClearAudioStream(finish_race_as);
                        SDL_PutAudioStreamData(finish_race_as, finish_race_buf, finish_race_len);
                        SDL_ResumeAudioStreamDevice(finish_race_as);
                    }
                }
                // Race

                SDL_RenderPresent(rend);

                fr.delay();
            }
           
            for (int i = 0; i < NUM_RACERS; i++) {
                SDL_WaitThread(threads[i], nullptr);
            }
            SDL_DestroyMutex(race_mutex);
            SDL_free(race_loop_buf);
            SDL_DestroyAudioStream(race_loop_as);
            SDL_free(start_race_buf);
            SDL_DestroyAudioStream(start_race_as);
            SDL_free(finish_race_buf);
            SDL_DestroyAudioStream(finish_race_as);
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