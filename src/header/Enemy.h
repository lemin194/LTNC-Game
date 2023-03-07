#ifndef ENEMY_H_
#define ENEMY_H_

#include "LTexture.h"
#include "Character.h"
#include "Projectile.h"

class Enemy {
public:
    Enemy();
    Enemy(const Enemy& e);
    Enemy& operator=(const Enemy& e);
    ~Enemy();

    static Mix_Chunk* sound_gothit;

    void AddForce(double hacc, double vacc);
    void ChaseCharacter(Character& c);
    void InitHitbox(int x, int y, int w, int h) ;
    void InitClock();
    SDL_Rect GetHitbox();
    void PostUpdate();
    void Collide(SDL_Rect rcheck);
    int GetHealth();
    bool CollideProjectile(Projectile& p);
    int completelyDied = 0;


protected:
    const double MAX_SPEED = 9.0;
    const double FRICTION = .9;
    const double dawdleInterval = 1.75;
    double hvel, vvel, hacc, vacc;
    double hitbox_x, hitbox_y, hitbox_w, hitbox_h;
    double dawdlen = 100, dawdlingInterval = 1.5;
    const double dyingDuration = 1;
    double dawddir = 0, dawdled = 0;
    clock_t dawdling_clock, got_hit_clock, dying_clock;

    clock_t attack_clock;
    double attack_interval = 0.5;

    int dawdling = false;
    int chasing = false;
    int got_hit = false;
    int health = 5;
    double kb_coeficient = 1;
    double chasingSpeed = 0;

    int damage = 1;

    double chaseDist2 = 200 * 200;
    double attackDist2 = 15 * 15;

};


#endif