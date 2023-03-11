#include "Projectile.h"

void Projectile::InitSprite() {
    sprite.texture = Projectile::texture;


    spr_width  = texture->GetWidth ();
    spr_height = texture->GetHeight();
    
    hitbox_w = spr_width; hitbox_h = spr_height;

    sprite.center = {spr_width / 2, spr_height / 2};
    spr_offsetX = - sprite.center.x + hitbox_w / 2;
    spr_offsetY = - sprite.center.y + hitbox_h / 2;
    

    spr_init_angle = 0;

    sprite.AddClip({0, 0, spr_width, spr_height});
    sprite.clipTime = 0;
}

void Projectile::Init(int x, int y, double rad, double speed) {
    pos = {x, y};
    hitbox_x = pos.x - hitbox_w / 2;
    hitbox_y = pos.y - hitbox_h / 2;
    this->rad = rad;
    this->speed = speed;
}

void Projectile::Update() {
    double xmove = speed * std::sin(rad);
    double ymove = speed * std::cos(rad);

    this->hitbox_x += xmove;
    this->hitbox_y += ymove;
    sprite.angle = spr_init_angle - rad * 180 / PI;
    sprite.Animate();
}

SDL_FPoint Projectile::GetSpeed() {
    return {speed, rad};
}

SDL_Rect Projectile::GetHitbox() {
    return {hitbox_x, hitbox_y, hitbox_w, hitbox_h};
}


void Projectile::Render(SDL_Renderer* gRenderer, int camX, int camY) {

    // SDL_SetRenderDrawColor(gRenderer, 255, 32, 64, 255);
    // SDL_Rect hitbox = {hitbox_x - camX, hitbox_y - camY, hitbox_w, hitbox_h};
    // SDL_RenderDrawRect(gRenderer, &hitbox);
    
    double render_x = spr_offsetX + hitbox_x,
        render_y = spr_offsetY + hitbox_y;

    this->sprite.Render(render_x - camX, render_y - camY, gRenderer);
}