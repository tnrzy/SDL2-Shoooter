//
// Created by Chenyu Zhang on 2024/11/15.
//
#include<SDL2/SDL.h>
#include "struct.h"
#include "draw.h"
#include <SDL2_image/SDL_image.h>
//App app;
void prepareScene(){
    SDL_SetRenderDrawColor(app.renderer,30,255,128,255);
    SDL_RenderClear(app.renderer);
}
void presentScene(){
    SDL_RenderPresent(app.renderer);
}
SDL_Texture *loadTexture(const char *filename){
    SDL_Texture *texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
    texture = IMG_LoadTexture(app.renderer, filename);
    return texture;
}
void blit(SDL_Texture *texture, int x, int y){
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(app.renderer,texture, NULL, &dest);
}