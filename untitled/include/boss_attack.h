//
// Created by Moyuan on 24-12-13.
//

#ifndef BOSS_ATTACK_H
#define BOSS_ATTACK_H
#include "common.h"
#include <random>
#include <vector>

class Boss_attack{
public:
    class attack_info {

    public:
        attack_info(int type,std::vector<SDL_Rect> attackrect,int lasting){
            this->type=type;
            this->startTime=SDL_GetTicks();
            if (type == 1){
                this->max_state=1;
                this->lasting=lasting;
                this->position.push_back(SDL_Rect(0,0,0,0));
            }
            for (int i=1;i<=max_state;i++) this->position.push_back(attackrect[i]);
        }

        std::vector<SDL_Rect> position;
        int state=1;
        int type;
        int x;
        int y;
        int max_state=0;
        int lasting=-1;
        uint32_t startTime;
    };

    Boss_attack(SDL_Renderer *renderer);

    ~Boss_attack();

    void add_attack(int type,int x,int y,int lasting);
    void render(SDL_Renderer *renderer); //用于敌人的渲染

    static const int type_num = 1;
    const int max_pic[type_num]={12};
    std::vector<attack_info*> positions;
    std::vector<int> widths[type_num+5];
    std::vector<int> heights[type_num+5];

    bool check_collision(int playerX, int playerY, int player_width, int player_height);

protected: //可以设计子类访问
    SDL_Surface *minionSurface;
    SDL_Texture *minionTexture;
    std::vector<SDL_Texture *> miniontextures[type_num+5];

    FILE *myLog;

    std::default_random_engine generator;

};

#endif //BOSS_ATTACK_H
