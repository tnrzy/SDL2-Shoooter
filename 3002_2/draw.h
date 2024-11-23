//
// Created by Chenyu Zhang on 2024/11/15.
//

#ifndef INC_3002_2_DRAW_H
#define INC_3002_2_DRAW_H
#include<SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
void prepareScene();
void presentScene();
SDL_Texture *loadTexture(const char *filename);
void blit(SDL_Texture *texture, int x, int y);
#endif //INC_3002_2_DRAW_H
