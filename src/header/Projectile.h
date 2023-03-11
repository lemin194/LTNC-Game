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
    double speed, rad;

protected:
    Sprite sprite;
    double hitbox_x, hitbox_y, hitbox_w, hitbox_h;
    SDL_Point pos;
    int spr_offsetX, spr_offsetY, spr_width, spr_height;
    int spr_init_angle = 0;



};



#endif