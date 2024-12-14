//
// Created by Chenyu Zhang on 2024/12/6.
//
#include "../include/minion_explosion.h"
#include "../include/common.h"
minion_explosion::minion_explosion(SDL_Renderer *renderer) {
    myLog = myLog = fopen("explosion.log", "w");
    if (!myLog) {
        exit(-1);
    }
    explosion0.reserve(7);
    explosion1.reserve(10);
    explosion2.reserve(9);
    explosion100.reserve(10);
    explosion101.reserve(9);
    positions.reserve(100);
    for (int i = 0; i < 7; i++) {
        std::string path = "res/png/explosion/explosion0/exp0" + std::to_string(i) + ".png";
        SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(), "rb"));
        if (!surf) {
            fprintf(myLog, "SDL_explosion_Surface Error: %s\n", IMG_GetError());
            exit(-5);
        }
        explosion0.push_back(SDL_CreateTextureFromSurface(renderer, surf));
        SDL_FreeSurface(surf);
        surf = nullptr;
    }
    for (int i = 0; i < 10; i++) {
        std::string path = "res/png/explosion/explosion1/exp1" + std::to_string(i) + ".png";
        SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(), "rb"));
        if (!surf) {
            fprintf(myLog, "SDL_explosion_Surface Error: %s\n", IMG_GetError());
            exit(-5);
        }
        explosion1.push_back(SDL_CreateTextureFromSurface(renderer, surf));
        SDL_FreeSurface(surf);
        surf = nullptr;
    }
    for (int i = 0; i < 9; i++) {
        std::string path = "res/png/explosion/explosion2/exp2" + std::to_string(i) + ".png";
        SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(), "rb"));
        if (!surf) {
            fprintf(myLog, "SDL_explosion_Surface Error: %s\n", IMG_GetError());
            exit(-5);
        }
        explosion2.push_back(SDL_CreateTextureFromSurface(renderer, surf));
        SDL_FreeSurface(surf);
        surf = nullptr;
    }
    for (int i = 0; i < 9; i++) {
        std::string path = "res/png/explosion/explosion100/" + std::to_string(i) + ".png";
        SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(), "rb"));
        if (!surf) {
            fprintf(myLog, "SDL_explosion_Surface Error: %s\n", IMG_GetError());
            exit(-5);
        }
        explosion100.push_back(SDL_CreateTextureFromSurface(renderer, surf));
        SDL_FreeSurface(surf);
        surf = nullptr;
    }
    for (int i = 0; i < 9; i++) {
        std::string path = "res/png/explosion/explosion101/" + std::to_string(i) + ".png";
        SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(), "rb"));
        if (!surf) {
            fprintf(myLog, "SDL_explosion_Surface Error: %s\n", IMG_GetError());
            exit(-5);
        }
        explosion101.push_back(SDL_CreateTextureFromSurface(renderer, surf));
        SDL_FreeSurface(surf);
        surf = nullptr;
    }
}

minion_explosion::~minion_explosion(){
    if (myLog){
        fclose(myLog);
    }
    if (!positions.empty()){
        positions.clear();
    }
    if (!explosion0.empty()){
        for (SDL_Texture * tex: explosion0){
            SDL_DestroyTexture(tex);
        }
        explosion0.clear();
    }
    if (!explosion1.empty()){
        for (SDL_Texture * tex: explosion1){
            SDL_DestroyTexture(tex);
        }
        explosion1.clear();
    }
    if (!explosion2.empty()){
        for (SDL_Texture * tex: explosion2){
            SDL_DestroyTexture(tex);
        }
        explosion2.clear();
    }
    if (!explosion100.empty()){
        for (SDL_Texture * tex: explosion100){
            SDL_DestroyTexture(tex);
        }
        explosion100.clear();
    }
    if (!explosion101.empty()){
        for (SDL_Texture * tex: explosion101){
            SDL_DestroyTexture(tex);
        }
        explosion101.clear();
    }
}

void minion_explosion::add_explosion(SDL_Rect rect, int type){
    explosion_info *new_explosion;
    if (type == 0){
        new_explosion = new explosion_info(rect.x-40, rect.y-40, type);
        positions.push_back(new_explosion);
    }
    if (type == 1){
        new_explosion = new explosion_info(rect.x-8, rect.y+7, type);
        positions.push_back(new_explosion);
    }
    if (type == 2){
        new_explosion = new explosion_info(rect.x-85, rect.y-80, type);
        positions.push_back(new_explosion);
    }
    if (type == 100){
        new_explosion = new explosion_info(rect.x, rect.y, type);
        positions.push_back(new_explosion);
    }
    if (type == 101){
        new_explosion = new explosion_info(rect.x, rect.y, type);
        positions.push_back(new_explosion);
    }

}

void minion_explosion::doexp(explosion_info* info, int type,SDL_Renderer* renderer){
    uint32_t stopTime= SDL_GetTicks();
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
    if (type == 1){
        SDL_RenderCopy(renderer, explosion2[info->pivot], nullptr, &info->rect);
        info->timer = info->timer+1;
        if (info->timer % 4 == 0 ){
            info->pivot = info->pivot+1;
        }
        if (info->timer > 35){
            info->done = true;
        }
    }
    if (type == 2){
        SDL_RenderCopy(renderer, explosion1[info->pivot], nullptr, &info->rect);
        info->timer = info->timer+1;
        if (info->timer % 5 == 0 ){
            info->pivot = info->pivot+1;
        }
        if (info->timer > 48){
            info->done = true;
        }
    }
    if (type == 100){
        if (stopTime-info->startTime > 150 ){
            if (info->pivot <8) {
                info->pivot = info->pivot+1;
            }
            else info->done = true;
            info->startTime = stopTime;
        }
        SDL_RenderCopy(renderer, explosion100[info->pivot], nullptr, &info->rect);
    }
    if (type == 101){
        if (stopTime-info->startTime > 150 ){
            if (info->pivot <8) {
                info->pivot = info->pivot+1;
            }
            else info->done = true;
            info->startTime = stopTime;
        }
        SDL_RenderCopy(renderer, explosion101[info->pivot], nullptr, &info->rect);
    }
}

void minion_explosion::render_explosion(SDL_Renderer *renderer) {
    if (!positions.empty()) {
        for (auto itExp = positions.begin(); itExp != positions.end();) {
            if ((*itExp)->done){
                itExp=positions.erase(itExp);
                continue;
            }
            if (itExp != positions.end()) {
                doexp((*itExp),(*itExp)->type, renderer);
            }
            else {
                positions.pop_back();
            }
            if (itExp != positions.end()) {
                ++itExp;
            }
        }
    }
}

