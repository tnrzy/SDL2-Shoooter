//
// Created by Chenyu Zhang on 2024/11/30.
//

#ifndef INC_3002_2_DROP_ITEMS_H
#define INC_3002_2_DROP_ITEMS_H
#include "common.h"
class drop_items{
    class drop_info{
    public:
        SDL_Rect rect;
        int type;
        int dx;
        int dy;
        int stage;
        drop_info(SDL_Rect rect,int dx, int dy){
            this->rect = rect;
            this->dx = dx;
            this->dy = dy;
            this->stage = 0;
        }
    };
public:
    explicit drop_items(SDL_Renderer *renderer);
    ~drop_items();
    void render(SDL_Renderer *renderer);
    void add_drop(SDL_Rect rect,int num);
    //void start();
    //void update_pos();
    std::vector<drop_info*> dropposition;
    int width;
    int height; //宽度和高度
protected:
    SDL_Surface *dropSurface;
    SDL_Texture *dropTexture;
    FILE *myLog;
    std::default_random_engine generator;

};
#endif //INC_3002_2_DROP_ITEMS_H
