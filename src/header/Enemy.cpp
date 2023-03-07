
#include "Enemy.h"
#include "Projectile.h"
void Enemy::InitHitbox(int x, int y, int w, int h) {
    hitbox_x = x;
    hitbox_y = y;
    hitbox_w = w;
    hitbox_h = h;
    this->dawdling_clock = rand() % (int)(CLOCKS_PER_SEC * dawdlingInterval);
}

void Enemy::InitClock() {
    dawdling_clock = clock();
    dawdlingInterval = dawdleInterval + (rand() % 100) * 0.005;
}

SDL_Rect Enemy::GetHitbox() {
    return {hitbox_x, hitbox_y, hitbox_w, hitbox_h};
}


void Enemy::AddForce(double hacc, double vacc) {
    this->hvel += hacc;
    this->vvel += vacc;
}

void Enemy::ChaseCharacter(Character& c) {
    double px, py;
    px = c.GetHitbox().x + c.GetHitbox().w / 2;
    py = c.GetHitbox().y + c.GetHitbox().h / 2;
    // hitbox_x += rand() % 3 - 1;
    if (health <= 0) {
        hvel = 0; vvel = 0;
        return;
    }

    double ex = hitbox_x + hitbox_w * 0.5;
    double ey = hitbox_y + hitbox_h * 0.5;
    double d2 = (px - ex) * (px - ex) + (py - ey) * (py - ey);
    // std::cout << d2 << "d2\n";
    // std::cout << "enemy: " << health << " " << ex << "_" << ey << chasing << "_" << dawdling << "_" << "c";
    double vel = chasingSpeed;
    double coe = .2;

    if (d2 < attackDist2) {
        clock_t curr_time = clock();
        if (((double) curr_time - (double)attack_clock) / CLOCKS_PER_SEC >= attack_interval) {
            if (c.TakeDamage({(int)ex, (int)ey}, damage)) attack_clock = clock();
        }
    }

    if (chasing) {
        if (d2 > chaseDist2 || d2 < 50) {
            chasing = false;
        }
        else {
            double deltax = px - ex, deltay = py - ey;
            double theta = std::atan2(deltay, deltax);

            hvel += (vel * cos(theta) - hvel) * coe;
            vvel += (vel * sin(theta) - vvel) * coe;
        }
    }
    else if (dawdling) {
        hvel += (vel * cos(dawddir) - hvel) * coe;
        vvel += (vel * sin(dawddir) - vvel) * coe;

        dawdled += vel;
        if (dawdled > dawdlen) {
            dawdled = 0;
            dawdling = 0;
            chasing = 0;
        }
    }
    else {
        if (d2 <= chaseDist2) {
            chasing = true;
        }
        else {
            clock_t curr_time = clock();
            if (((double)curr_time - (double)dawdling_clock) / CLOCKS_PER_SEC >= dawdlingInterval) {
                dawdling_clock = clock();
                
                dawdlingInterval = dawdleInterval + (rand() % 100) * 0.005;
                dawdling = true;
                
                dawddir = (rand() % 360) * 2 * PI / 360;
                dawdled = 0;
            } 
            // std::cout << "...";
        }
    }
    // if (hvel > 0) std::cout << hvel << "\n";
    this->hvel = FRICTION * this->hvel;
    this->vvel = FRICTION * this->vvel;
    if (this->hvel >  MAX_SPEED) this->hvel =  MAX_SPEED;
    if (this->hvel < -MAX_SPEED) this->hvel = -MAX_SPEED;
    if (this->vvel >  MAX_SPEED) this->vvel =  MAX_SPEED;
    if (this->vvel < -MAX_SPEED) this->vvel = -MAX_SPEED;

    // std:: cout << "\n";
    // std::cout << "hvel: " << hvel << ", hx: ";
}

int Enemy::GetHealth() {
    return health;
}

void Enemy::Collide(SDL_Rect rcheck) {
    double cx = hitbox_x, cy = hitbox_y, cw = hitbox_w, ch = hitbox_h;
    int hsign = hvel == 0 ? 0 : (hvel > 0 ? 1 : -1);
    int vsign = vvel == 0 ? 0 : (vvel > 0 ? 1 : -1);
    SDL_Rect hitbox = {hitbox_x, hitbox_y, hitbox_w, hitbox_h};
    SDL_Rect hhitbox = {hitbox_x + hvel, hitbox_y, hitbox_w, hitbox_h};
    SDL_Rect vhitbox = {hitbox_x, hitbox_y + vvel, hitbox_w, hitbox_h};
    
    if (CheckCollisionRectangle(hhitbox, rcheck)) {
        SDL_Rect new_hhitbox = hitbox;
        int cnt = 0;
        while (!CheckCollisionRectangle(new_hhitbox, rcheck)) {
            new_hhitbox.x += hsign;
            cnt ++;
        }

        // std::cout << cnt << " " << rand() << "\n";
        hvel = 0;
        hitbox_x = new_hhitbox.x - hsign;
    }
    
    if (CheckCollisionRectangle(vhitbox, rcheck)) {
        SDL_Rect new_vhitbox = hitbox;
        while (!CheckCollisionRectangle(new_vhitbox, rcheck) ) {
            new_vhitbox.y += vsign;
        }
        vvel = 0;
        int cnt = 0;
        // std::cout << cnt << " " << rand() << "\n";
        hitbox_y = new_vhitbox.y - vsign;
    }

}

bool Enemy::CollideProjectile(Projectile& p) {
    SDL_Rect rcheck = p.GetHitbox();
    if (!CheckCollisionRectangle(rcheck, GetHitbox())) return 0;

    double px = rcheck.x + rcheck.w * 0.5, py = rcheck.y + rcheck.h * 0.5;
    double mx = hitbox_x + hitbox_w * 0.5, my = hitbox_y + hitbox_h * 0.5;

    double deltax = mx - px, deltay = my - py;
    auto [pspeed, pdir] = p.GetSpeed();
    double pvelx = sin(pdir) * pspeed * 0.5;
    double pvely = cos(pdir) * pspeed * 0.5;

    // std::cout << deltax * 0.5 << "-" << pvelx << "\n";
    double mult = 0.5;
    AddForce(kb_coeficient * (deltax * mult + pvelx), kb_coeficient * (deltay * mult + pvely));

    health --;
    got_hit = true;
    if (!health) dying_clock = clock();
    Mix_PlayChannel(-1, sound_gothit, 0);
    return 1;

}

void Enemy::PostUpdate() {
    hitbox_x += this->hvel;
    hitbox_y += this->vvel;
}


Enemy::Enemy() {
    dawdling_clock = clock();
    got_hit_clock = clock();
    dying_clock = clock();
    attack_clock = clock();
    hvel = 0;
    vvel = 0;
    hacc = 0;
    vacc = 0;
    hitbox_x = 0;
    hitbox_y = 0;
    hitbox_w = 0;
    hitbox_h = 0;
    dawdlen = 100;
    dawdlingInterval = dawdleInterval + (rand() % 100) * 0.005;
    dawddir = 0;
    dawdled = 0;
    dawdling = false;
    chasing = false;
    got_hit = false;
    health = 5;
    damage = 1;
    kb_coeficient = 1;
    chasingSpeed = 2;

    chaseDist2 = 200 * 200;
    completelyDied = 0;

}

Enemy::~Enemy() {
    
    hvel = 0;
    vvel = 0;
    hacc = 0;
    vacc = 0;
    hitbox_x = 0;
    hitbox_y = 0;
    hitbox_w = 0;
    hitbox_h = 0;
    dawdlen = 0;
    dawdlingInterval = 0;
    dawddir = 0;
    dawdled = 0;
    dawdling = false;
    chasing = false;
    got_hit = false;
    health = 0;
    kb_coeficient = 1;
    chasingSpeed = 0;

    chaseDist2 = 0;
    completelyDied = 0;

}

Enemy::Enemy(const Enemy& e){
    hvel = e.hvel;
    vvel = e.vvel;
    hacc = e.hacc;
    vacc = e.vacc;
    hitbox_x = e.hitbox_x;
    hitbox_y = e.hitbox_y;
    hitbox_w = e.hitbox_w;
    hitbox_h = e.hitbox_h;
    dawddir = e.dawddir;
    dawdled = e.dawdled;
    dawdling = e.dawdling;
    chasing = e.chasing;
    got_hit = e.got_hit;
    health = e.health;
    kb_coeficient = e.kb_coeficient;
    chasingSpeed = e.chasingSpeed;

    completelyDied = e.completelyDied;
}

Enemy& Enemy::operator=(const Enemy& e) {
    hvel = e.hvel;
    vvel = e.vvel;
    hacc = e.hacc;
    vacc = e.vacc;
    hitbox_x = e.hitbox_x;
    hitbox_y = e.hitbox_y;
    hitbox_w = e.hitbox_w;
    hitbox_h = e.hitbox_h;
    dawddir = e.dawddir;
    dawdled = e.dawdled;
    dawdling = e.dawdling;
    chasing = e.chasing;
    got_hit = e.got_hit;
    health = e.health;
    kb_coeficient = e.kb_coeficient;
    chasingSpeed = e.chasingSpeed;

    completelyDied = e.completelyDied;

    return *this;
}