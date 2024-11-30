#ifndef BACKGROUND_H
#define BACKGROUND_H

#ifdef __WIN32
#define Main main

#endif

#include <SDL.h>
#include <SDL_image.h>
#include "enemy.h"
#include "Player.h"
#include "bullet.h"
#include <cstdio>

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

    Player *player;
    Sint32 playerX;
    Sint32 playerY;

    Bullet *bullet;
};

#endif // BACKGROUND_H
