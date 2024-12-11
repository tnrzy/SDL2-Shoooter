//
// Created by Chenyu Zhang on 2024/12/4.
//

#ifndef INC_3002_2_ENEMY_CONTROL_H
#define INC_3002_2_ENEMY_CONTROL_H
#include "common.h"
#include "enemy.h"
#include "boss.h"
class enemy_controller{
public:
    int state;
    int counter;
    bool generated;
    enemy_controller();
    ~enemy_controller();
    void updatestage(Enemy *minions , Boss *boss);


    void minion_elimination(Enemy::enemy_info *enemyinfo);

    void renderenemies(Enemy *minions, Boss *boss, SDL_Renderer *renderer, int wide,int height);
};
#endif //INC_3002_2_ENEMY_CONTROL_H
