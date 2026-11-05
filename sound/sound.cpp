/*
 * Press 1 to play a background loop or restart it
 * Press 2 to play a pig-splash
 */

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "framerate.h"

const int width = 800;
const int height = 600;

int main(int argc, char *argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {

        SDL_Window *win = nullptr;
        SDL_Renderer *rend = nullptr;

        if(SDL_CreateWindowAndRenderer(argv[0], width, height, 0, &win, &rend)) {

            SDL_AudioSpec loop_spec;
            Uint8 *loop_buf = nullptr;
            Uint32 loop_len = 0;

            SDL_LoadWAV("loop_piano.wav", &loop_spec, &loop_buf, &loop_len);
            SDL_AudioStream *loop_as = SDL_OpenAudioDeviceStream(
                SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                &loop_spec,
                nullptr,
                nullptr
            );

            SDL_AudioSpec splash_spec;
            Uint8 *splash_buf = nullptr;
            Uint32 splash_len = 0;

            SDL_LoadWAV("splash.wav", &splash_spec, &splash_buf, &splash_len);
            SDL_AudioStream *splash_as = SDL_OpenAudioDeviceStream(
                SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                &splash_spec,
                nullptr,
                nullptr
            );


            SDL_Event e;

            FrameRate fr;

            SDL_SetRenderDrawColor(rend, 0xdd, 0xbb, 0xff, SDL_ALPHA_OPAQUE);

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
                            switch(k.key){
                            case SDLK_ESCAPE:
                                quit = true;
                                break;
                            case SDLK_1:
                                SDL_ClearAudioStream(loop_as);
                                SDL_PutAudioStreamData(loop_as, loop_buf, loop_len);
                                SDL_ResumeAudioStreamDevice(loop_as);
                                break;
                            case SDLK_2:
                                SDL_ClearAudioStream(splash_as);
                                SDL_PutAudioStreamData(splash_as, splash_buf, splash_len);
                                SDL_ResumeAudioStreamDevice(splash_as);
                                break;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }

                if(SDL_GetAudioStreamAvailable(loop_as) == 0) {
                    SDL_PutAudioStreamData(loop_as, loop_buf, loop_len);
                }


                SDL_RenderClear(rend);


                SDL_RenderPresent(rend);

                fr.delay();
            }

            SDL_free(loop_buf);
            SDL_DestroyAudioStream(loop_as);
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