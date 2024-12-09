#ifndef ENEMY_H
#define ENEMY_H

#include "common.h"
#include <random>
#include <vector>


class Enemy {
public:
    class enemy_info {

    public:
        enemy_info(int type,SDL_Rect minionrect,SDL_Rect health_bar){
            if (type == 0){
                this->position = minionrect;
                this->type = 0;
                this->dy = 2;
                this->health = 2;
                this ->max_health = 2;
                this ->health_bar = health_bar;
                this ->drop_num = 2;


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
            if (type == 2){
                this->position = minionrect;
                this->type = 2;
                this->dy = 1;
                this->health = 4;
                this->max_health = 4;
                this -> health_bar = health_bar;
                this ->drop_num = 3;
            }

        }

        SDL_Rect position;
        int dy;
        int type;
        double health;
        int max_health;
        SDL_Rect health_bar;
        int drop_num;
    };

    Enemy(SDL_Renderer *renderer);

    ~Enemy();

    void render(SDL_Renderer *renderer, int width,bool generating);//用于敌人的渲染

    void start(); //用于开始计时
    std::vector<enemy_info*> positions;
    std::vector<SDL_Rect> minionPosition; //要让自机和子弹知道敌机的位置信息
    std::vector<int> widths;
    std::vector<int> heights;


    bool check_collision(int playerX, int playerY, int player_width, int player_height);

protected: //可以设计子类访问
    int type_num = 3;
    SDL_Surface *minionSurface;
    SDL_Texture *minionTexture;
    std::vector<SDL_Texture *> miniontextures;

    FILE *myLog;

    std::default_random_engine generator;

    uint32_t startTime; //因为Get_ticks函数用的是uint32类型

};



#endif //ENEMY_H
