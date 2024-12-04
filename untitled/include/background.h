#ifndef BACKGROUND_H
#define BACKGROUND_H

#ifdef __WIN32
#define Main main

#endif

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include "enemy.h"
#include "Player.h"
#include "bullet.h"
#include <cstdio>
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include "drop_items.h"
#include "enemy_control.h"

enum GameState {
    START,
    END,
    PLAYING
};

class Background {
public:
    // 构造函数
    Background();

    // 析构函数
    ~Background();

    void loadJPG(char* file);
    void loadPNG(char* file);

    // 初始化函数
    void init();

    // 游戏进程函数
    void gameplay();

    // 渲染函数
    void render();
    void fontrender(int height);

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *backgroundSurface;
    SDL_Texture *backgroundTexture;

    char *imageAccess; //用于得到图像文件

    FILE *myLog;
    GameState state; //游戏运行的状态

    Enemy *minion;
    int minion_width; //minion_width是当前窗口的宽度
    int window_height;
    int width;
    Player *player;
    Sint32 playerX;
    Sint32 playerY;

    Bullet *bullet;
    drop_items *drop;
    enemy_controller *controller;
    double magic_point;
    int beated_enemy;
    int score;
};

#endif // BACKGROUND_H
