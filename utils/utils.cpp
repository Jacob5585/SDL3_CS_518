#include <cstdlib>

#include <SDL3/SDL.h>

// Gaurentees random 32bit number on differnt OS
Uint32 rand32() {
    Uint32 result = rand();

    result ^= rand() << 15;
    result ^= rand() << 30;

    return result;
}