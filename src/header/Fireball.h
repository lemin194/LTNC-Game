#ifndef FIREBALL_H_
#define FIREBALL_H_

#include "Projectile.h"

class Fireball : public Projectile {
public:
    static LTexture* texture;
    void InitSprite();
    void UpdatePos(int x, int y);
    
};

#endif