#include <stdio.h>
#include "../include/common.h"
#include <cstdio>


#include "../include/background.h"
#include "../include/enemy.h"
#include "../include/boss.h"
#include "../include/Player.h"
#include "../include/enemy_control.h"
#include "../include/minion_explosion.h"
#include <string>
#include <iostream>
using namespace std;

#ifdef __WIN32
#define Main main

#endif

#define FRAMERATE 60 //帧率控制为60


Background::Background() : myLog(nullptr), state(START), window(nullptr), imageAccess(nullptr),
renderer(nullptr), backgroundSurface(nullptr), backgroundTexture(nullptr) { //构造函数;立即初始化右边变量
    window = nullptr;
    renderer = nullptr;

    myLog = fopen("Background.log", "w");
    if (myLog == nullptr) {
        exit(-1); // 文件打开失败为-1
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG); //png与jpg皆可

    magic_point=1;
    beated_enemy=0;
    score=0;
    skill_state=0;
    dropnumber=0;
}


Background::~Background() { //销毁函数
    if (myLog) {
        fclose(myLog);
    }

    if (backgroundSurface) {
        SDL_FreeSurface(backgroundSurface);
    }

    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }

    if (minion) {
        delete minion;
    }

    if (boss) {
        delete boss;
    }

    if (player) {
        delete player;
    }

    if (bullet) {
        delete bullet;
    }

    IMG_Quit();
    SDL_Quit();
}


void Background::loadJPG(char *file) {
    if (imageAccess != file) {
        imageAccess = file;
    } else {
        return;
    }

    backgroundSurface = IMG_LoadJPG_RW(SDL_RWFromFile(file, "rb"));
    if (!backgroundSurface) {
        fprintf(myLog, "Error loading background surface: %s\n", IMG_GetError());
        exit(-5);
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    backgroundSurface = nullptr;
}

void Background::loadPNG(char *file) {
    if (imageAccess != file) {
        imageAccess = file;
    } else {
        return;
    }

    backgroundSurface = IMG_LoadPNG_RW(SDL_RWFromFile(file, "rb"));
    if (!backgroundSurface) {
        fprintf(myLog, "Error loading background surface: %s\n", IMG_GetError());
        exit(-5);
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    backgroundSurface = nullptr;
}


void Background::init() { //用于初始化的函数
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { //小于零表示初始化失败；同时也初始化了event
        fprintf(myLog, "SDL_Init Error: %s\n", SDL_GetError()); //没有初始化成功就打印到myLog
        exit(-2);
    }

    TTF_Init();
    backgroundSurface = IMG_LoadJPG_RW(SDL_RWFromFile("res/png/start.jpg", "rb"));
    minion_width = backgroundSurface->w; //选择产生的宽度为背景图片的1倍
    window_height = backgroundSurface->h;
    playerX = backgroundSurface->w / 2;
    playerY = backgroundSurface->h / 2;

    window = SDL_CreateWindow("STG Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        backgroundSurface->w, backgroundSurface->h, SDL_WINDOW_SHOWN); //这样获取的窗口就直接是图片大小

    SDL_FreeSurface(backgroundSurface);

    if (!window) {
        fprintf(myLog, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        exit(-3);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(myLog, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        exit(-4);
    }

    this->loadPNG("res/png/start.jpg"); //在成员函数内部，使用 this 指针调用类的其他成员函数
}


void Background::gameplay() { //游戏进程函数
    bullet = new Bullet(renderer);
    player = new Player(renderer);
    minion = new Enemy(renderer);
    boss = new Boss(renderer);
    drop = new drop_items(renderer);
    controller = new enemy_controller();
    explosions = new minion_explosion(renderer);

    SDL_Event event; //用于接受传入的各类事件
    uint32_t begin, end, elapsed, rate;
    uint32_t delay; //用于控制帧率
    bool isTurbo = false;
    bool isSpacePressed = false;


    while (true) {
        begin = SDL_GetTicks();
        render(); //绘制图像
        int speed = 5;

        while (SDL_PollEvent(&event)) { //因为要修改event所以pass by ref; 用while是为了在一轮内把事件都处理完
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (state != PLAYING) {
                        state = PLAYING;
                    }
                    if (event.button.y > 0 && event.button.y < window_height/3) {
                        player -> playerType = 0;
                    } else if (event.button.y > window_height/3 && event.button.y < 2*window_height/3) {
                        player -> playerType = 1;
                    } else {
                        player -> playerType = 2;
                    }
                }
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (state != PLAYING) {
                            state = PLAYING;
                        } break;


                    case SDLK_LSHIFT:
                        isTurbo = true; // 当键被按下时，开启加速模式
                        player -> turbo = true;
                        break;



                    case SDLK_ESCAPE:
                        exit(0);
                    break;

                    default:
                        break;
                }
            }

            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_LSHIFT) {
                    isTurbo = false; // 当键被释放时，关闭加速模式
                    player -> turbo = false;
                }
            }

            if (event.type == SDL_QUIT) {
                SDL_Quit();
            }
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        if (state == PLAYING) {
            if (keystate[SDL_SCANCODE_LEFT]) {
                playerX -= isTurbo ? 2 : speed; // 使用键的状态来决定速度
            }
            if (keystate[SDL_SCANCODE_RIGHT]) {
                playerX += isTurbo ? 2 : speed;
            }
            if (keystate[SDL_SCANCODE_DOWN]) {
                playerY += isTurbo ? 2 : speed;
            }
            if (keystate[SDL_SCANCODE_UP]) {
                playerY -= isTurbo ? 2 : speed;
            }
            if (keystate[SDL_SCANCODE_LALT]) {
                uint32_t nowTime = SDL_GetTicks();
                if (magic_point >= 2 && nowTime - skill_time > 800) {
                    skill_state = 1;
                    skill_time = SDL_GetTicks();

                }
            }
            if (keystate[SDL_SCANCODE_LCTRL]) {
                isSpacePressed = true;
            } else {
                isSpacePressed = false;
            }

            if (isSpacePressed) {
                bullet -> attack = true;
            } else {
                bullet -> attack = false;
            }
        }
        SDL_Delay(10);
        /*
        end = SDL_GetTicks();
        elapsed = end - begin;
        rate = 1000 / FRAMERATE; //一轮循环的时间
        delay = rate - elapsed;
        if (delay > 0) { //使帧率稳定在设定值
            SDL_Delay(delay);
        }*/
    }
}


void Background::render() { //设定渲染器的函数
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); //设定渲染器颜色
    SDL_RenderClear(renderer); //清除渲染缓冲区

    switch (state) {
        case START:
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
            break;
        case END:
            this->loadJPG("res/png/end.jpg");
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

            delete minion;
            delete boss;
            delete bullet;
            delete player; //删除对象达到重新开始的效果,再次开始时需要重新创建
            delete drop;
            delete controller;
            delete explosions;
            bullet = new Bullet(renderer);
            player = new Player(renderer);
            minion = new Enemy(renderer);
            boss = new Boss(renderer);
            drop = new drop_items(renderer);
            explosions = new minion_explosion(renderer);
            controller = new enemy_controller();

            break;
        case PLAYING:
            this->loadJPG("res/png/background.jpg");
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

            player -> render(playerX, playerY); //利用传入的数据刷新player的渲染器；在此处渲染是为了保证每次渲染屏幕时，player都会被渲染到
            controller->updatestage(minion,boss);
            controller->renderenemies(minion,boss,renderer,minion_width);
            bullet->render(renderer, playerX, playerY, player,magic_point); //常规子弹的渲染
            drop ->render(renderer);
            this->fontrender(width);
            int skill_type = 0;
            if (skill_state == 1) {
                skill_state = 0;
                magic_point -=1;
                skillrender(skill_type);
                if (skill_type == 0) {
                    for (auto itMinion = minion->positions.begin(); itMinion != minion->positions.end();) {
                        if (itMinion != minion->positions.end()) {
                            beated_enemy += 1;
                            int sco = (*itMinion)->type;
                            if (sco == 2) {
                                score += 1;
                            } else if (sco == 1) {
                                score += 3;
                            } else if (sco == 0) {
                                score += 2;
                            }

                            explosions ->add_explosion((*itMinion)->position,(*itMinion)->type);
                            controller->minion_elimination((*itMinion));
                            itMinion = minion->positions.erase(itMinion);
                        }

                    }
                }
            }
            dropnumber=0;
            for (auto itdrop = drop->dropposition.begin(); itdrop != drop->dropposition.end();){
                if((*itdrop)->rect.y > window_height + 50){
                    itdrop=drop->dropposition.erase(itdrop);
                }
                else if ( (*itdrop)->rect.x+ (*itdrop)->rect.w>= playerX-50
                        && (*itdrop)->rect.x <= playerX + player->playerWidth+50
                        && (*itdrop)->rect.y+ (*itdrop)->rect.h >= playerY-50
                        && (*itdrop)->rect.y <= playerY + player->playerHeight+50) {
                    itdrop=drop->dropposition.erase(itdrop);
                    dropnumber+=1;
                        }
                else {
                    ++itdrop;
                }
            }
            if (player->playerType == 0) {

                for (auto itMinion = minion->positions.begin(); itMinion != minion->positions.end();) {
                    if ((*itMinion)->position.y > window_height + 50) {
                        itMinion = minion->positions.erase(itMinion);
                        continue;
                    }
                    for (auto itBullet = bullet->bulletPosition.begin(); itBullet != bullet->bulletPosition.end();) {
                        //对子弹的检测
                        if (itBullet->x + bullet->width>= (*itMinion)->position.x
                            && itBullet->x <= ((*itMinion)->position.x + minion->widths[(*itMinion)->type])
                            && itBullet->y + bullet->height >= (*itMinion)->position.y
                            && itBullet->y <= ((*itMinion)->position.y + minion->heights[(*itMinion)->type])) {
                            if (itBullet != bullet->bulletPosition.end()) {
                                //防止数组越界
                                itBullet = bullet->bulletPosition.erase(itBullet);
                            } else {
                                bullet->bulletPosition.pop_back();
                            }
                            if (itMinion != minion->positions.end()) {
                                //防止数组越界
                                (*itMinion)->health = (*itMinion)->health - 1;
                                double k = (*itMinion)->position.w * ((*itMinion)->health) / ((*itMinion)->
                                               max_health);
                                (*itMinion)->health_bar.w = static_cast<int>(std::round(k));
                                if ((*itMinion)->health == 0) {
                                    beated_enemy += 1;
                                    int sco = (*itMinion)->type;
                                    if (sco == 2) {

                                        score += 1;
                                    } else if (sco == 1) {

                                        score += 3;
                                    } else if (sco == 0) {

                                        score += 2;
                                    }
                                    drop->add_drop((*itMinion)->position, (*itMinion)->drop_num);
                                    explosions ->add_explosion((*itMinion)->position,(*itMinion)->type);
                                    controller->minion_elimination((*itMinion));
                                    itMinion = minion->positions.erase(itMinion);
                                }
                            } else {
                                beated_enemy += 1;
                                int sco = (*itMinion)->type;
                                if (sco == 2) {

                                    score += 1;
                                } else if (sco == 1) {

                                    score += 3;
                                } else if (sco == 0) {

                                    score += 2;
                                }
                                minion->positions.pop_back();
                            }
                        } else {
                            if (itBullet != bullet->bulletPosition.end()) {
                                ++itBullet;
                            }
                        }
                    }
                    if (itMinion != minion->positions.end()) {
                        ++itMinion;
                    }
                }
                for (auto bosses = boss->positions.begin(); bosses != boss->positions.end();) {
                    if ((*bosses)->position.y > window_height + 50) {
                        bosses = boss->positions.erase(bosses);
                        continue;
                    }
                    for (auto itBullet = bullet->bulletPosition.begin(); itBullet != bullet->bulletPosition.end();) {
                        //对子弹的检测
                        if (itBullet->x + bullet->width>= (*bosses)->position.x
                            && itBullet->x <= ((*bosses)->position.x + boss->widths[(*bosses)->type])
                            && itBullet->y + bullet->height >= (*bosses)->position.y
                            && itBullet->y <= ((*bosses)->position.y + boss->heights[(*bosses)->type])) {
                            if (itBullet != bullet->bulletPosition.end()) {
                                //防止数组越界
                                itBullet = bullet->bulletPosition.erase(itBullet);
                            } else {
                                bullet->bulletPosition.pop_back();
                            }
                            if (bosses != boss->positions.end()) { //防止数组越界
                                (*bosses)->health = (*bosses)->health - 0.1;
                                double k = (minion_width-12) * ((*bosses)->health)/((*bosses)->max_health);
                                (*bosses)->health_bar.w = static_cast<int>(std::round(k));
                                if ((*bosses)->health <= 0){
                                    beated_enemy+=1;

                                    score+=(*bosses)->score;
                                    drop ->add_drop((*bosses)->position,(*bosses)->drop_num);
                                    //explosions ->add_explosion((*bosses)->position,(*bosses)->type);
                                    //controller->boss_elimination((*bosses));
                                    bosses = boss->positions.erase(bosses);
                                }
                            } else {
                                beated_enemy+=1;

                                score+=(*bosses)->score;
                                boss->positions.pop_back();
                            }
                        } else {
                            if (itBullet != bullet->bulletPosition.end()) {
                                ++itBullet;
                            }
                        }
                    }
                    if (bosses != boss->positions.end()) {
                        ++bosses;
                    }
                }
            }

            if (player->playerType == 1) { //x + (player->playerWidth/2)-width/2, y - (player->playerHeight)
                if (bullet -> attack == true) {

                    for (auto itMinion = minion->positions.begin(); itMinion != minion->positions.end();) {
                        if ((*itMinion)->position.y > window_height+ 50){
                            itMinion = minion->positions.erase(itMinion);
                            continue;
                        }
                        if ((*itMinion)->position.x + minion->widths[(*itMinion)->type] >= (playerX + player->playerWidth/2) - bullet->width*(bullet->firePoint)/2 &&
                        (*itMinion)->position.x <= (playerX + player->playerWidth/2) + bullet->width*(bullet->firePoint)/2 &&
                        (*itMinion)->position.y <= playerY &&
                        (*itMinion)->position.y + minion->heights[(*itMinion)->type] >= playerY - bullet->height*(bullet->firePoint)) {
                            if (itMinion != minion->positions.end()) { //防止数组越界
                                (*itMinion)->health = (*itMinion)->health - 0.1;
                                double k = (*itMinion)->position.w * ((*itMinion)->health)/((*itMinion)->max_health);
                                (*itMinion)->health_bar.w = static_cast<int>(std::round(k));
                                if ((*itMinion)->health <= 0){
                                    beated_enemy+=1;
                                    int sco = (*itMinion)->type;
                                    if (sco == 2) {

                                        score+=1;
                                    }else if (sco == 1) {

                                        score+=3;
                                    }else if (sco == 0) {

                                        score+=2;
                                    }
                                    drop ->add_drop((*itMinion)->position,(*itMinion)->drop_num);
                                    explosions ->add_explosion((*itMinion)->position,(*itMinion)->type);
                                    controller->minion_elimination((*itMinion));
                                    itMinion = minion->positions.erase(itMinion);
                                }
                            } else {
                                beated_enemy+=1;
                                int sco = (*itMinion)->type;
                                if (sco == 2) {

                                    score+=1;
                                }else if (sco == 1) {

                                    score+=3;
                                }else if (sco == 0) {

                                    score+=2;
                                }
                                minion->positions.pop_back();
                            }
                        }
                        if (itMinion != minion->positions.end()) {
                            ++itMinion;
                        }
                    }
                    for (auto bosses = boss->positions.begin(); bosses != boss->positions.end();) {
                        if ((*bosses)->position.y > window_height+ 50){
                            bosses = boss->positions.erase(bosses);
                            continue;
                        }
                        if ((*bosses)->position.x + boss->widths[(*bosses)->type] >= (playerX + player->playerWidth/2) - bullet->width*(bullet->firePoint)/2 &&
                        (*bosses)->position.x <= (playerX + player->playerWidth/2) + bullet->width*(bullet->firePoint)/2 &&
                        (*bosses)->position.y <= playerY &&
                        (*bosses)->position.y + boss->heights[(*bosses)->type] >= playerY - bullet->height*(bullet->firePoint)) {
                            if (bosses != boss->positions.end()) { //防止数组越界
                                (*bosses)->health = (*bosses)->health - 0.1;
                                double k = (minion_width-12) * ((*bosses)->health)/((*bosses)->max_health);
                                (*bosses)->health_bar.w = static_cast<int>(std::round(k));
                                if ((*bosses)->health <= 0){
                                    beated_enemy+=1;

                                    score+=(*bosses)->score;
                                    drop ->add_drop((*bosses)->position,(*bosses)->drop_num);
                                    //explosions ->add_explosion((*bosses)->position,(*bosses)->type);
                                    //controller->boss_elimination((*bosses));
                                    bosses = boss->positions.erase(bosses);
                                }
                            } else {
                                beated_enemy+=1;
                                score+=(*bosses)->score;
                                boss->positions.pop_back();
                            }
                        }
                        if (bosses != boss->positions.end()) {
                            ++bosses;
                        }
                    }
                }
            }
            if (player->playerType == 2) { //x + (player->playerWidth/2)-width/2, y - (player->playerHeight)
                if (bullet -> attack == true) {

                    for (auto itMinion = minion->positions.begin(); itMinion != minion->positions.end();) {
                        if ((*itMinion)->position.y > window_height+ 50){
                            itMinion = minion->positions.erase(itMinion);
                            continue;
                        }
                        if ((*itMinion)->position.x + minion->widths[(*itMinion)->type] >= (playerX + player->playerWidth/2) - bullet->width/2 &&
                        (*itMinion)->position.x <= (playerX + player->playerWidth/2) + bullet->width/2 ) {
                            if (itMinion != minion->positions.end()) { //防止数组越界
                                (*itMinion)->health = (*itMinion)->health - 0.1;
                                double k = (*itMinion)->position.w * ((*itMinion)->health)/((*itMinion)->max_health);
                                (*itMinion)->health_bar.w = static_cast<int>(std::round(k));
                                if ((*itMinion)->health <= 0){
                                    beated_enemy+=1;
                                    int sco = (*itMinion)->type;
                                    if (sco == 2) {

                                        score+=1;
                                    }else if (sco == 1) {

                                        score+=3;
                                    }else if (sco == 0) {

                                        score+=2;
                                    }
                                    drop ->add_drop((*itMinion)->position,(*itMinion)->drop_num);
                                    controller->minion_elimination((*itMinion));
                                    explosions ->add_explosion((*itMinion)->position,(*itMinion)->type);
                                    itMinion = minion->positions.erase(itMinion);
                                }
                            } else {
                                beated_enemy+=1;
                                int sco = (*itMinion)->type;
                                if (sco == 2) {

                                    score+=1;
                                }else if (sco == 1) {

                                    score+=3;
                                }else if (sco == 0) {

                                    score+=2;
                                }
                                minion->positions.pop_back();
                            }
                        }
                        if (itMinion != minion->positions.end()) {
                            ++itMinion;
                        }
                    }
                    for (auto bosses = boss->positions.begin(); bosses != boss->positions.end();) {

                        if ((*bosses)->position.x + boss->widths[(*bosses)->type] >= (playerX + player->playerWidth/2) - bullet->width/2 &&
                        (*bosses)->position.x <= (playerX + player->playerWidth/2) + bullet->width/2 ) {
                            if (bosses != boss->positions.end()) { //防止数组越界
                                (*bosses)->health = (*bosses)->health - 0.1;
                                double k = (minion_width-12) * ((*bosses)->health)/((*bosses)->max_health);
                                (*bosses)->health_bar.w = static_cast<int>(std::round(k));
                                if ((*bosses)->health <= 0){
                                    beated_enemy+=1;
                                    score+=(*bosses)->score;
                                    drop ->add_drop((*bosses)->position,(*bosses)->drop_num);
                                //controller->minion_elimination((*bosses));
                                //explosions ->add_explosion((*bosses)->position,(*bosses)->type);
                                    bosses = boss->positions.erase(bosses);
                                }
                            } else {
                                beated_enemy+=1;
                                score+=(*bosses)->score;
                                boss->positions.pop_back();
                            }
                        }
                        if (bosses != boss->positions.end()) {
                            ++bosses;
                        }
                    }
                }
            }

            if (magic_point<4) {
                magic_point+=dropnumber*0.2;
            }
            if (magic_point>4) {
                magic_point=4;
            }
            explosions->render_explosion(renderer);
            if (minion->check_collision(playerX,playerY,player->playerWidth,player->playerHeight)||boss->check_collision(playerX,playerY,player->playerWidth,player->playerHeight)){
                state = END;
                playerX = minion_width / 2;
                playerY = window_height / 2;
            }
            break;
    }

    SDL_RenderPresent(renderer); //此处会present一次
}
void Background::fontrender(int width) {

    TTF_Font *ttffont = TTF_OpenFont("res/font/simkai.ttf", 20);
    if (ttffont == NULL)
    {
        printf("simkai.ttf open failed\n");
        exit(-10);
    }
    SDL_Color color = { 52,203,120,255 };
    int b=(int)magic_point;
    string str1="magicpoint:"+to_string(b);
    const char * str2 = str1.c_str();
    SDL_Surface *text5_surface = TTF_RenderUTF8_Blended(ttffont, str2, color);
    if (!text5_surface) {
        exit(-15);
    }
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, text5_surface);
    SDL_FreeSurface(text5_surface);/*释放surface*/
    /*将surface拷贝到渲染器*/
    SDL_Rect dstrect;
    dstrect.x = 0;/*显示的起始位置*/
    dstrect.y = 100;/*显示的起始位置*/
    dstrect.w = text5_surface->w;/*显示的宽度*/
    dstrect.h = text5_surface->h;/*显示的高度*/
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    SDL_DestroyTexture(texture);

    string str3="score:"+to_string(score);
    const char * str4 = str3.c_str();
    SDL_Surface *text6_surface = TTF_RenderUTF8_Blended(ttffont, str4, color);
    if (!text6_surface) {
        exit(-15);
    }
    SDL_Texture * texture2 = SDL_CreateTextureFromSurface(renderer, text6_surface);
    SDL_FreeSurface(text6_surface);
    /*将surface拷贝到渲染器*/
    SDL_Rect dstrect2;
    dstrect2.x = 300;/*显示的起始位置*/
    dstrect2.y = 300;/*显示的起始位置*/
    dstrect2.w = text6_surface->w;/*显示的宽度*/
    dstrect2.h = text6_surface->h;/*显示的高度*/
    SDL_RenderCopy(renderer, texture2, NULL, &dstrect2);
    SDL_DestroyTexture(texture2);
}
void Background::skillrender(int skill_type) {
    if (skill_type == 0) {
    }
}
