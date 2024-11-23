//
// Created by Chenyu Zhang on 2024/11/15.
//
#include "common.h"
#include "input.h"
//App app;
void doinput(){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                exit(1);
                break;
            case SDL_KEYDOWN:
                doKeyDown(&event.key);
                break;
            case SDL_KEYUP:
                doKeyUp(&event.key);
                break;
            default:
                break;
        }
    }
}
void doKeyUp(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[event->keysym.scancode] = 0;
    }
}

void doKeyDown(SDL_KeyboardEvent *event)
{
    if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[event->keysym.scancode] = 1;
    }
}
/*void dokeypress(SDL_KeyboardEvent *event){
    if ((*event).repeat == 0){
        if ((*event).keysym.scancode == SDL_SCANCODE_UP){
            app.up = 1;
        }
        if ((*event).keysym.scancode == SDL_SCANCODE_DOWN){
            app.down = 1;
        }
        if ((*event).keysym.scancode == SDL_SCANCODE_LEFT){
            app.left = 1;
        }
        if ((*event).keysym.scancode == SDL_SCANCODE_RIGHT){
            app.right = 1;
        }
        if ((*event).keysym.scancode == SDL_SCANCODE_SPACE){
            app.fire = 1;
        }
    }
}
void dokeyrelease(SDL_KeyboardEvent *event){
    if ((*event).repeat == 0){
        if ((*event).keysym.scancode == SDL_SCANCODE_UP){
            app.up = 0;
        }
        if ((*event).keysym.scancode == SDL_SCANCODE_DOWN){
            app.down = 0;
        }
        if ((*event).keysym.scancode == SDL_SCANCODE_LEFT){
            app.left = 0;
        }
        if ((*event).keysym.scancode == SDL_SCANCODE_RIGHT){
            app.right = 0;
        }
        if ((*event).keysym.scancode == SDL_SCANCODE_SPACE){
            app.fire = 0;
        }
    }
}*/