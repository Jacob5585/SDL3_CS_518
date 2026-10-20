#include <SDL3/SDL.h>
#include <SDL3_gfx/SDL3_gfxPrimitives.h>

// 1916 mercedes logo
void draw_mercedes(SDL_Renderer *rend) {
    boxRGBA(rend, 0, 0, 500, 500, 245, 245, 245, 255);

    // 
    filledCircleColor(rend, 250, 250, 250, 0xFF00D7FF);
    filledCircleColor(rend, 250, 250, 240, 0xFF000000);
    filledCircleColor(rend, 250, 250, 175, 0xFF00D7FF);
    filledCircleColor(rend, 250, 250, 165, 0xFF0000FF);

    // 
    float vx[6] = {250, 60, 215, 250, 285, 450};
    float vy[6] = {325, 400, 225, 10, 225, 400};
    filledPolygonRGBA(rend, vx, vy, 6, 255, 255, 255, 255);

    // 
    thickLineColor(rend, 250, 325, 250, 10, 5, 0xFF00D7FF);
    thickLineColor(rend, 285, 225, 60, 400, 5, 0xFF00D7FF);
    thickLineColor(rend, 215, 225, 450, 400, 5, 0xFF00D7FF);
    
    thickLineColor(rend, 250, 325, 60, 400, 5, 0xFF00D7FF);
    thickLineColor(rend, 60, 400, 215, 225, 5, 0xFF00D7FF);
    thickLineColor(rend, 215, 225, 250, 10, 5, 0xFF00D7FF);
    thickLineColor(rend, 250, 10, 285, 225, 5, 0xFF00D7FF);
    thickLineColor(rend, 285, 225, 450, 400, 5, 0xFF00D7FF);
    thickLineColor(rend, 450, 400, 250, 325, 5, 0xFF00D7FF);

    //
    float vx2[6] = {55.8, 62.55, 46.8, 27.45, 46.8, 62.55};
    float vy2[6] = {250.0, 232.9, 246.85, 250.0, 253.15, 268.0};
    filledPolygonRGBA(rend, vx2, vy2, 6, 255, 215, 0, 255);

    float vx3[6] = {112.64, 129.5, 108.5, 92.6, 104.0, 104.7};
    float vy3[6] = {112.64, 105.3, 104.0, 92.6, 108.5, 130.1};
    filledPolygonRGBA(rend, vx3, vy3, 6, 255, 215, 0, 255);

    float vx4[6] = {444.2, 437.45, 453.2, 472.55, 453.2, 437.45};
    float vy4[6] = {250.0, 267.1, 253.15, 250.0, 246.85, 232.0};
    filledPolygonRGBA(rend, vx4, vy4, 6, 255, 215, 0, 255);

    float vx5[6] = {387.36, 394.7, 396.0, 402.4, 391.5, 369.9};
    float vy5[6] = {112.64, 129.5, 108.5, 92.6, 104.0, 104.7};
    filledPolygonRGBA(rend, vx5, vy5, 6, 255, 215, 0, 255);

    // 
    // Letter M
    thickLineRGBA(rend, 122, 432, 122, 412, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 122, 412, 130, 425, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 130, 425, 138, 412, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 138, 412, 138, 432, 3, 255, 215, 0, 255);

    // Letter E
    thickLineRGBA(rend, 156, 431, 156, 451, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 156, 431, 170, 431, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 156, 441, 167, 441, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 156, 451, 170, 451, 3, 255, 215, 0, 255);

    // Letter R
    thickLineRGBA(rend, 192, 444, 192, 464, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 192, 444, 206, 444, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 206, 444, 206, 454, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 206, 454, 192, 454, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 200, 454, 210, 464, 3, 255, 215, 0, 255);

    // Letter C
    thickLineRGBA(rend, 246, 450, 230, 450, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 230, 450, 230, 470, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 230, 470, 246, 470, 3, 255, 215, 0, 255);

    // Letter E
    thickLineRGBA(rend, 268, 448, 268, 468, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 268, 448, 282, 448, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 268, 458, 279, 458, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 268, 468, 282, 468, 3, 255, 215, 0, 255);

    // Letter D
    thickLineRGBA(rend, 304, 437, 304, 457, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 304, 437, 318, 442, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 318, 442, 318, 452, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 318, 452, 304, 457, 3, 255, 215, 0, 255);

    // Letter E
    thickLineRGBA(rend, 338, 420, 338, 440, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 338, 420, 352, 420, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 338, 430, 349, 430, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 338, 440, 352, 440, 3, 255, 215, 0, 255);

    // Letter S
    thickLineRGBA(rend, 380, 406, 362, 406, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 362, 406, 362, 415, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 362, 415, 380, 415, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 380, 415, 380, 424, 3, 255, 215, 0, 255);
    thickLineRGBA(rend, 380, 424, 362, 424, 3, 255, 215, 0, 255);

    stringRGBA(rend, 225, 375, "MERCEDES", 0, 0, 0, 255);
}

void draw_nasa(SDL_Renderer *rend) {
    boxRGBA(rend, 500, 0, 1000, 500, 245, 245, 245, 255);

    filledCircleRGBA(rend, 750, 250, 200, 0, 0, 255, 255);

    filledCircleRGBA(rend, 700, 150, 2, 255, 255, 255, 255);
    filledCircleRGBA(rend, 800, 180, 2, 255, 255, 255, 255);
    filledCircleRGBA(rend, 650, 220, 2, 255, 255, 255, 255);
    filledCircleRGBA(rend, 830, 280, 2, 255, 255, 255, 255);
    filledCircleRGBA(rend, 720, 340, 2, 255, 255, 255, 255);
    filledCircleRGBA(rend, 780, 200, 2, 255, 255, 255, 255);
    filledCircleRGBA(rend, 680, 300, 2, 255, 255, 255, 255);
    filledCircleRGBA(rend, 760, 130, 2, 255, 255, 255, 255);
    filledCircleRGBA(rend, 820, 330, 2, 255, 255, 255, 255);
    filledCircleRGBA(rend, 630, 260, 2, 255, 255, 255, 255);

    // ellipse
    // top right
    float vx1[4] = {813.6, 833.2, 820.5, 785.4};
    float vy1[4] = {313.6, 294.1, 249.8, 214.6};
    bezierColor(rend, vx1, vy1, 4, 30, 0xFFFFFFFF);

    // top left
    float vx2[4] = {785.4, 750.2, 705.9, 686.4};
    float vy2[4] = {214.6, 179.5, 166.8, 186.4};
    bezierColor(rend, vx2, vy2, 4, 30, 0xFFFFFFFF);

    // bottom left
    float vx3[4] = {686.4, 666.8, 679.5, 714.6};
    float vy3[4] = {186.4, 205.9, 250.2, 285.4};
    bezierColor(rend, vx3, vy3, 4, 30, 0xFFFFFFFF);

    // bottom right
    float vx4[4] = {728.6, 759.5, 794.1, 813.6};
    float vy4[4] = {299.5, 327.9, 333.2, 313.6};
    bezierColor(rend, vx4, vy4, 4, 30, 0xFFFFFFFF);

    // First Red Curve in background
    float vx5_base[] = {500,  625,  750,  875, 1000};
    float vy5_base[] = {300,  100,  400,  150,   50};
    int n = 5;
    int s = 500;
    for (int i = 0; i < 25; i++) {

        float vx5_copy[5], vy5_copy[5];
        for (int j = 0; j < n; j++) {
            vx5_copy[j] = vx5_base[j];
            vy5_copy[j] = vy5_base[j];
        }

        vx5_copy[n-1] += i;
        vy5_copy[n-1] += i;

        bezierRGBA(rend, vx5_copy, vy5_copy, n, s, 255, 0, 0, 255);
    }

    // Letters
    // Letter N
    thickLineRGBA(rend, 600, 287, 600, 212, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 600, 287, 5, 255, 255, 255, 255);
    filledCircleRGBA(rend, 600, 212, 5, 255, 255, 255, 255);
    
    thickLineRGBA(rend, 600, 212, 660, 287, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 600, 212, 5, 255, 255, 255, 255);
    filledCircleRGBA(rend, 660, 287, 5, 255, 255, 255, 255);
    
    thickLineRGBA(rend, 660, 287, 660, 212, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 660, 287, 5, 255, 255, 255, 255);
    filledCircleRGBA(rend, 660, 212, 5, 255, 255, 255, 255);    

    // letter A
    thickLineRGBA(rend, 676, 287, 705, 212, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 676, 287, 5, 255, 255, 255, 255);
    filledCircleRGBA(rend, 705, 212, 5, 255, 255, 255, 255);

    thickLineRGBA(rend, 705, 212, 734, 287, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 705, 212, 5, 255, 255, 255, 255);
    filledCircleRGBA(rend, 734, 287, 5, 255, 255, 255, 255);
    
    // letter S
    thickLineRGBA(rend, 798, 212, 751, 212, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 798, 212, 5, 255, 255, 255, 255);
    filledCircleRGBA(rend, 751, 212, 5, 255, 255, 255, 255);

    thickLineRGBA(rend, 751, 212, 751, 250, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 751, 250, 5, 255, 255, 255, 255);

    thickLineRGBA(rend, 751, 250, 798, 250, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 798, 250, 5, 255, 255, 255, 255);

    thickLineRGBA(rend, 798, 250, 798, 287, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 798, 287, 5, 255, 255, 255, 255);

    thickLineRGBA(rend, 798, 287, 751, 287, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 751, 287, 5, 255, 255, 255, 255);
    
    // letter A
    thickLineRGBA(rend, 836, 287, 865, 212, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 836, 287, 5, 255, 255, 255, 255);
    filledCircleRGBA(rend, 865, 212, 5, 255, 255, 255, 255);

    thickLineRGBA(rend, 865, 212, 893, 287, 10, 255, 255, 255, 255);
    filledCircleRGBA(rend, 865, 212, 5, 255, 255, 255, 255);
    filledCircleRGBA(rend, 893, 287, 5, 255, 255, 255, 255);
    
    // Seconds Red Curve in background
    float vx6_base[] = {500,  625,  750,  900, 1000};
    float vy6_base[] = {475,  100,  400,  150,   50};
    int s2 = 100;
    for (int i = 0; i < 25; i++) {

        float vx6_copy[5], vy6_copy[5];
        for (int j = 0; j < n; j++) {
            vx6_copy[j] = vx6_base[j];
            vy6_copy[j] = vy6_base[j];
        }

        vx6_copy[n-1] += i;
        vy6_copy[n-1] += i;

        bezierRGBA(rend, vx6_copy, vy6_copy, n, s2, 255, 0, 0, 255);
    }
    
    stringRGBA(rend, 750, 410, "NASA", 50, 50, 50, 255);
}

int main(int argc, char *argv[]) {

    if(SDL_Init(SDL_INIT_VIDEO)) {

        SDL_Window *win = nullptr;
        SDL_Renderer *rend = nullptr;

        if(SDL_CreateWindowAndRenderer(argv[0], 1000, 500, 0, &win, &rend)) {

            SDL_Event e;

            // draw_mitsubishi(rend);
            draw_mercedes(rend);
            draw_nasa(rend);

            SDL_RenderPresent(rend);
            // 

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
                            if(k.key == SDLK_ESCAPE) {
                                quit = true;
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }

                // 

                // 
            }

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