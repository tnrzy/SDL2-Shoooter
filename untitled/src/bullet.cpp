#include "../include/common.h"
#include "../include/bullet.h"

Bullet::Bullet(SDL_Renderer *renderer) : bulletSurface(nullptr), bulletTexture(nullptr), attack(false), firePoint(2){
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

void Bullet::FireRender (SDL_Renderer *renderer, int fireCount) {

    if (fireCount == 0) {
        bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire0.png", "rb"));
    } else if (fireCount == 1) {
        bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire1.png", "rb"));
    } else if (fireCount == 2) {
        bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire2.png", "rb"));
    } else if (fireCount == 3) {
        bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire3.png", "rb"));
    }
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
void Bullet::LightRender (SDL_Renderer *renderer) {


    bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Light.png", "rb"));
    if (!bulletSurface) {
        fprintf(myLog, "SDL_Bullet_Surface Error: %s\n", IMG_GetError());
        exit(-5);
    }


    width = bulletSurface->w;
    height = bulletSurface->h;

    bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);
    bulletSurface = nullptr;
}

void Bullet::render(SDL_Renderer *renderer, int x, int y, Player *player,double magic_point) { //需要获取窗口的宽度；需要随机刷新敌机
    uint32_t stopTime = SDL_GetTicks(); //stopTime随着call该render函数，每次都在更新
    static int fireCount = 0;
    double piece=400/magic_point;
    if (player->playerType == 0) {
        if (stopTime - startTime >= piece) { //每个ms刷新一次子弹；渲染的时间可能大于ms，所以用大于号
            SDL_Rect bulletRect = {x + (player->playerWidth/2)-width/2, y,width,height}; //x,y是自机的坐标
            if (attack == true) {
                bulletPosition.push_back(bulletRect);
            }
            startTime = stopTime; //刷新一次子弹后，startTime会被更新一次
        }
        if (!bulletPosition.empty()) { //读取并生成子弹
            int dy = 5*magic_point; //子弹的速度
            for (int i = 0; i < bulletPosition.size(); i++) {
                bulletPosition[i].y -= dy;
                if (bulletPosition[i].y < 0) {
                    bulletPosition.erase(bulletPosition.begin() + i);
                    continue;
                }
                SDL_Rect rect = bulletPosition[i];
                SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
            }
        }
    }


    if (player->playerType == 1) {
        if (stopTime - startTime >= 50) {
            fireCount = (fireCount + 1) % 4;
            startTime = stopTime;
        }
        if (attack == true) {
            FireRender(renderer, fireCount);
            SDL_Rect bulletRect = {x + (player->playerWidth/2) - width*firePoint/2, y - height*firePoint,width*firePoint,height*firePoint};
            SDL_RenderCopy(renderer, bulletTexture, NULL, &bulletRect);
        }
    }
    if (player->playerType == 2) {
        if (attack == true) {
            LightRender(renderer);
            SDL_Rect skillRect = {x + (player->playerWidth/2)-width/2, 0,width,y};
            SDL_RenderCopy(renderer, bulletTexture, NULL, &skillRect);
        }
    }
}

void Bullet::start() {
    startTime = SDL_GetTicks(); //返回自初始化经过的毫秒数
}