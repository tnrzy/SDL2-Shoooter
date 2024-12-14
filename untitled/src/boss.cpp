//
// Created by Chenyu Zhang on 2024/12/4.
//
#include "../include/boss.h"

#include <thread>

#include "../include/common.h"
#include "../include/background.h"


#include "../include/common.h"

#define FRAMERATE 60
Boss::Boss(SDL_Renderer *renderer,Boss_attack *boss_attack) : minionSurface(nullptr), minionTexture(nullptr) {
    myLog = fopen("Boss.log", "w");
    if (!myLog) {
        exit(-1);
    }
    this->boss_attack=boss_attack;
    positions.reserve(type_num);
    for (int i = 0; i < type_num; i++){
        widths[i].reserve(20);
        heights[i].reserve(20);
        for (int j = 0; ; j++) {
            std::string path = "res/png/boss/"+ std::to_string(i)+"/" + std::to_string(j) + ".png";
            SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(),"rb"));
            if (!surf) {
                fprintf(myLog, "SDL_Boss_Surface Error: %s\n", IMG_GetError());
                max_pic[i]=j;
                break;
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

void Boss::render(SDL_Renderer *renderer, int wide,int height,int playerX,int playerY) { //需要获取窗口的宽度；需要随机刷新敌机
    uint32_t stopTime = SDL_GetTicks(); //stopTime随着call该render函数，每次都在更新
    static std::uniform_int_distribution<int> distribution(-1, 1);
    static std::uniform_int_distribution<int> type_distribution(0,type_num-1);
    if (t&&positions.empty()) { //每个ms刷新一次敌机；渲染的时间可能大于ms，所以用大于号
        generator.seed((unsigned)time(nullptr) + generator()); //对随机数种子更新
        //int miniontype = type_distribution(generator);
        int miniontype = num;
        num=(num+1)%2;

        std::vector<SDL_Rect> minionRect;
        minionRect.reserve(20);
        for (int i=0;i<max_pic[miniontype];i++) {
            int minionPositionX = wide/2-widths[miniontype][i]/2; //敌机的初始坐标
            minionRect.push_back({minionPositionX, 80, widths[miniontype][i], heights[miniontype][i]});
        }
        SDL_Rect health_bar = {4,54,wide-8,22};
        SDL_Rect health_bar_back = {0,50,wide,30};
        boss_info *new_minion = new boss_info(miniontype,minionRect,health_bar,health_bar_back);
        positions.push_back(new_minion);
        t=false;

        startTime = stopTime; //刷新一次敌机后，startTime会被更新一次
    }
    if (!positions.empty()){
        for(auto & position : positions){
            generator.seed((unsigned)time(nullptr) + generator());
            if (position->type==0) {
                if (position->mode==0) {
                    position -> health=position->max_health;
                    position -> health_bar.w=wide-8;
                    int d_x=distribution(generator),d_y=distribution(generator);
                    position->position[0].x+=d_x;
                    position->position[0].y+=d_y;
                    if (stopTime-startTime>=2000) {
                        position->mode=1;
                        position->position[0].x=wide/2-widths[0][0]/2;
                        position->position[0].y=80;
                        collision=true;
                        startTime=stopTime;
                    }
                }
                else if (position->mode==1) {
                    position -> health=position->max_health;
                    position -> health_bar.w=wide-8;
                    if (stopTime-startTime>=2000) {
                        position->mode=2;
                        position->state=1;
                        dx=0;
                        startTime=stopTime;
                        moveTime=stopTime;
                    }
                }
                else if (position->mode==2) {
                    if (stopTime-startTime>=150) {
                        position -> state++;
                        if (position->state>10) {
                            position -> state=1;
                            position->position[1]=position->position[10];
                            timer++;
                            std::uniform_int_distribution<int> times(1,static_cast<int>(position->health));
                            if (timer>=times(generator)) {
                                timer=0;
                                position->position[11].x=position->position[1].x+widths[0][1]/2-widths[0][11]/2;
                                position->position[11].y=5;
                                position->mode=3;
                                position->state=11;
                                dx=dy=0;
                                startTime=stopTime;
                                moveTime=stopTime;
                            }
                        }
                        else if (position->state>1){
                            position->position[position->state]=position->position[position->state-1];
                            if (position->state==2) {
                                std::uniform_int_distribution<int> times(1,100);
                                if (times(generator)>75) {
                                    position->mode=4;
                                    moveTime=stopTime;
                                    dx=0;
                                }
                            }
                        }
                        //if (positions[i] -> state>=positions[i] -> sum[positions[i] ->mode]) positions[i] -> state=positions[i] -> sum[positions[i] ->mode]-positions[i] -> mode_pic[positions[i] ->mode];
                        startTime=stopTime;
                    }
                    if (stopTime-moveTime>=1000) {
                        dx=distribution(generator);
                        if (position->position[position->state].x<=5) dx=abs(dx);
                        if (position->position[position->state].x+widths[0][position->state]>=wide-5) dx=-abs(dx);
                        moveTime=stopTime;
                    }
                        position->position[position->state].x+=dx;
                }
                else if (position->mode==3) {
                    if (stopTime-moveTime>=1000) {
                        dx=distribution(generator);
                        dy=distribution(generator);
                        if (position->position[position->state].x<=5) dx=abs(dx);
                        if (position->position[position->state].x+widths[0][position->state]>=wide-5) dx=-abs(dx);
                        if (position->position[position->state].y<=205) dy=abs(dy);
                        if (position->position[position->state].y+heights[0][position->state]>=height-300) dy=-abs(dy);
                        moveTime=stopTime;
                    }

                    position->position[position->state].x+=dx;
                    position->position[position->state].y+=dy;
                    if (position->health<position->max_health) {
                        if (skill_type==0) {
                            position->health+=0.001;
                        }
                        else position->health+=0.09;
                        double k = (wide-12) * (position->health)/(position->max_health);
                        position->health_bar.w = static_cast<int>(std::round(k));
                    }
                    if (stopTime-startTime>=7000) {
                        position->mode=2;
                        position->state=1;
                        startTime=stopTime;
                        moveTime=stopTime;
                    }
                }
                else{
                    if (stopTime-startTime>=1500&&startTime!=-1) {
                        boss_attack->add_attack(1,position->position[position->state].x+widths[position->type][position->state]/2,position->position[position->state].y+heights[position->type][position->state]/2,
                            2000);
                        startTime=-1;
                        moveTime=stopTime;
                    }
                    if (stopTime-moveTime>=2000) {
                        position->mode=2;
                        startTime=stopTime;
                        moveTime=stopTime;
                    }
                }
            }

            if (position->type==1) {
                if (position->health<=10) {
                    position->state=1;
                    position->position[1].x=wide/2-widths[position->type][1]/2;
                    int d_x=distribution(generator),d_y=distribution(generator);
                    position->position[1].x+=d_x;
                    position->position[1].y+=d_y;
                    if (stopTime-startTime<=3000) {
                        position->health=10;
                    }
                    else {
                        position->health=0;
                    }
                    double k = (wide-12) * (position->health)/(position->max_health);
                    position->health_bar.w = static_cast<int>(std::round(k));
                }
                else if (position->mode==0) {
                    position -> health=position->max_health;
                    position -> health_bar.w=wide-8;
                    if (stopTime-startTime>=2000) {
                        position->mode=1;
                        collision=true;
                        startTime=stopTime;
                        moveTime=stopTime;
                    }
                }
                else if (position->mode==1) {
                    if (stopTime-startTime>=150) {
                        position -> state++;
                        if (position->state>9) {
                            position -> state=0;
                            position->position[0]=position->position[9];
                            std::uniform_int_distribution<int> times(1,100);
                            if (times(generator)<=50) {
                                position->mode=2;
                            }
                        }
                        else if (position->state>0){
                            position->position[position->state]=position->position[position->state-1];
                            if (position->state==3) {
                                std::uniform_int_distribution<int> times(1,100);
                                if (times(generator)>50) {
                                    position->mode=4;
                                    moveTime=stopTime;
                                }
                            }
                        }
                        startTime=stopTime;
                    }
                    if (stopTime-moveTime>=1000) {
                        dx=distribution(generator);
                        if (position->position[position->state].x<=5) dx=abs(dx);
                        if (position->position[position->state].x+widths[0][position->state]>=wide-5) dx=-abs(dx);
                        moveTime=stopTime;
                    }
                    position->position[position->state].x+=dx;
                }
                else if (position->mode==2) {
                    if (stopTime-startTime>=3000) {
                        std::uniform_int_distribution<int> times(1,100);
                        if (times(generator)<=50) {
                            position->mode=3;
                        }
                        else position->mode=1;
                    }
                }
                else if (position->mode==3) {
                    boss_attack->add_attack(2,position->position[position->state].x,position->position[position->state].y+heights[position->type][position->state]/2,
                            -1,playerX,playerY);
                    boss_attack->add_attack(2,position->position[position->state].x+widths[position->type][position->state],position->position[position->state].y+heights[position->type][position->state]/2,
                            -1,playerX,playerY);
                    position->mode=1;
                }
                else if (position->mode==4) {
                    if (stopTime-startTime>=7000) {
                        startTime=stopTime;
                        moveTime=stopTime;
                        position->mode=2;
                    }
                    if (stopTime-moveTime>=2000) {
                        std::uniform_int_distribution<int> d(-90,90);
                        for (int i=position->position[position->state].x+widths[position->type][position->state]/2-270+d(generator);i<=position->position[position->state].x+widths[position->type][position->state]/2+270+d(generator);i+=90) {
                            std::uniform_int_distribution<int> times(1,100);
                            if (times(generator)>25) boss_attack->add_attack(3,i,position->position[position->state].y+heights[position->type][position->state]/2,
                            -1);
                        }
                        moveTime=stopTime;
                    }
                }
            }
            SDL_Rect rect0 = position->position[position->state];
            SDL_RenderCopy(renderer,miniontextures[position->type][position->state], nullptr,&rect0);

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 191);
            SDL_Rect rect1 = position -> health_bar_back;
            SDL_RenderFillRect(renderer,&rect1);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 191);
            SDL_Rect rect2 = position -> health_bar;
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
void Boss::skill(int type) {
    skill_type=type;
}
void Boss::start() {
    startTime = SDL_GetTicks(); //返回自初始化经过的毫秒数
    moveTime = SDL_GetTicks();
}
