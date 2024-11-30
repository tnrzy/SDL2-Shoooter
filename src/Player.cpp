#include "../include/Player.h"


Player::Player(SDL_Renderer* renderer) : playerRenderer(renderer), playerSurface(nullptr), playerTexture(nullptr) { //用于初始化自机，把图片拷贝到Texture
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
}

void Player::render(Sint32 x, Sint32 y) {
    SDL_ShowCursor(false); //选择在渲染器上不显示鼠标

    SDL_Rect playerRect = {x, y, playerWidth, playerHeight}; //自机的矩形信息
    SDL_RenderCopy(playerRenderer, playerTexture, nullptr, &playerRect); //复制整个图片，并在规定的位置（x,y)渲染
}

