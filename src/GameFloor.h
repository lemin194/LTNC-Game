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
    static TTF_Font* gFont;
    static SDL_Renderer* renderer;
    GameFloor();
    virtual ~GameFloor();
    virtual void GameInit();
    

    virtual int HandleMouseEvents(SDL_Event e);

    virtual void Update();
    virtual void Draw();
    Character* player;

protected:
    SDL_Rect camera;
    int mouseX, mouseY;

    

};



#endif