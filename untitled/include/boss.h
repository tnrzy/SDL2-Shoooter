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
        boss_info(int type,std::vector<SDL_Rect> minionrect,SDL_Rect health_bar,SDL_Rect health_bar_back){
            if (type == 0){
                this->type = 0;
                this->score = 10;
                this->health = 100;
                this ->max_health = 100;
                // this->max_mode=4;
                this ->health_bar = health_bar;
                this ->health_bar_back = health_bar_back;
                this ->drop_num = 30;
                // this ->mode_pic={1,0,10,1};
                // sum.push_back(1);
                this->max_pic=12;
                for (int i=0;i<max_pic;i++) this->position.push_back(minionrect[i]);
            }
            // for (int i = 1; i < max_mode; i++) sum.push_back(sum[i-1]+mode_pic[i]);
        }

        std::vector<SDL_Rect> position;
        int state=0;
        int mode=0;
        // int max_mode=0;
        int type;
        // std::vector<int> mode_pic;
        // std::vector<int> sum;
        int max_pic;
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

    void render(SDL_Renderer *renderer, int width,int height); //用于敌人的渲染

    void start(); //用于开始计时
    static const int type_num = 1;
    const int max_pic[type_num]={12};
    std::vector<boss_info*> positions;
    std::vector<SDL_Rect> minionPosition; //要让自机和子弹知道敌机的位置信息
    std::vector<int> widths[type_num];
    std::vector<int> heights[type_num];

    void skill(int type);
    bool check_collision(int playerX, int playerY, int player_width, int player_height);

protected: //可以设计子类访问
    SDL_Surface *minionSurface;
    SDL_Texture *minionTexture;
    std::vector<SDL_Texture *> miniontextures[type_num];

    FILE *myLog;

    std::default_random_engine generator;

    uint32_t startTime; //因为Get_ticks函数用的是uint32类型
    uint32_t moveTime;

    int timer=0;
    int dx=0;
    int dy=0;
    int skill_type=-1;
    bool collision=false;
};
#endif //INC_3002_2_BOSS_H
