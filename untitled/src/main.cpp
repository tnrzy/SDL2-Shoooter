#include "../include/common.h"
#include "../include/background.h"
#include "../include/enemy.h"

#ifdef __WIN32
#define SDL_main main

#endif

int main() {
    Background background;
    background.init();
    background.gameplay();
    
    return 0;
}