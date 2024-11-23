//
// Created by Chenyu Zhang on 2024/11/18.
//

#include "stage.h"
#include "common.h"
#include "draw.h"
#include <iostream>
static void logic();
static void draw();
static void initPlayer();
static void fireBullet();
static void doPlayer();
static void doBullets();
static void doFighters();
static void drawPlayer();
static void drawBullets();
static void drawFighters();
static void SpawnEnemies();
//App app;
//Stage stage;
static Entity *player;
static SDL_Texture *bulletTexture;
static SDL_Texture *enemyTexture;
static int enemySpawnTimer;
void initStage()
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    initPlayer();

    bulletTexture = loadTexture("pics/playerBullet.png");
    enemyTexture = loadTexture("pics/enemy.png");
    enemySpawnTimer = 0;
}

static void initPlayer()
{
    player = new Entity();
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x = 100;
    player->y = 100;
    player->texture = loadTexture("pics/player1.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}
static void logic(){
    doPlayer();
    doBullets();
    doFighters();
    SpawnEnemies();
}
static void doPlayer(){
    player->dx =player->dy = 0;
    if (player -> reload > 0){
        player -> reload --;
    }
    if(app.keyboard[SDL_SCANCODE_UP]){
        player -> dy = -PLAYER_SPEED;
    }
    if(app.keyboard[SDL_SCANCODE_DOWN]){
        player -> dy = PLAYER_SPEED;
    }
    if(app.keyboard[SDL_SCANCODE_RIGHT]){
        player -> dx = PLAYER_SPEED;
    }
    if(app.keyboard[SDL_SCANCODE_LEFT]){
        player -> dx = -PLAYER_SPEED;
    }
    if (app.keyboard[SDL_SCANCODE_SPACE] && player -> reload == 0){
        fireBullet();
    }
    player -> x += player ->dx;
    player -> y += player ->dy;
}
static void fireBullet(){
    auto * bullet = new Entity();
    stage.bulletTail -> next = bullet;
    stage.bulletTail = bullet;
    bullet ->x = player ->x;
    bullet ->y = player ->y;
    bullet ->dy = -BULLET_SPEED;
    bullet ->health = 1;
    bullet ->texture = bulletTexture;
    SDL_QueryTexture(bullet -> texture,NULL, NULL, &bullet -> w, &bullet -> h);
    bullet ->x += 17;
    player -> reload = 3;
}
static void doBullets(){
    Entity *b, * prev;
    prev = &stage.bulletHead;
    for (b = stage.bulletHead.next; b != nullptr; b = b->next){
        b->x += b->dx;
        b->y += b->dy;
        if (b->y < 0){
            if (b == stage.bulletTail){
                stage.bulletTail = prev;
            }
            prev ->next = b->next;
            free(b);
            b = prev;
        }
        prev = b;
    }
}
static void doFighters(){

}
static void draw()
{
    drawPlayer();

    drawBullets();
}

static void drawPlayer(){
    blit(player->texture,player->x,player->y);

}
static void drawBullets()
{
    Entity *b;

    for (b = stage.bulletHead.next ; b != nullptr ; b = b->next)
    {
        blit(b->texture, b->x, b->y);
    }
}
