#ifndef BULLET_H
#define BULLET_H

#include "common.h"
#include "Player.h"



class Bullet {
public:
    Bullet(SDL_Renderer *renderer);

    ~Bullet();

    void render(SDL_Renderer *renderer, int x, int y, Player *player); //用于渲染

    void start(); //用于开始计时

    std::vector<SDL_Rect> bulletPosition;
    int width;
    int height; //宽度和高度

    bool attack; //用于表示是否在发射子弹

protected: //可以设计子类访问
    SDL_Surface *bulletSurface;
    SDL_Texture *bulletTexture;
    FILE *myLog;

    std::default_random_engine generator;

    uint32_t startTime; //因为Get_ticks函数用的是uint32类型

};





#endif //BULLET_H
