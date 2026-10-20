#include <array>

#include <SDL3/SDL.h>
#include <SDL3_gfxPrimitives.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("p5_conferw", 1000, 500, 0);
    SDL_Renderer* rend = SDL_CreateRenderer(win, nullptr);

    /*
     * Couldn't figure out a reliable way to get the format from the renderer
     * as we did with surfaces.  Read somewhere the GPU might handle this?
     * Sounds fishy, but at least I'm getting the colors I'm hoping for :-|
     */
    const SDL_PixelFormatDetails* pfd = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32);

    Uint32 i_blue = SDL_MapRGB(pfd, nullptr, 0x0e, 0x79, 0xbf);
    Uint32 i_red = SDL_MapRGB(pfd, nullptr, 0xe6, 0x25, 0x2b);

    Uint32 s_dark = SDL_MapRGB(pfd, nullptr, 0x00, 0x62, 0x41);

    Uint32 white = SDL_MapRGB(pfd, nullptr, 0xff, 0xff, 0xff);
    bool quit = false;
    SDL_Event e;

    while (!quit) {
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

        /*
         * Starbucks
         */

        /* left dark green square */
        boxColor(rend, 0, 0, 499, 500, s_dark);

        /* outer circle */
        filledCircleColor(rend, 250, 200, 146, white);
        filledCircleColor(rend, 250, 200, 144, s_dark);

        /* crown */
        const Sint16 crown_x[] = {191, 180, 203, 197, 248, 298, 293, 315, 303};
        const Sint16 crown_y[] = {126, 106, 112,  92, 112,  92, 112, 106, 126};
        filledPolygonColor(rend, crown_x, crown_y, 9, white);

        /* star */
        const Sint16 star_x[] = {227, 235, 222, 239, 248, 256, 273, 259, 269, 246};
        const Sint16 star_y[] = {112,  89,  75,  74,  57,  74,  75,  89, 113,  99};
        filledPolygonColor(rend, star_x, star_y, 10, white);

        /* white body, then will "erase" stuff */
        const Sint16 body_x[] = {144, 166, 183, 190, 247, 306, 312, 329, 351, 343, 314, 305, 324, 269, 224, 173, 189, 182, 153};
        const Sint16 body_y[] = {253, 244, 202, 143, 129, 143, 203, 242, 254, 280, 263, 278, 317, 336, 336, 316, 277, 263, 280};
        filledPolygonColor(rend, body_x, body_y, 19, white);

        /* face */
        filledEllipseColor(rend, 248, 166, 34, 42, s_dark);
        filledEllipseColor(rend, 248, 166, 29, 37, white);
        filledEllipseColor(rend, 228, 160, 8, 5, s_dark); // left eye
        filledEllipseColor(rend, 267, 160, 8, 5, s_dark); // right eye
        thickLineColor(rend, 240, 165, 238, 149, 8, s_dark); // left eyebrow
        thickLineColor(rend, 241, 149, 219, 149, 8, s_dark);
        thickLineColor(rend, 255, 167, 255, 149, 8, s_dark); // right eyebrow
        thickLineColor(rend, 255, 149, 278, 150, 8, s_dark);
        filledEllipseColor(rend, 248, 184, 12, 10, s_dark); //mouth
        filledTrigonColor(rend, 233, 183, 236, 175, 252, 183, white);
        filledTrigonColor(rend, 261, 183, 257, 175, 240, 183, white);

        /* 3 hairs, left toward right */
        std::array <Sint16, 18> hair_x;
        std::array <Sint16, 18> hair_y;
        hair_x = {196, 185, 185, 185, 185, 191, 191, 191, 191, 173, 173, 173, 173, 192, 192, 192, 192, 177};
        hair_y = {137, 167, 167, 167, 167, 201, 201, 201, 201, 243, 243, 243, 243, 286, 286, 286, 286, 320};
        for(int rep = 0; rep < 3; rep++) {
            for(int i = 0; i < 8; i++) {
                for(auto &val: hair_x) {
                    val++;
                }
                bezierColor(rend, hair_x.data(), hair_y.data(), 18, 20, s_dark);
            }
            for(auto &val: hair_x) {
                val += 5;
            }
            hair_y[0] -= 5;
            hair_y[17] += 5;
        }

        /* 3 hairs, right toward left */
        hair_x = {302, 311, 311, 311, 311, 303, 303, 303, 303, 324, 324, 324, 324, 302, 302, 302, 302, 319};
        hair_y = {139, 169, 169, 169, 169, 202, 202, 202, 202, 242, 242, 242, 242, 286, 286, 286, 286, 320};
        for(int rep = 0; rep < 3; rep++) {
            for(int i = 0; i < 8; i++) {
                for(auto &val: hair_x) {
                    val--;
                }
                bezierColor(rend, hair_x.data(), hair_y.data(), 18, 20, s_dark);
            }
            for(auto &val: hair_x) {
                val -= 5;
            }
            hair_y[0] -= 5;
            hair_y[17] += 5;
        }

        /* fins? */
        filledTrigonColor(rend, 113, 176, 136, 117, 159, 167, white); // left
        filledTrigonColor(rend, 130, 151, 185, 139, 159, 167, white);
        filledTrigonColor(rend, 383, 174, 337, 167, 363, 117, white); // right
        filledTrigonColor(rend, 337, 167, 311, 140, 352, 148, white);

        /* left tail? */
        thickLineColor(rend, 106, 193, 150, 183, 10, white);
        thickLineColor(rend, 107, 217, 143, 206, 10, white);
        thickLineColor(rend, 111, 238, 140, 228, 10, white);
        thickLineColor(rend, 118, 257, 142, 247, 10, white);
        thickLineColor(rend, 128, 274, 151, 265, 10, white);
        thickLineColor(rend, 137, 288, 152, 284, 10, white);
        thickLineColor(rend, 151, 302, 161, 299, 10, white);

        /* right tail? */
        thickLineColor(rend, 345, 184, 395, 193, 10, white);
        thickLineColor(rend, 352, 206, 395, 218, 10, white);
        thickLineColor(rend, 355, 227, 390, 238, 10, white);
        thickLineColor(rend, 354, 246, 381, 257, 10, white);
        thickLineColor(rend, 342, 262, 376, 273, 10, white);
        thickLineColor(rend, 342, 282, 365, 288, 10, white);
        thickLineColor(rend, 333, 298, 351, 303, 10, white);

        /* name */
        stringColor(rend, 215, 395, "STARBUCKS", white);

        /*
         * IHOP
         */

        // right white square
        boxColor(rend, 500, 0, 999, 500, white);

        /* 'I' */
        boxColor(rend, 580, 172, 605, 238, i_blue);
        filledCircleColor(rend, 592, 172, 12, i_blue);
        filledCircleColor(rend, 592, 238, 12, i_blue);
        /* 'H' */
        boxColor(rend, 630, 172, 655, 238, i_blue);
        boxColor(rend, 685, 172, 710, 238, i_blue);
        boxColor(rend, 655, 195, 685, 215, i_blue);
        filledCircleColor(rend, 642, 172, 12, i_blue);
        filledCircleColor(rend, 697, 172, 12, i_blue);
        filledCircleColor(rend, 642, 238, 12, i_blue);
        filledCircleColor(rend, 697, 238, 12, i_blue);

        /* 'O' */
        filledCircleColor(rend, 765, 205, 45, i_blue);
        filledCircleColor(rend, 765, 205, 23, white);

        /* 'P' */
        boxColor(rend, 825, 190, 850, 263, i_blue);
        filledCircleColor(rend, 837, 263, 12, i_blue);
        filledCircleColor(rend, 862, 195, 36, i_blue);
        filledCircleColor(rend, 862, 195, 17, white);

        /*
         * the smile... 16 arcs to fake a thick curve
         */
        for(int i = 0; i < 16; i++) {
            arcColor(rend, 815, 220 - i, 100, 25, 155, i_red);
        }

        /* name */
        stringColor(rend, 750 - 16, 395, "IHOP", i_blue);



        SDL_RenderPresent(rend);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
