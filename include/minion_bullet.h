//
// Created by Chenyu Zhang on 2024/12/10.
//

#ifndef INC_3002_2_MINION_BULLET_H
#define INC_3002_2_MINION_BULLET_H
#include "../include/common.h"
#include "../include/enemy.h"
class minion_bullet{
public:
    class mbullet_info{
    public:
        int dy;
        int dx;
        SDL_Rect rect;
        int damage;
        int mtype;
        mbullet_info(int dx, int dy, int type, int x, int y){
            this->dy = dy;
            this->dx = dx;
            if (type == 0){
                auto bul = SDL_Rect(x-5,y-20,15,50);
                this ->rect = bul;
                this ->mtype = 0;
            }
            if (type == 1){
                auto bul = SDL_Rect(x-25,y-20,15,50);
                this ->rect = bul;
                this ->mtype = 0;
            }
            if (type == 2){
                auto bul = SDL_Rect(x+15,y-20,15,50);
                this ->rect = bul;
                this ->mtype = 0;
            }
            if (type == 20){
                auto bul = SDL_Rect(x-10,y-10,20,20);
                this ->rect = bul;
                this ->mtype = 3;
            }
            if (type == 21){
                auto bul = SDL_Rect(x+30,y - 20,22,29);
                this ->rect = bul;
                this ->mtype = 1;
            }
            if (type == 22){
                auto bul = SDL_Rect(x+10,y+20,22,29);
                this ->rect = bul;
                this ->mtype = 1;
            }
            if (type == 23){
                auto bul = SDL_Rect(x- 30,y - 20,22,29);
                this ->rect = bul;
                this ->mtype = 2;
            }
            if (type == 24){
                auto bul = SDL_Rect(x-10,y+20,22,29);
                this ->rect = bul;
                this ->mtype = 2;
            }
        }
    };
    minion_bullet(SDL_Renderer *renderer);
    ~minion_bullet();
    void add_minion_bullet(int type,Enemy::enemy_info enemy);
    std::vector<mbullet_info*> positions;
    std::vector<SDL_Texture *> mbullet;
    FILE *myLog;

    void minion_bullet_render(SDL_Renderer *renderer, int width, int height);
};
#endif //INC_3002_2_MINION_BULLET_H
