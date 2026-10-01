#include <SDL3/SDL.h>

#include "framerate.h"

FrameRate::FrameRate(unsigned fps) {
    set_fps(fps);

    return;
}

void FrameRate::set_fps(unsigned fps) {
    fps_ = fps;
    if(fps > 0) {
        frame_ns_ = 1'000'000'000 / fps;
        start_ = SDL_GetTicksNS();
    }

    return;
}

unsigned FrameRate::get_fps() {
    return fps_;
}

void FrameRate::delay() {
    if(fps_ > 0) {
        Uint64 ns_passed = SDL_GetTicksNS() - start_;
        if(frame_ns_ > ns_passed) {
            Uint64 ns_left = frame_ns_ - ns_passed;
            SDL_DelayPrecise(ns_left);

            start_ = SDL_GetTicksNS();
        }
    }

    return;
}