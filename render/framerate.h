#ifndef __FRAMERATE_H__
#define __FRAMERATE_H__

#include <SDL3/SDL.h>

class FrameRate {
    public:
        FrameRate(unsigned fps = 30);
        unsigned get_fps();
        void set_fps(unsigned fps);
        void delay();

    private:
        unsigned fps_;
        Uint64 start_;
        Uint64 frame_nano_seconds_;
};

#endif /* __FRAMERATE_H__ */