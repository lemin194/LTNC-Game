#ifndef WEAPON_H_
#define WEAPON_H_

#include "Sprite.h"
class Weapon {

public:
    static LTexture* texture;
    void InitSprite();
    void UpdatePos(int x, int y);
    void UpdateAngle(double angle);
    void Render(SDL_Renderer* renderer, int camX, int camY);
    void Flip(SDL_RendererFlip flip);
    int wielding = 0;
    Sprite sprite;


private:
    double angle;
    int spr_x, spr_y, spr_width, spr_height;
    int spr_offsetX, spr_offsetY;
    int flip;
    double offset_angle = 0.2;
};

#endif