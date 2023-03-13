#ifndef GAMEFLOOR_H_
#define GAMEFLOOR_H_

#include "GameBase.h"
#include "LTexture.h"
#include "Character.h"
#include "Sprite.h"
#include "TileSet.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Golem.h"
#include "Goblin.h"
#include "StormHead.h"

class GameFloor{
public :
    static TTF_Font* font_ui;
    static SDL_Renderer* renderer;
    GameFloor();
    ~GameFloor();
    void GameInit(string path);


    int HandleMouseEvents(SDL_Event e);

    void Update();
    void Draw();

    bool IsWin();
    bool IsLose();
    Character* player;
    int GetHealth();

private:
    SDL_Rect camera;
    int mouseX, mouseY;
    std::vector<Projectile> projectile_vec;
    std::vector<shProjectile> epvec;
    std::vector<Golem*> gols;
    std::vector<Goblin*> gobs;
    std::vector<StormHead*> stormheads;
    LTexture* heart;
    
    TileSet tileSet;

    

};



#endif