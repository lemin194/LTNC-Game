#include "Weapon.h"

void Weapon::InitSprite() {

    sprite.texture = texture;
    sprite.AddClip({0, 0, texture->GetWidth() / 3, texture->GetHeight()});

    angle = 0;
    spr_width = texture->GetWidth() / 3;
    spr_height = texture->GetHeight();
    spr_offsetX = spr_width / 2;
    spr_offsetY = spr_height / 2 + 12;

    for (int i = 0; i < 3; i++) {
        sprite.AddClip({i * spr_width, 0, spr_width, spr_height});
    }
    sprite.SetFrameId(0);

    sprite.center = {spr_offsetX, spr_offsetY};
    spr_x = 0;
    spr_y = 0;
}
void Weapon::UpdatePos(int x, int y) {
    spr_x = x - spr_offsetX;
    spr_y = y - spr_offsetY;
}

void Weapon::UpdateAngle(double angle) {
    this->angle = angle;
    this->sprite.angle = angle;
}
void Weapon::Flip(SDL_RendererFlip flip) {
    this->flip = 1 - 2 * (flip == SDL_FLIP_HORIZONTAL);
    sprite.flip = flip;
}
void Weapon::Render(SDL_Renderer* renderer, int camX, int camY) {
    sprite.Render(spr_x - camX, spr_y - camY, renderer);
}