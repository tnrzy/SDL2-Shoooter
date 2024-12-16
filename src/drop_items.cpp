#include "../include/common.h"

#include "../include/drop_items.h"

drop_items::drop_items(SDL_Renderer* renderer): dropSurface(nullptr), dropTexture(nullptr) {
    myLog = fopen("drop_items.log", "w");
    if (!myLog) {
        exit(-1);
    }
    dropSurface = IMG_LoadPNG_RW(SDL_RWFromFile("res/png/drop_item.png", "rb"));
    if (!dropSurface) {
        fprintf(myLog, "SDL_drop_item_Surface Error: %s\n", IMG_GetError());
        exit(-5);
    }
    width = dropSurface->w;
    height = dropSurface->h;
    dropTexture = SDL_CreateTextureFromSurface(renderer, dropSurface);
    SDL_FreeSurface(dropSurface);
    dropSurface = nullptr;

}
drop_items:: ~drop_items(){
    if (myLog) {
        fclose(myLog);
    }

    if (dropSurface) {
        SDL_FreeSurface(dropSurface);
    }

    if (dropTexture) {
        SDL_DestroyTexture(dropTexture);
    }
    if (!dropposition.empty()){
        dropposition.clear();
    }

}

void drop_items::add_drop(SDL_Rect rect, int num) {
    int enemy_width = rect.w;
    int enemy_height = rect.h;
    for (int i = 0; i < num ;i++) {
        static std::uniform_int_distribution<int> distribution(0, 1000);
        generator.seed((unsigned) time(nullptr) + generator());
        int x_add = distribution(generator) % (enemy_width - width);
        int y_add = distribution(generator) % (enemy_height / 2);
        SDL_Rect drop = {rect.x + x_add, rect.y + y_add, width, height};
        int dx = 0;
        if (x_add < (enemy_width - width) / 3) {
            dx = -1;
        }
        if (x_add > (enemy_width - width) * 2 / 3) {
            dx = 1;
        }
        int dy = 6 + distribution(generator) % 3;
        drop_info *new_info = new drop_info(drop, dx, dy);
        dropposition.reserve(150);
        dropposition.push_back(new_info);
    }

}
void drop_items::render(SDL_Renderer *renderer) {
    if (!dropposition.empty()){
        for(int i = 0; i < dropposition.size(); i++){
            dropposition[i]->rect.y += dropposition[i] ->dy;
            dropposition[i]->rect.x += dropposition[i] ->dx;
            if(dropposition[i]->stage<=60){
                dropposition[i]->stage ++;
            }
            else{
                dropposition[i]->dx = 0;
            }
            SDL_Rect rect0 = dropposition[i]->rect;
            SDL_RenderCopy(renderer,dropTexture, nullptr,&rect0);
        }
    }
}
