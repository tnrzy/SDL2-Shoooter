#include "../include/enemy.h"
#include "../include/common.h"

#define FRAMERATE 60
Enemy::Enemy(SDL_Renderer *renderer) : minionSurface(nullptr), minionTexture(nullptr) {
    myLog = fopen("Enemy.log", "w");
    if (!myLog) {
        exit(-1);
    }
    positions.reserve(type_num+2);
    widths.reserve(type_num+2);
    heights.reserve(type_num+2);
    for (int i = 0; i < type_num; i++){
        std::string path = "res/png/enemy_plane" + std::to_string(i) + ".png";
        SDL_Surface *surf = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(),"rb"));
        if (!surf) {
            fprintf(myLog, "SDL_Enemy_Surface Error: %s\n", IMG_GetError());
            exit(-5);
        }
        widths.push_back(surf->w);
        heights.push_back(surf->h);

        miniontextures.push_back(SDL_CreateTextureFromSurface(renderer, surf));
        SDL_FreeSurface(surf);
        surf= nullptr;
    }
    generate_interval = 2500;
    generate_timer = 0;
    enemy_buff = 0;
}

Enemy::~Enemy() {
    if (myLog) {
        fclose(myLog);
    }

    if (minionSurface) {
        SDL_FreeSurface(minionSurface);
    }

    if (!miniontextures.empty()) {
        for (SDL_Texture *tex : miniontextures){
            SDL_DestroyTexture(tex);
        }
        miniontextures.clear();
    }
    if (!positions.empty()){
        positions.clear();
    }
    if (!widths.empty()){
        widths.clear();
    }
    if (!heights.empty()){}
    heights.clear();
}

void Enemy::render(SDL_Renderer *renderer, int wide,bool generating) {  //需要获取窗口的宽度；需要随机刷新敌机
    if (generate_timer <= 4000){
        generate_timer++;
        if (generate_timer == 1000){
            generate_interval = 1500;
        }
        if (generate_timer == 2000){
            generate_interval = 800;
        }
        if (generate_timer == 3000){
            enemy_buff += 1;
        }
        if (generate_timer == 4000){
            enemy_buff += 1;
        }
    }

    uint32_t stopTime = SDL_GetTicks(); //stopTime随着call该render函数，每次都在更新
    static std::uniform_int_distribution<int> distribution(0, wide);
    static std::uniform_int_distribution<int> type_distribution(0,type_num-1);
    SDL_SetRenderDrawColor(renderer,255,0,0,255);
    if (stopTime - startTime >= generate_interval && generating) {  //每个ms刷新一次敌机；渲染的时间可能大于ms，所以用大于号
        generator.seed((unsigned)time(nullptr) + generator()); //对随机数种子更新

        int minionPositionX = 0.8 * distribution(generator); //随机产生敌机的坐标
        int miniontype = type_distribution(generator);
        SDL_Rect minionRect = {minionPositionX, 0, widths[miniontype], heights[miniontype]};
        SDL_Rect health_bar = {minionPositionX,-10,widths[miniontype],5};
        enemy_info *new_minion = new enemy_info(miniontype,minionRect,health_bar,enemy_buff);
        positions.reserve(100);
        positions.push_back(new_minion);


        startTime = stopTime; //刷新一次敌机后，startTime会被更新一次
    }
    if (!positions.empty()){
        for(int i = 0; i < positions.size(); i++){
            positions[i]->position.y += positions[i] ->dy;
            positions[i]->health_bar.y += positions[i] -> dy;
            SDL_Rect rect0 = positions[i]->position;
            SDL_RenderCopy(renderer,miniontextures[positions[i]->type], nullptr,&rect0);
            SDL_Rect rect1 = positions[i] -> health_bar;
            SDL_RenderFillRect(renderer,&rect1);
        }
    }
}
bool Enemy::check_collision(int playerX, int playerY,int player_width, int player_height) {
    int x_core = playerX + player_width/2;
    int y_core = playerY + player_height/2;
    for (auto & enemy : positions) {
        if(enemy ->position.y-5 <= y_core && y_core <= enemy->position.y + heights[enemy->type] + 5){
            int left = enemy ->position.x ;
            int right = enemy ->position.x + widths[enemy->type];
            if (left <= x_core && x_core <= right){
                return true;
            }
        }
    }
    return false;
}
void Enemy::updateattack(){
    for (auto & enemy : positions) {
        enemy->bullet_timer +=1;
        if (enemy->type == 0) {
            if (enemy->bullet_timer >= 300) {
                enemy->bullet_timer = 0;
            }
        }
        if (enemy ->type == 2){
            if (enemy->bullet_timer >= 400) {
                enemy->bullet_timer = 0;
            }
        }
    }
}
void Enemy::start() {
    startTime = SDL_GetTicks(); //返回自初始化经过的毫秒数
}
