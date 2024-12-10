#include "../include/Player.h"


Player::Player(SDL_Renderer* renderer) : playerRenderer(renderer), playerSurface(nullptr), playerTexture(nullptr),
turboRenderer(renderer), turboSurface(nullptr), turboTexture(nullptr), turbo(false){ //用于初始化自机，把图片拷贝到Texture
    playerSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/myPlane.png", "rb")); //让Surface打开一张图片
    if (playerSurface == nullptr) {
        exit(-1);
    }

    playerWidth = playerSurface->w;
    playerHeight = playerSurface->h; //获取自机图片的宽和高

    playerTexture = SDL_CreateTextureFromSurface(playerRenderer, playerSurface); //把Surface拷贝到Texture
    SDL_FreeSurface(playerSurface);
    playerSurface = nullptr;
}

Player::~Player() {
    if (playerTexture != nullptr) {
        SDL_DestroyTexture(playerTexture);
    }

    if (turboTexture != nullptr) {
        SDL_DestroyTexture(turboTexture);
    }
}

void Player::turboRender(SDL_Renderer* renderer, int turboCount) {
    if (turboCount == 0) {
        turboSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/turbo/turbo0.png", "rb")); //让Surface打开一张图片
        if (turboSurface == nullptr) {
            exit(-1);
        }
    } else {
        turboSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/turbo/turbo1.png", "rb")); //让Surface打开一张图片
        if (turboSurface == nullptr) {
            exit(-1);
        }
    }


    turboWidth = turboSurface->w;
    turboHeight = turboSurface->h; //获取自机图片的宽和高

    turboTexture = SDL_CreateTextureFromSurface(renderer, turboSurface); //把Surface拷贝到Texture
    SDL_FreeSurface(turboSurface);
    turboSurface = nullptr;
}

void Player::render(Sint32 x, Sint32 y) {
    SDL_ShowCursor(true); //选择在渲染器上不显示鼠标
    static uint32_t startTime = SDL_GetTicks();
    uint32_t stopTime = SDL_GetTicks();
    static int turboCount = 0;

    if (stopTime - startTime >= 100) {
        turboCount = (turboCount + 1) % 2;
        startTime = stopTime;
    }

    SDL_Rect playerRect = {x, y, playerWidth, playerHeight}; //自机的矩形信息
    SDL_RenderCopy(playerRenderer, playerTexture, nullptr, &playerRect); //复制整个图片，并在规定的位置（x,y)渲染

    if (turbo == true) {
        turboRender(turboRenderer, turboCount);
        SDL_Rect turboRect = {x + playerWidth/2 - turboWidth/2, y + playerHeight/2 - turboHeight/2, turboWidth, turboHeight}; //自机的矩形信息
        SDL_RenderCopy(turboRenderer, turboTexture, nullptr, &turboRect);
    }
}

