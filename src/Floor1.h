#ifndef FLOOR1_H_
#define FLOOR1_H_

#include "GameFloor.h"
class Floor1 : public GameFloor {

public:
    virtual int HandleMouseEvents(SDL_Event e);
    virtual void GameInit();
    virtual void Update();
    virtual void Draw();

private :
    StormHead* stormhead;
    TileSet tileSet;

    SDL_Rect camera;
    std::vector<Projectile> projectile_vec;
    std::vector<shProjectile> epvec;
    std::vector<Golem*> gols;
    std::vector<Goblin*> gobs;
    LTexture gTextTexture1;


};

#endif
