//
// Created by Chenyu Zhang on 2024/12/6.
//
#include "../include/minion_explosion.h"
#include "../include/common.h"
minion_explosion::minion_explosion(SDL_Renderer *renderer){
    myLog = myLog = fopen("explosion.log", "w");
    if (!myLog) {
        exit(-1);
    }
    explosion0.reserve(7);
    positions.reserve(100);
    for (int i = 0; i < 7; i++){
        std::string path = "res/png/explosion/explosion0/exp0" + std::to_string(i) + ".png";
        SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(),"rb"));
        if (!surf) {
            fprintf(myLog, "SDL_explosion_Surface Error: %s\n", IMG_GetError());
            exit(-5);
        }
        explosion0.push_back(SDL_CreateTextureFromSurface(renderer, surf));
        SDL_FreeSurface(surf);
        surf= nullptr;
    }
}



void minion_explosion::add_explosion(SDL_Rect rect, int type){
    explosion_info *new_explosion = new explosion_info(rect.x-40, rect.y-40, type);
    positions.push_back(new_explosion);
}

void minion_explosion::doexp(explosion_info* info, int type,SDL_Renderer* renderer){
    if (type == 0){
        SDL_RenderCopy(renderer, explosion0[info->pivot], nullptr, &info->rect);
        info->timer = info->timer+1;
        if (info->timer % 7 == 0 ){
            info->pivot = info->pivot+1;
        }
        if (info->timer > 46){
            info->done = true;
        }
    }
}

void minion_explosion::render_explosion(SDL_Renderer *renderer) {
    if (!positions.empty()) {
        for (auto itExp = positions.begin(); itExp != positions.end();) {
            if ((*itExp)->done){
                positions.erase(itExp);
            }
            doexp((*itExp),(*itExp)->type, renderer);
            if (itExp != positions.end()) {
                ++itExp;
            }

        }
    }
}

