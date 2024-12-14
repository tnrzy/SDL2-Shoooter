#include "../include/common.h"
#include "../include/bullet.h"

Bullet::Bullet(SDL_Renderer *renderer) : bulletSurface(nullptr), bulletTexture(nullptr), attack(false), firePoint(1){
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


    if (magic_point >= 0 && magic_point < 3) {
        if (fireCount == 0) {
            bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire/Fire0.png", "rb"));
        } else if (fireCount == 1) {
            bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire/Fire1.png", "rb"));
        } else if (fireCount == 2) {
            bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire/Fire2.png", "rb"));
        } else if (fireCount == 3) {
            bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire/Fire3.png", "rb"));
        }
        if (!bulletSurface) {
            fprintf(myLog, "SDL_Bullet_Surface Error: %s\n", IMG_GetError());
            exit(-5);
        }
    } else {
        if (fireCount == 0) {
            bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire/Fire4.png", "rb"));
        } else if (fireCount == 1) {
            bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire/Fire5.png", "rb"));
        } else if (fireCount == 2) {
            bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire/Fire6.png", "rb"));
        } else if (fireCount == 3) {
            bulletSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/Fire/Fire7.png", "rb"));
        }
        if (!bulletSurface) {
            fprintf(myLog, "SDL_Bullet_Surface Error: %s\n", IMG_GetError());
            exit(-5);
        }
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
    double piece=500/(0.5+0.5*int(magic_point));
    if (player->playerType == 0) {
        if (stopTime - startTime >= piece) { //每个ms刷新一次子弹；渲染的时间可能大于ms，所以用大于号
            SDL_Rect bulletRect;
            SDL_Rect bulletRect1;
            SDL_Rect bulletRect2;





            bulletRect = {x + (player->playerWidth/2)-width/2, y,width,height};


            if (attack == true) {

                bulletPosition.push_back(bulletRect);

            }
            startTime = stopTime; //刷新一次子弹后，startTime会被更新一次
        }
        if (!bulletPosition.empty()) { //读取并生成子弹
            int dy = 5*(0.5+0.5*int(magic_point)); //子弹的速度
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
            if (magic_point >= 1 && magic_point < 2) {
                firePoint = 1;
            } else if (magic_point >= 2 && magic_point < 3) {
                firePoint = 2;
            } else if (magic_point >= 3 && magic_point <= 4) {
                firePoint = 2;
            }
            FireRender(renderer, fireCount);
            SDL_Rect bulletRect = {x + (player->playerWidth/2) - width*firePoint/2, y - height*firePoint,width*firePoint,height*firePoint};
            SDL_RenderCopy(renderer, bulletTexture, NULL, &bulletRect);
        }
    }
    if (player->playerType == 2) {
        if (attack == true) {
            LightRender(renderer);
            if (magic_point<3) {
                SDL_Rect skillRect = {x + (player->playerWidth/2)-width/16*(7+int(magic_point)), 0,width/8*(7+int(magic_point)),y};
                SDL_RenderCopy(renderer, bulletTexture, NULL, &skillRect);
            }else {
                SDL_Rect skillRect1 = {x -width/16*(7+int(magic_point)), 0,width/8*(7+int(magic_point)),y};
                SDL_RenderCopy(renderer, bulletTexture, NULL, &skillRect1);
                SDL_Rect skillRect2 = {x + (player->playerWidth)-width/16*(7+int(magic_point)), 0,width/8*(7+int(magic_point)),y};
                SDL_RenderCopy(renderer, bulletTexture, NULL, &skillRect2);
            }

        }
    }
}

void Bullet::start() {
    startTime = SDL_GetTicks(); //返回自初始化经过的毫秒数
}