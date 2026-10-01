#ifndef __FRAMERATE_H__
#define __FRAMERATE_H__

#include <SDL3/SDL.h>

class FrameRate {
    public:
        FrameRate(unsigned fps = 30);
        void set_fps(unsigned fps);
        unsigned get_fps();
        void delay();

    private:
        unsigned fps_;
        Uint64 start_;
        Uint64 frame_ns_;
};



#endif /* __FRAMERATE_H__ */