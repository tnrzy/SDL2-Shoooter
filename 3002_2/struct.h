//
// Created by Chenyu Zhang on 2024/11/14.
//

#ifndef INC_3002_2_STRUCT_H
#define INC_3002_2_STRUCT_H
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "defs.h"

struct Delegate{
    void (*logic)();
    void (*draw)();
};
struct App{
    SDL_Renderer *renderer;
    SDL_Window *window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
};
struct Entity{
    float x;
    float y;
    int w;
    int h;
    float dx;
    float dy;
    int health;
    int reload;
    SDL_Texture *texture;
    Entity * next;
    //Entity();
};

/*Entity::Entity() {
    x = y = dy = dx = 0.0;
    texture = nullptr;
    next = nullptr;
    w = h = health = reload = 0;
}*/

struct Stage{
    Entity fighterHead, *fighterTail;
    Entity bulletHead, *bulletTail;
};
extern App app;
extern Stage stage;
#endif //INC_3002_2_STRUCT_H
