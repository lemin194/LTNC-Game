#ifndef SHPROJECTILE_H_
#define SHPROJECTILE_H_

#include "Projectile.h"

class shProjectile : public Projectile {
public:
    static LTexture* texture;
    void InitSprite();

    
};


#endif