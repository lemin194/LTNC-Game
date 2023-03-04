#include "shProjectile.h"

void shProjectile::InitSprite() {
    sprite.texture = shProjectile::texture;

    spr_width  = texture->GetWidth ();
    spr_height = texture->GetHeight();
    hitbox_w = spr_width; hitbox_h = spr_height;

    spr_offsetX = - spr_width / 2 + hitbox_w / 2;
    spr_offsetY = - spr_height / 2 + hitbox_h / 2;
    
    // std::cout << spr_width << " " << spr_height << " " << spr_offsetX << " " << spr_offsetY << "\n";


    sprite.AddClip({0, 0, spr_width, spr_height});
    sprite.clipTime = 0;
}