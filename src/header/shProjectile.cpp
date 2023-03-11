#include "shProjectile.h"

void shProjectile::InitSprite() {
    sprite.texture = shProjectile::texture;

    spr_width  = texture->GetWidth ();
    spr_height = texture->GetHeight();
    hitbox_w = spr_width; hitbox_h = spr_height;

    sprite.center = {spr_width / 2, spr_height / 2};
    spr_offsetX = -sprite.center.x + hitbox_w / 2;
    spr_offsetY = -sprite.center.y + hitbox_h / 2;
    

    sprite.AddClip({0, 0, spr_width, spr_height});
    sprite.clipTime = 0;

    spr_init_angle = 0;

}