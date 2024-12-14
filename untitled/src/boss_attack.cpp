//
// Created by Moyuan on 24-12-13.
//
#include "../include/Boss_attack.h"
#include <vector>
#include "../include/common.h"
#include "../include/background.h"

#define FRAMERATE 60
Boss_attack::Boss_attack(SDL_Renderer *renderer) : minionSurface(nullptr), minionTexture(nullptr) {
    myLog = fopen("Boss_attack.log", "w");
    if (!myLog) {
        exit(-1);
    }
    positions.reserve(100);
    for (int i = 1; i <= type_num; i++){
        widths[i].reserve(20);
        heights[i].reserve(20);
        widths[i].push_back(0);
        heights[i].push_back(0);
        miniontextures[i].push_back(nullptr);
        for (int j = 1; ; j++) {
            std::string path = "res/png/boss/attack/"+ std::to_string(i)+"/" + std::to_string(j) + ".png";
            SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(),"rb"));
            if (!surf) {
                fprintf(myLog, "SDL_Boss_attack_Surface Error: %s\n", IMG_GetError());
                break;
            }
            widths[i].push_back(surf->w);
            heights[i].push_back(surf->h);

            miniontextures[i].push_back(SDL_CreateTextureFromSurface(renderer, surf));
            SDL_FreeSurface(surf);
            surf= nullptr;
        }
    }
}

Boss_attack::~Boss_attack() {
    if (myLog) {
        fclose(myLog);
    }

    if (minionSurface) {
        SDL_FreeSurface(minionSurface);
    }

    if (minionTexture) {
        SDL_DestroyTexture(minionTexture);
    }
}

void Boss_attack::add_attack(int type,int x,int y,int lasting) {
    std::vector<SDL_Rect> attackRect;
    attackRect.reserve(20);
    attackRect.push_back(SDL_Rect(0,0,0,0));
    for (int i=1;i<miniontextures[type].size();i++) {
        int attackPositionX = x-widths[type][i]/2; //敌机的初始坐标
        attackRect.push_back({attackPositionX, y, widths[type][i], heights[type][i]});
    }
    attack_info *new_attack = new attack_info(type,attackRect,lasting);
    positions.push_back(new_attack);
}

void Boss_attack::render(SDL_Renderer *renderer) { //需要获取窗口的宽度；需要随机刷新敌机
    uint32_t stopTime = SDL_GetTicks(); //stopTime随着call该render函数，每次都在更新
    if (!positions.empty()){
        for(int i = 0; i < positions.size(); i++){
            switch (positions[i]->type) {
                case 1:
                    positions[i]->position[1].h=1000;
                break;
            }
            SDL_Rect rect0 = positions[i]->position[positions[i]->state];
            SDL_RenderCopy(renderer,miniontextures[positions[i]->type][positions[i]->state], nullptr,&rect0);
            if (positions[i]->lasting != -1&&stopTime - positions[i]->startTime >= positions[i]->lasting) {
                positions.erase(positions.begin()+i);
            }
        }
    }
}
bool Boss_attack::check_collision(int playerX, int playerY,int player_width, int player_height) {
    int x_core = playerX + player_width/2;
    int y_core = playerY + player_height/2;
    for (auto & attack : positions) {
        if(attack ->position[attack->state].y <= y_core && y_core <= attack->position[attack->state].y + attack->position[attack->state].h){
            int left = attack ->position[attack->state].x ;
            int right = attack ->position[attack->state].x + attack->position[attack->state].w;
            if (left <= x_core && x_core <= right){
                return true;
            }
        }
    }
    return false;
}
