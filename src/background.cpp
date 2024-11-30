#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>
#include "../include/background.h"
#include "../include/enemy.h"
#include "../include/Player.h"

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

    backgroundSurface = IMG_LoadJPG_RW(SDL_RWFromFile("res/png/start.jpg", "rb"));
    minion_width = backgroundSurface->w * 0.8; //选择产生的宽度为背景图片的0.8倍
    playerX = backgroundSurface->w / 2;
    playerY = 7*backgroundSurface->h / 8;

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

    SDL_Event event; //用于接受传入的各类事件
    uint32_t begin, end, elapsed, rate;
    uint32_t delay; //用于控制帧率
    bool isTurbo = false;



    while (true) {
        begin = SDL_GetTicks();
        render(); //绘制图像
        int speed = 5;

        while (SDL_PollEvent(&event)) { //因为要修改event所以pass by ref; 用while是为了在一轮内把事件都处理完
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (state != PLAYING) {
                            state = PLAYING;
                        } else {
                            bullet->attack = true;
                        }break;


                    case SDLK_LSHIFT:
                        isTurbo = true; // 当键被按下时，开启加速模式
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
                }
                if (event.key.keysym.sym == SDLK_SPACE) {
                    bullet -> attack = false;
                }
            }

            if (event.type == SDL_QUIT) {
                SDL_Quit();
            }
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        if (state == PLAYING) {
            if (keystate[SDL_SCANCODE_LEFT]) {
                playerX -= isTurbo ? speed : 2; // 使用键的状态来决定速度
            }
            if (keystate[SDL_SCANCODE_RIGHT]) {
                playerX += isTurbo ? speed: 2;
            }
            if (keystate[SDL_SCANCODE_DOWN]) {
                playerY += isTurbo ? speed: 2;
            }
            if (keystate[SDL_SCANCODE_UP]) {
                playerY -= isTurbo ? speed: 2;
            }
        }
        SDL_Delay(16);
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
            delete bullet;
            delete player; //删除对象达到重新开始的效果,再次开始时需要重新创建
            bullet = new Bullet(renderer);
            player = new Player(renderer);
            minion = new Enemy(renderer);
            break;
        case PLAYING:
            this->loadJPG("res/png/background.jpg");
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

            player -> render(playerX, playerY); //利用传入的数据刷新player的渲染器；在此处渲染是为了保证每次渲染屏幕时，player都会被渲染到
            minion->render(renderer,minion_width); //minion_width是当前窗口的宽度
            bullet->render(renderer, playerX, playerY, player); //常规子弹的渲染
            for (auto itMinion = minion->positions.begin(); itMinion != minion->positions.end();) {
                for (auto itBullet = bullet->bulletPosition.begin(); itBullet != bullet->bulletPosition.end();) { //对子弹的检测
                    if (itBullet->x >= ((*itMinion)->position.x-bullet->width + 2)
                        && itBullet->x <= ((*itMinion)->position.x + minion->widths[(*itMinion)->type] - 2)
                        && itBullet->y >= (*itMinion)->position.y
                        && itBullet->y <= ((*itMinion)->position.y + minion->heights[(*itMinion)->type]) - 7) {
                        if (itBullet != bullet->bulletPosition.end()) { //防止数组越界
                            itBullet = bullet->bulletPosition.erase(itBullet);
                        } else {
                            bullet->bulletPosition.pop_back();
                        }
                        if (itMinion != minion->positions.end()) { //防止数组越界
                            (*itMinion)->health = (*itMinion)->health - 1;
                            double k = (*itMinion)->position.w * ((*itMinion)->health)/((*itMinion)->max_health);
                            (*itMinion)->health_bar.w = static_cast<int>(std::round(k));
                            if ((*itMinion)->health == 0){
                                itMinion = minion->positions.erase(itMinion);
                            }
                        } else {
                            minion->positions.pop_back();
                        }
                    } else {
                        if (itBullet != bullet->bulletPosition.end()) {
                            ++itBullet;
                        }
                    }
                }

                if ((*itMinion)->position.x >= playerX && (*itMinion)->position.x <= (playerX + minion->widths[(*itMinion)->type])
                    && (*itMinion)->position.y >= playerY && (*itMinion)->position.y <= (playerY + minion->heights[(*itMinion)->type])) {
                    state = END;
                }

                if (itMinion != minion->positions.end()) {
                    ++itMinion;
                }
            }
            break;
    }

    SDL_RenderPresent(renderer); //此处会present一次
}