//
// Created by Chenyu Zhang on 2024/12/4.
//
#ifndef INC_3002_2_BOSS_H
#define INC_3002_2_BOSS_H

#include "common.h"
#include <random>
#include <vector>

class Boss{
public:
    class boss_info {

    public:
        boss_info(int type,SDL_Rect minionrect,SDL_Rect health_bar,SDL_Rect health_bar_back){
            if (type == 0){
                this->position = minionrect;
                this->type = 0;
                this->score = 10;
                this->health = 100;
                this ->max_health = 100;
                this ->health_bar = health_bar;
                this ->health_bar_back = health_bar_back;
                this ->drop_num = 30;


            }
            if (type == 1){
                this->position = minionrect;
                this->type = 1;
                this->dy = 3;
                this->health = 1;
                this->max_health = 1;
                this -> health_bar = health_bar;
                this ->drop_num = 1;
            }

        }

        SDL_Rect position;
        int dy;
        int type;
        int score;
        double health;
        int max_health;
        SDL_Rect health_bar;
        SDL_Rect health_bar_back;
        int drop_num;
    };
    bool t=true;
    Boss(SDL_Renderer *renderer);

    ~Boss();

    void render(SDL_Renderer *renderer, int width); //用于敌人的渲染

    void start(); //用于开始计时
    std::vector<boss_info*> positions;
    std::vector<SDL_Rect> minionPosition; //要让自机和子弹知道敌机的位置信息
    std::vector<int> widths;
    std::vector<int> heights;

    bool check_collision(int playerX, int playerY, int player_width, int player_height);

protected: //可以设计子类访问
    int type_num = 1;
    SDL_Surface *minionSurface;
    SDL_Texture *minionTexture;
    std::vector<SDL_Texture *> miniontextures;

    FILE *myLog;

    std::default_random_engine generator;

    uint32_t startTime; //因为Get_ticks函数用的是uint32类型

};
#endif //INC_3002_2_BOSS_H
