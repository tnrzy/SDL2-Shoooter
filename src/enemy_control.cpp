//
// Created by Chenyu Zhang on 2024/12/4.
//
#include "../include/common.h"
#include"../include/enemy_control.h"
enemy_controller::enemy_controller() {
    state = 0;
    counter = 0;

    generated=false;

}
enemy_controller::~enemy_controller()= default;
void enemy_controller::renderenemies(Enemy *minions,Boss *boss,SDL_Renderer *renderer, int wide,int height,int playerX,int playerY) {
    if (state == 0){
        minions->render(renderer,wide,true);   //正常状态，只渲染普通敌人
    }
    if (state == 1){
        minions->render(renderer,wide,false);  // 过渡状态，不再生成新的敌人
    }
    if (state == 2){
         boss->render(renderer,wide,height,playerX,playerY);//渲染boss，不渲染普通敌人
    }
}
void enemy_controller::minion_elimination(Enemy::enemy_info *enemyinfo){
    counter +=enemyinfo->type;
}
void enemy_controller::updatestage(Enemy* minions,Boss* boss) { //阶段检测与切换
    if (state == 0 && counter >= 10){
        state = 1; //在一阶段中击杀积分大于五十 转入过渡阶段
    }
    if (state == 1){
        if(minions->positions.empty()) {
            //在过渡阶段中，小怪全消失后进入三阶段
            state = 2;
        }
    }
    if (state == 2){
        if(boss->positions.empty()&&generated){ //boss死亡转回一阶段
            state = 0;
            counter = 0;
            minions->enemy_buff +=1;
            generated=false;
        }
        else generated=true;
    }
    // boss死后转回一阶段；
}


