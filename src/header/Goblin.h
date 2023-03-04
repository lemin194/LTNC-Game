#ifndef GOBLIN_H_
#define GOBLIN_H_

#include "Enemy.h"
#include "Sprite.h"

class Goblin : public Enemy {
public :
    Goblin();
    Goblin(const Goblin& g);
    using Enemy::operator=;
    Goblin& operator=(const Goblin& g);
    ~Goblin();
    static LTexture* texture;
    void InitSprite();
    void Animate();
    void Render(SDL_Renderer* renderer, int camX, int camY);



private:
    const int MAX_HEALTH = 3;
    double spr_offsetX, spr_offsetY;
    double spr_width, spr_height;
    int curr_sprite = 0;
    Sprite sprites[5];

};


#endif