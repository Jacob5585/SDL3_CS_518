#include <SDL3/SDL.h>

#include "framerate.h"

FrameRate::FrameRate(unsigned fps) {
    set_fps(fps);

    return;
}

unsigned FrameRate::get_fps() {
    return fps_;
}

void FrameRate::set_fps(unsigned fps) {
    fps_ = fps;
    
    if (fps > 0) {
        frame_nano_seconds_ = 1'000'000'000 / fps_;
        start_ = SDL_GetTicksNS();
    }

    return;
}

void FrameRate::delay() {
    if (fps_ > 0) {
        Uint64 ns_passed = SDL_GetTicksNS() - start_;

        if (frame_nano_seconds_ > ns_passed) {
            Uint64 nano_left = frame_nano_seconds_ - ns_passed;
            SDL_DelayPrecise(nano_left);
            start_ = SDL_GetTicksNS();
        }
        
    }

    return;
}