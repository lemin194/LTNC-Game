#include "Fireball.h"

void Fireball::InitSprite() {
    sprite.texture = Fireball::texture;

    spr_width  = 64;
    spr_height = 32;
    hitbox_w = 16; hitbox_h = 16;
    hitbox_x = pos.x - hitbox_w / 2;
    hitbox_y = pos.y - hitbox_h / 2;

    sprite.center = {57, 17};
    spr_offsetX = -sprite.center.x + hitbox_w / 2;
    spr_offsetY = -sprite.center.y + hitbox_h / 2;
    
    // std::cout << spr_width << " " << spr_height << " " << spr_offsetX << " " << spr_offsetY << "\n";

    for (int i = 0; i < 4; i++)
    sprite.AddClip({spr_width * i, 0, spr_width, spr_height});
    sprite.clipTime = 0.12;

    spr_init_angle = 90;

}

void Fireball::UpdatePos(int x, int y) {
    hitbox_x = x - hitbox_w / 2; hitbox_y = y - hitbox_h / 2;
}