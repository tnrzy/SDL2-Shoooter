//
// Created by Chenyu Zhang on 2024/12/4.
//
#include "../include/boss.h"

#include <thread>

#include "../include/common.h"
#include "../include/background.h"


#include "../include/common.h"

#define FRAMERATE 60
Boss::Boss(SDL_Renderer *renderer) : minionSurface(nullptr), minionTexture(nullptr) {
    myLog = fopen("Boss.log", "w");
    if (!myLog) {
        exit(-1);
    }
    positions.reserve(type_num);
    for (int i = 0; i < type_num; i++){
        widths[i].reserve(20);
        heights[i].reserve(20);
        for (int j = 0; j < max_pic[i]; j++) {
            std::string path = "res/png/boss/"+ std::to_string(i)+"/" + std::to_string(j) + ".png";
            SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(),"rb"));
            if (!surf) {
                fprintf(myLog, "SDL_Boss_Surface Error: %s\n", IMG_GetError());
                exit(-5);
            }
            widths[i].push_back(surf->w);
            heights[i].push_back(surf->h);

            miniontextures[i].push_back(SDL_CreateTextureFromSurface(renderer, surf));
            SDL_FreeSurface(surf);
            surf= nullptr;
        }
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

void Boss::render(SDL_Renderer *renderer, int wide,int height) { //需要获取窗口的宽度；需要随机刷新敌机
    uint32_t stopTime = SDL_GetTicks(); //stopTime随着call该render函数，每次都在更新
    static std::uniform_int_distribution<int> distribution(-1, 1);
    static std::uniform_int_distribution<int> type_distribution(0,type_num-1);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    if (t&&positions.empty()) { //每个ms刷新一次敌机；渲染的时间可能大于ms，所以用大于号
        generator.seed((unsigned)time(nullptr) + generator()); //对随机数种子更新
        int miniontype = type_distribution(generator);

        std::vector<SDL_Rect> minionRect;
        minionRect.reserve(20);
        for (int i=0;i<max_pic[miniontype];i++) {
            int minionPositionX = wide/2-widths[miniontype][i]/2; //敌机的初始坐标
            minionRect.push_back({minionPositionX, 200, widths[miniontype][i], heights[miniontype][i]});
        }
        SDL_Rect health_bar = {4,174,wide-8,22};
        SDL_Rect health_bar_back = {0,170,wide,30};
        boss_info *new_minion = new boss_info(miniontype,minionRect,health_bar,health_bar_back);
        positions.push_back(new_minion);
        t=false;

        startTime = stopTime; //刷新一次敌机后，startTime会被更新一次
    }
    if (!positions.empty()){
        for(int i = 0; i < positions.size(); i++){
            generator.seed((unsigned)time(nullptr) + generator());
            switch (positions[i]->type) {
                case 0:
                    if (positions[i]->stage==0) {
                        positions[i] -> health=positions[i]->max_health;
                        positions[i] -> health_bar.w=wide-8;
                        int d_x=distribution(generator),d_y=distribution(generator);
                        positions[i]->position[0].x+=d_x;
                        positions[i]->position[0].y+=d_y;
                        if (stopTime-startTime>=2000) {
                            positions[i]->stage=1;
                            positions[i]->position[0].x=wide/2-widths[0][0]/2;
                            positions[i]->position[0].y=200;
                            collision=true;
                            startTime=stopTime;
                        }
                    }
                    else if (positions[i]->stage==1) {
                        positions[i] -> health=positions[i]->max_health;
                        positions[i] -> health_bar.w=wide-8;
                        if (stopTime-startTime>=2000) {
                            positions[i]->stage=2;
                            positions[i]->state=1;
                            dx=0;
                            startTime=stopTime;
                            moveTime=stopTime;
                        }
                    }
                    else if (positions[i]->stage==2) {
                        if (stopTime-startTime>=150) {
                            positions[i] -> state++;
                            if (positions[i]->state>10) {
                                positions[i] -> state=1;
                                positions[i]->position[1]=positions[i]->position[10];
                                timer++;
                                std::uniform_int_distribution<int> times(1,static_cast<int>(positions[i]->health)/10);
                                if (timer>=times(generator)) {
                                    timer=0;
                                    positions[i]->position[11].x=positions[i]->position[1].x+widths[0][1]/2-widths[0][11]/2;
                                    positions[i]->position[11].y=125;
                                    positions[i]->stage=3;
                                    positions[i]->state=11;
                                    dx=dy=0;
                                    startTime=stopTime;
                                    moveTime=stopTime;
                                }
                            }
                            else if (positions[i]->state>1){
                                positions[i]->position[positions[i]->state]=positions[i]->position[positions[i]->state-1];
                            }
                            //if (positions[i] -> state>=positions[i] -> sum[positions[i] ->stage]) positions[i] -> state=positions[i] -> sum[positions[i] ->stage]-positions[i] -> stage_pic[positions[i] ->stage];
                            startTime=stopTime;
                        }
                        if (stopTime-moveTime>=1000) {
                            dx=distribution(generator);
                            if (positions[i]->position[positions[i]->state].x<=5) dx=abs(dx);
                            if (positions[i]->position[positions[i]->state].x+widths[0][positions[i]->state]>=wide-5) dx=-abs(dx);
                            moveTime=stopTime;
                        }
                        positions[i]->position[positions[i]->state].x+=dx;
                    }
                    else{
                        if (stopTime-moveTime>=1000) {
                            dx=distribution(generator);
                            dy=distribution(generator);
                            if (positions[i]->position[positions[i]->state].x<=5) dx=abs(dx);
                            if (positions[i]->position[positions[i]->state].x+widths[0][positions[i]->state]>=wide-5) dx=-abs(dx);
                            if (positions[i]->position[positions[i]->state].y<=205) dy=abs(dy);
                            if (positions[i]->position[positions[i]->state].y+heights[0][positions[i]->state]>=height-300) dy=-abs(dy);
                            moveTime=stopTime;
                        }
                        positions[i]->position[positions[i]->state].x+=dx;
                        positions[i]->position[positions[i]->state].y+=dy;
                        positions[i]->health+=0.09;
                        double k = (wide-12) * (positions[i]->health)/(positions[i]->max_health);
                        positions[i]->health_bar.w = static_cast<int>(std::round(k));
                        if (stopTime-startTime>=10000) {
                            positions[i]->stage=2;
                            positions[i]->state=1;
                            startTime=stopTime;
                            moveTime=stopTime;
                        }
                    }

                break;
            }
            SDL_Rect rect0 = positions[i]->position[positions[i]->state];
            SDL_RenderCopy(renderer,miniontextures[positions[i]->type][positions[i]->state], nullptr,&rect0);

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
    if (!collision) return false;
    int x_core = playerX + player_width/2;
    int y_core = playerY + player_height/2;
    for (auto & boss : positions) {
        if(boss ->position[boss->state].y <= y_core-20 && y_core+20 <= boss->position[boss->state].y + heights[boss->type][boss->state]){
            int left = boss ->position[boss->state].x ;
            int right = boss ->position[boss->state].x + widths[boss->type][boss->state];
            if (left <= x_core-20 && x_core+20 <= right){
                return true;
            }
        }
    }
    return false;
}
void Boss::start() {
    startTime = SDL_GetTicks(); //返回自初始化经过的毫秒数
    moveTime = SDL_GetTicks();
}
