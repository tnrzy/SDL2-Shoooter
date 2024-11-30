#include "../include/common.h"
#include "../include/bullet.h"

Bullet::Bullet(SDL_Renderer *renderer) : bulletSurface(nullptr), bulletTexture(nullptr), attack(false){
    myLog = fopen("Bullet.log", "w");
    if (!myLog) {
        exit(-1);
    }

    bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/bullet.png", "rb"));
    if (!bulletSurface) {
        fprintf(myLog, "SDL_Bullet_Surface Error: %s\n", IMG_GetError());
        exit(-5);
    }

    width = bulletSurface->w;
    height = bulletSurface->h; //得到敌机图片的宽度和高度

    bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);
    bulletSurface = nullptr;
}

Bullet::~Bullet() {
    if (myLog) {
        fclose(myLog);
    }

    if (bulletSurface) {
        SDL_FreeSurface(bulletSurface);
    }

    if (bulletTexture) {
        SDL_DestroyTexture(bulletTexture);
    }
}

void Bullet::render(SDL_Renderer *renderer, int x, int y, Player *player) { //需要获取窗口的宽度；需要随机刷新敌机
    uint32_t stopTime = SDL_GetTicks(); //stopTime随着call该render函数，每次都在更新

    if (stopTime - startTime >= 400) { //每个ms刷新一次子弹；渲染的时间可能大于ms，所以用大于号
        SDL_Rect bulletRect = {x + (player->playerWidth/2)-width/2, y,width,height}; //x,y是自机的坐标
        if (attack == true) {
            bulletPosition.push_back(bulletRect);
        }

        startTime = stopTime; //刷新一次子弹后，startTime会被更新一次
    }

    if (!bulletPosition.empty()) { //读取并生成子弹
        int dy = 5; //子弹的速度
        for (int i = 0; i < bulletPosition.size(); i++) {
            bulletPosition[i].y -= dy;
            SDL_Rect rect = bulletPosition[i];
            SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
        }
    }
}

void Bullet::start() {
    startTime = SDL_GetTicks(); //返回自初始化经过的毫秒数
}