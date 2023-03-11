#ifndef STORMHEAD_H_
#define STORMHEAD_H_

#include "Sprite.h"
#include "shProjectile.h"
#include "Golem.h"
#include "Goblin.h"

class StormHead {
public:
    static LTexture* texture_idle;
    static LTexture* texture_run;
    static LTexture* texture_attack;
    static LTexture* texture_attack2;
    static LTexture* texture_damaged;
    static LTexture* texture_death;
    static Mix_Chunk* sound_shoot;
    static Mix_Chunk* sound_spawn;
    static Mix_Chunk* sound_gothit;
    static Mix_Chunk* sound_dies;
    StormHead();
    StormHead(const StormHead& e);
    StormHead& operator=(const StormHead& e);
    ~StormHead();

    void Update(std::vector<shProjectile>& pvec, std::vector<Golem*>& gols, std::vector<Goblin*>& gobs, SDL_Point chPos);
    void InitHitbox(int x, int y, int w, int h) ;
    void InitSprite();
    void InitClock();
    SDL_Rect GetHitbox();
    int GetHealth();
    bool CollideProjectile(Projectile& p);

    void Att_shoot1(std::vector<shProjectile>& pvec, SDL_Point chPos);
    void Att_shoot2(std::vector<shProjectile>& pvec, SDL_Point chPos);
    void Att_spawn(std::vector<Golem*>& gols, std::vector<Goblin*>& gobs);

    void Animate();
    void Render(SDL_Renderer* renderer, int camX, int camY);
    int completelyDied = 0;


private:
    const double MAX_SPEED = 9.0;
    const double FRICTION = .9;
    const int MAX_HEALTH = 30;
    double hvel, vvel, hacc, vacc;
    double hitbox_x, hitbox_y, hitbox_w, hitbox_h;

    bool attacking = 0, spawning = 0;
    int got_hit = false;
    int health = 0;
    double chasingSpeed = 0;

    double chaseDist2 = 200 * 200;
    enum {SPR_IDLE, SPR_RUN, SPR_ATTACK, SPR_ATTACK2, SPR_DAMAGED, SPR_DEATH, SPRITE_COUNT};
    enum {ATT_IDLE, ATT_SPAWN, ATT_SHOOT1, ATT_SHOOT2, ATTACK_STATE_COUNT};
    Sprite sprites[SPRITE_COUNT];

    int curr_attack = 0;
    bool idling = 0;
    bool dying = false;
    
    int curr_sprite;
    int attack_click = 0;
    int attack_done = 0;

    int shot = 0, shotTotal = 3;
    int shotDir = 0;

    double next_attack_interval = 5;
    double shooting_interval = 0.2;
    double shooting2_interval = 0.15;
    clock_t mClock, shootingClock;


    double spr_offsetX, spr_offsetY;
    double spr_width, spr_height;

    
};



#endif