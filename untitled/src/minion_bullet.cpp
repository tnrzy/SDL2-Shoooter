//
// Created by Chenyu Zhang on 2024/12/10.
//
#include "../include/minion_bullet.h"
#include "../include/common.h"
minion_bullet::minion_bullet(SDL_Renderer *renderer){
    myLog = myLog = fopen("mbulllets.log", "w");
    if (!myLog) {
    exit(-1);
    }
    mbullet.reserve(4);
    for (int i = 0; i < 4; i++) {
        std::string path = "res/png/enemy_bullet/minion_bullet" + std::to_string(i) + ".png";
        SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(), "rb"));
        if (!surf) {
            fprintf(myLog, "SDL_minion_bullet_Surface Error: %s\n", IMG_GetError());
            exit(-5);
        }
        mbullet.push_back(SDL_CreateTextureFromSurface(renderer, surf));
        SDL_FreeSurface(surf);
        surf = nullptr;
    }

};

void minion_bullet::add_minion_bullet(int type,Enemy::enemy_info enemy) {
    int x_core = enemy.position.x + enemy.position.w/2;
    int y_core = enemy.position.y + enemy.position.h/2;
    if (enemy.type == 0){
        if (enemy.bullet_timer == 10){
            auto *new_bullet = new mbullet_info(0,3,0,x_core, y_core);
            positions.push_back(new_bullet);
        }
        if (enemy.bullet_timer == 150){
            auto *new_bullet1 = new mbullet_info(0,3,1,x_core,y_core);
            positions.push_back(new_bullet1);
            auto *new_bullet2 = new mbullet_info(0,3,2,x_core,y_core);
            positions.push_back(new_bullet2);
        }
    }
    if (enemy.type == 2) {
        if (enemy.bullet_timer == 70) {
            auto *new_bullet0 = new mbullet_info(0, 2, 20, x_core, y_core);
            positions.push_back(new_bullet0);
            auto *new_bullet1 = new mbullet_info(1, 1, 20, x_core, y_core);
            positions.push_back(new_bullet1);
            auto *new_bullet2 = new mbullet_info(2, 0, 20, x_core, y_core);
            positions.push_back(new_bullet2);
            auto *new_bullet3 = new mbullet_info(1, -1, 20, x_core, y_core);
            positions.push_back(new_bullet3);
            auto *new_bullet4 = new mbullet_info(0, -2, 20, x_core, y_core);
            positions.push_back(new_bullet4);
            auto *new_bullet5 = new mbullet_info(-1, -1, 20, x_core, y_core);
            positions.push_back(new_bullet5);
            auto *new_bullet6 = new mbullet_info(-2, 0, 20, x_core, y_core);
            positions.push_back(new_bullet6);
            auto *new_bullet7 = new mbullet_info(-1, 1, 20, x_core, y_core);
            positions.push_back(new_bullet7);
        }
        if (enemy.bullet_timer == 170){
            auto *new_bullet0 = new mbullet_info(1, 2, 21, x_core, y_core);
            positions.push_back(new_bullet0);
            auto *new_bullet1 = new mbullet_info(1, 2, 22, x_core, y_core);
            positions.push_back(new_bullet1);
            auto *new_bullet2 = new mbullet_info(-1, 2, 23, x_core, y_core);
            positions.push_back(new_bullet2);
            auto *new_bullet3 = new mbullet_info(-1, 2, 24, x_core, y_core);
            positions.push_back(new_bullet3);
        }
    }
}
void minion_bullet::minion_bullet_render(SDL_Renderer *renderer, int width, int height) {
    if (!positions.empty()) {
        for (auto itbul = positions.begin(); itbul != positions.end();) {
            if ((*itbul)->rect.x < -30
                    || (*itbul)->rect.x > width + 30
                    || (*itbul)->rect.y < -30
                    || (*itbul)->rect.y > height + 30){
                itbul = positions.erase(itbul);
                continue;
            }
            if (itbul != positions.end()) {
                (*itbul)->rect.x +=(*itbul)->dx;
                (*itbul)->rect.y +=(*itbul)->dy;
                SDL_RenderCopy(renderer, mbullet[(*itbul)->mtype], nullptr, &(*itbul)->rect);
            }
            else {
                positions.pop_back();
            }
            if (itbul != positions.end()) {
                ++itbul;
            }
        }
    }
}

