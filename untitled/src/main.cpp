#include "../include/common.h"
#include "../include/background.h"
#include "../include/enemy.h"

#ifdef __WIN32
#define SDL_main main

#endif

int main(int argc, char *argv[]) {
    Background background;
    background.init();
    background.gameplay();
    
    return 0;
}