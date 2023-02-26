#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Sprite.h"
#include "Weapon.h"
#include "Projectile.h"
#include "shProjectile.h"

class Character {
public:
    static LTexture* textures[3];

    Character();
    ~Character();

    void InitSprite();
    void LoadWeaponTexture (std::string* path, SDL_Renderer* gRenderer);
    void Update();
    void AddForce(double hacc, double vacc);
    void ReceiveMouseInput(int &camX, int &camY);
    void ShootProjectile(int &camX, int &camY, std::vector<Projectile> & pvec);
    void Render(SDL_Renderer* renderer, int camX, int camY);
    void InitHitbox(int x, int y, int w, int h) ;
    SDL_Rect GetHitbox();
    void PostUpdate();
    void Collide(SDL_Rect rcheck);
    void AnimateMoving(int hsign);
    void AnimateIdle();
    void AnimateWeapon();
    bool CollideProjectile(shProjectile& p);
    bool TakeDamage(SDL_Point ePos, int damage);
    int health = 5;
    
private:
    const double MAX_SPEED = 9.0;
    const double FRICTION = .71;
    double hvel, vvel, hacc, vacc;
    double hitbox_x, hitbox_y, hitbox_w, hitbox_h;


    double spr_offsetX, spr_offsetY;
    double spr_width, spr_height;
    Sprite sprites[3];
    int curr_sprite = 0;

    clock_t firing_clock;
    double firingRate;

    Weapon weapon;

    bool got_hit = 1;
    clock_t gothit_clock;

    double kb_coefficient = 0.5;
    double gothit_duration = 0.4;

};


#endif