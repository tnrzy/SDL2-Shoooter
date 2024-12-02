#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"



class Player {
public:
    Player(SDL_Renderer* renderer);

    ~Player();

    void render(Sint32 x, Sint32 y); //x,y为鼠标的横纵坐标

    int playerWidth; //自机图片的宽和高, 并且要让敌机知道否则无法判断；是以左上角为原点计算
    int playerHeight;
    int playerType; //说明这是哪种飞机

protected:
    SDL_Surface *playerSurface;
    SDL_Texture *playerTexture;
    SDL_Renderer *playerRenderer;



};



#endif //PLAYER_H
