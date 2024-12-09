//
// Created by Chenyu Zhang on 2024/12/4.
//
#include "../include/boss.h"
#include "../include/common.h"
#include "../include/background.h"

#include "../include/background.h"

#include "../include/common.h"

#define FRAMERATE 60
Boss::Boss(SDL_Renderer *renderer) : minionSurface(nullptr), minionTexture(nullptr) {
    myLog = fopen("Boss.log", "w");
    if (!myLog) {
        exit(-1);
    }
    positions.reserve(type_num+2);
    widths.reserve(type_num+2);
    heights.reserve(type_num+2);
    for (int i = 0; i < type_num; i++){
        std::string path = "res/png/boss_plane" + std::to_string(i) + ".png";
        SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(),"rb"));
        if (!surf) {
            fprintf(myLog, "SDL_Boss_Surface Error: %s\n", IMG_GetError());
            exit(-5);
        }
        widths.push_back(surf->w);
        heights.push_back(surf->h);

        miniontextures.push_back(SDL_CreateTextureFromSurface(renderer, surf));
        SDL_FreeSurface(surf);
        surf= nullptr;
    }
}

Boss::~Boss() {
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

void Boss::render(SDL_Renderer *renderer, int wide) { //需要获取窗口的宽度；需要随机刷新敌机
    uint32_t stopTime = SDL_GetTicks(); //stopTime随着call该render函数，每次都在更新
    static std::uniform_int_distribution<int> distribution(0, wide);
    static std::uniform_int_distribution<int> type_distribution(0,type_num-1);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    if (t&&positions.empty()) { //每个ms刷新一次敌机；渲染的时间可能大于ms，所以用大于号
        generator.seed((unsigned)time(nullptr) + generator()); //对随机数种子更新
        int miniontype = type_distribution(generator);
        int minionPositionX = wide/2-widths[miniontype]/2; //随机产生敌机的坐标
        SDL_Rect minionRect = {minionPositionX, 200, widths[miniontype], heights[miniontype]};
        SDL_Rect health_bar = {4,174,wide-8,22};
        SDL_Rect health_bar_back = {0,170,wide,30};
        boss_info *new_minion = new boss_info(miniontype,minionRect,health_bar,health_bar_back);
        positions.reserve(100);
        positions.push_back(new_minion);
        t=false;

        startTime = stopTime; //刷新一次敌机后，startTime会被更新一次
    }
    if (!positions.empty()){
        for(int i = 0; i < positions.size(); i++){
            // positions[i]->position.y += positions[i] ->dy;
            // positions[i]->health_bar.y += positions[i] -> dy;
            SDL_Rect rect0 = positions[i]->position;
            SDL_RenderCopy(renderer,miniontextures[positions[i]->type], nullptr,&rect0);

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 191);
            SDL_Rect rect1 = positions[i] -> health_bar_back;
            SDL_RenderFillRect(renderer,&rect1);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 191);
            SDL_Rect rect2 = positions[i] -> health_bar;
            SDL_RenderFillRect(renderer,&rect2);

        }
    }
}
bool Boss::check_collision(int playerX, int playerY,int player_width, int player_height) {
    int x_core = playerX + player_width/2;
    int y_core = playerY + player_height/2;
    for (auto & boss : positions) {
        if(boss ->position.y-5 <= y_core && y_core <= boss->position.y + heights[boss->type] + 5){
            int left = boss ->position.x ;
            int right = boss ->position.x + widths[boss->type];
            if (left <= x_core && x_core <= right){
                return true;
            }
        }
    }
    return false;
}
void Boss::start() {
    startTime = SDL_GetTicks(); //返回自初始化经过的毫秒数
}
