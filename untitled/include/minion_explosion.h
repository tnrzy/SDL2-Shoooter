//
// Created by Chenyu Zhang on 2024/12/6.
//

#ifndef INC_3002_2_MINION_EXPLOSION_H
#define INC_3002_2_MINION_EXPLOSION_H
#include "../include/common.h"
class minion_explosion{
public:
    class explosion_info{
    public:
        int x;
        int y;
        int timer;
        int type;
        SDL_Rect rect;
        int pivot;
        bool done;
        explosion_info(int x, int y, int type){
            this -> x = x;
            this -> y = y;
            this -> timer = 0;
            int pivot = 0;
            this -> type = type;
            this -> done = false;
            if (type == 0 ){
                auto exp = SDL_Rect(x,y,162,162);
                this->rect = exp;
            }
            if (type == 1){
                auto exp = SDL_Rect(x,y,60,60);
                this->rect = exp;
            }
            if (type == 2){
                auto exp = SDL_Rect(x,y,280,280);
                this->rect = exp;
            }


        }
    };
    minion_explosion(SDL_Renderer *renderer);
    void add_explosion(SDL_Rect, int type);
    void render_explosion(SDL_Renderer *renderer);
protected:
    std::vector<explosion_info*> positions;
    std::vector<SDL_Texture *> explosion0;
    std::vector<SDL_Texture *> explosion1;
    std::vector<SDL_Texture *> explosion2;
    FILE *myLog;
    bool done;

    void doexp(explosion_info *info, int type, SDL_Renderer *renderer);
};
#endif //INC_3002_2_MINION_EXPLOSION_H
