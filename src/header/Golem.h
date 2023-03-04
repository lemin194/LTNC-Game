#ifndef GOLEM_H_
#define GOLEM_H_

#include "Enemy.h"
#include "Sprite.h"

class Golem : public Enemy {
public :
    Golem();
    Golem(const Golem& g);
    using Enemy::operator=;
    Golem& operator=(const Golem& g);
    ~Golem();
    static LTexture* texture;
    void InitSprite();
    void Animate();
    void Render(SDL_Renderer* renderer, int camX, int camY);



private:
    const int MAX_HEALTH = 5;
    double spr_offsetX, spr_offsetY;
    double spr_width, spr_height;
    int curr_sprite = 0;
    Sprite sprites[5];

};


#endif