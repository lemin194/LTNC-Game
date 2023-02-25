#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "Sprite.h"

class Projectile {
public:
    static LTexture* texture;
    void InitSprite();
    void Init(int x, int y, double rad, double speed);
    SDL_Rect GetHitbox();
    void Update();
    SDL_FPoint GetSpeed();

    void Render(SDL_Renderer* renderer, int camX, int camY);

protected:
    Sprite sprite;
    double speed, rad;
    double hitbox_x, hitbox_y, hitbox_w, hitbox_h;
    int spr_offsetX, spr_offsetY, spr_width, spr_height;



};



#endif