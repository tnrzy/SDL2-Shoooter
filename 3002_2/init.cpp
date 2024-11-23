//
// Created by Chenyu Zhang on 2024/11/15.
//
#include <iostream>
#include "common.h"
#include "init.h"
#include <SDL2_image/SDL_image.h>
//App app;
void initSDL(){
    int rendererflags, windowflags;
    rendererflags = SDL_RENDERER_ACCELERATED;
    windowflags = 0;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    app.window = SDL_CreateWindow("shooter01",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                                  Screen_width,Screen_height,windowflags);
    if (app.window == nullptr){
        std::cerr << "SDL_Create_Window Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    app.renderer = SDL_CreateRenderer(app.window, -1,rendererflags);
    if (app.renderer == nullptr) {
        std::cerr << "SDL_Create_Renderer Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}
void cleanup(){
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}
