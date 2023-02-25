
#include "StormHead.h"
#include "Projectile.h"

void StormHead::InitHitbox(int x, int y, int w, int h) {
    hitbox_x = x;
    hitbox_y = y;
    hitbox_w = w;
    hitbox_h = h;
}

void StormHead::InitSprite() {
    sprites[SPR_IDLE]   .texture = texture_idle   ;
    sprites[SPR_RUN]    .texture = texture_run    ;
    sprites[SPR_ATTACK] .texture = texture_attack ;
    sprites[SPR_ATTACK2].texture = texture_attack2;
    sprites[SPR_DAMAGED].texture = texture_damaged;
    sprites[SPR_DEATH]  .texture = texture_death  ;

    for (int i = 0; i < SPRITE_COUNT; i++) {
        sprites[i].SetClipTime(0.1);
        sprites[i].resize = 3;
    }

    curr_sprite = SPR_IDLE;

    spr_width = 192;
    spr_height = 192;
    spr_offsetX = hitbox_w * 0.5 - 288;
    spr_offsetY = hitbox_h * 0.5 - 370;

    for (int i = 0; i < 10; i++) {
        sprites[SPR_IDLE].AddClip({(int)(i * spr_width), 0, (int)spr_width, (int)spr_height});
    }
    sprites[SPR_IDLE].SetLoop(1);
    sprites[SPR_IDLE].SetClipTime(0.2);

    for (int i = 0; i < 10; i++) {
        sprites[SPR_RUN].AddClip({(int)(i * spr_width), 0, (int)spr_width, (int)spr_height});
    }
    sprites[SPR_RUN].SetLoop(1);

    for (int i = 0; i < 20; i++) {
        sprites[SPR_ATTACK].AddClip({(int)(i * spr_width), 0, (int)spr_width, (int)spr_height});
    }
    sprites[SPR_ATTACK].SetLoop(0);

    for (int i = 0; i < 13; i++) {
        sprites[SPR_ATTACK2].AddClip({(int)(i * spr_width), 0, (int)spr_width, (int)spr_height});
    }
    sprites[SPR_ATTACK2].SetLoop(0);

    for (int i = 0; i < 2; i++) {
        sprites[SPR_DAMAGED].AddClip({(int)((i % 2) * spr_width), 0, (int)spr_width, (int)spr_height});
    }
    sprites[SPR_DAMAGED].SetLoop(0);

    for (int i = 0; i < 9; i++) {
        sprites[SPR_DEATH].AddClip({(int)(i * spr_width), 0, (int)spr_width, (int)spr_height});
    }
    sprites[SPR_DEATH].SetLoop(0);


    for (int i = 0; i < SPRITE_COUNT; i++) {
        sprites[i].SetFrameId(0);
    }
}


void StormHead::Animate() {
    // if (curr_sprite != 0)
    // std::cout << curr_sprite << "\n";
    if (health > 0) {
        if (attack_done) {
            if (got_hit) {
                if (curr_sprite != SPR_DAMAGED) { 
                    curr_sprite = SPR_DAMAGED;
                    sprites[curr_sprite].SetFrameId(0);
                    sprites[curr_sprite].InitClock();
                }
                else if (sprites[curr_sprite].Animate()) got_hit = false;
            }
            else {
                curr_sprite = SPR_IDLE;
                sprites[curr_sprite].Animate();
            }
        }
        else {
            if (this->curr_attack == ATT_SPAWN && curr_sprite != SPR_ATTACK) {
                this->curr_sprite = SPR_ATTACK;
                sprites[curr_sprite].SetFrameId(0);
                sprites[curr_sprite].InitClock();
                
            }
            else if (this->curr_attack == ATT_SHOOT1 && curr_sprite != SPR_ATTACK2) {
                this->curr_sprite = SPR_ATTACK2;
                shotDir = rand() % 360;
                sprites[curr_sprite].SetFrameId(0);
                sprites[curr_sprite].InitClock();
            }
            else if (this->curr_attack == ATT_SHOOT2 && curr_sprite != SPR_ATTACK2) {
                this->curr_sprite = SPR_ATTACK2;
                sprites[curr_sprite].SetFrameId(0);
                sprites[curr_sprite].InitClock();
            }
            else {
                
                int fid = this->sprites[curr_sprite].GetFrameId();
                if (this->curr_attack == ATT_SPAWN) {
                    if (fid >= 8) attack_click = std::max(attack_click, 1);
                    if (fid >= 12) attack_click = std::max(attack_click, 3);
                }
                else if (this->curr_attack == ATT_SHOOT1) {
                    if (fid >= 6) {
                        attack_click = std::max(attack_click, 1);
                    }
                }
                else if (this->curr_attack == ATT_SHOOT2) {
                    if (fid >= 6) {
                        attack_click = std::max(attack_click, 1);
                    }
                }
                if (sprites[curr_sprite].Animate()) {
                    attack_done = 1;
                    attack_click = 0;
                    curr_sprite = SPR_IDLE;
                    sprites[curr_sprite].SetFrameId(0);
                }

            }

        }
    }
    else {
        if (curr_sprite != SPR_DEATH) {
            curr_sprite = SPR_DEATH;
            sprites[curr_sprite].SetFrameId(0);
            sprites[curr_sprite].InitClock();
        }
        else {
            if (sprites[curr_sprite].Animate()) {
                completelyDied = true;
            }
        }
    }

}

void StormHead::InitClock() {
    mClock = clock(); 
    shootingClock = clock();
}

SDL_Rect StormHead::GetHitbox() {
    return {hitbox_x, hitbox_y, hitbox_w, hitbox_h};
}


void StormHead::Update(std::vector<shProjectile>& pvec, std::vector<Golem*>& gols, std::vector<Goblin*>& gobs, SDL_Point chPos) {
    clock_t curr_time = clock();
    if (((double)curr_time - (double)mClock) / CLOCKS_PER_SEC > next_attack_interval) {
        curr_attack = (curr_attack + 1) % ATTACK_STATE_COUNT;
        if (curr_attack == 0) curr_attack ++;
        attack_done = 0;
        attack_click = 0;
        mClock = clock();
    }
    if (!attack_done) {
        got_hit = false;
        if (this->curr_attack == ATT_IDLE) {
            attack_done = 1;
            attack_click = 0;
        }
        else if (this->curr_attack == ATT_SPAWN) {
            if (attack_click == 1 || attack_click == 3) Att_spawn(gols, gobs);
        }
        else if (this->curr_attack == ATT_SHOOT1) {
            if (attack_click >= 1 && attack_click <= 4) Att_shoot1(pvec, chPos);

        }
        else if (this->curr_attack == ATT_SHOOT2) {
            if (attack_click >= 1 && attack_click <= 5) Att_shoot2(pvec, chPos);
            
        }
    }
}

void StormHead::Att_shoot1(std::vector<shProjectile>& pvec, SDL_Point chPos) {
    double sx = hitbox_x + hitbox_w * 0.5;
    double sy = hitbox_y + hitbox_h * 0.5;
    clock_t curr_time = clock();
    if (((double)curr_time - (double)shootingClock) / CLOCKS_PER_SEC >= shooting_interval) {
        shootingClock = clock();
        int bullet_count = 12;
        double bullet_speed = 3;
        double angle_gap = 360 / bullet_count;
        double radius = 30;
        double shotDir = atan2(chPos.x - sx, chPos.y - sy) * 180 / PI;
        for (int i = 0; i < bullet_count; i++) {
            double theta = angle_gap * i + shotDir;
            theta = PI * theta / 180;
            shProjectile shp;
            shp.Init(sx + radius * sin(theta), sy + radius * cos(theta), theta, bullet_speed);
            shp.InitSprite();
            pvec.push_back(shp);
        }
        attack_click ++;
    }
}

void StormHead::Att_shoot2(std::vector<shProjectile>& pvec, SDL_Point chPos) {
    double sx = hitbox_x + hitbox_w * 0.5;
    double sy = hitbox_y + hitbox_h * 0.5;
    clock_t curr_time = clock();
    if (((double)curr_time - (double)shootingClock) / CLOCKS_PER_SEC >= shooting2_interval) {
        shootingClock = clock();
        int bullet_count = 2;
        double bullet_speed = 4;
        double angle_gap = 80;
        double radius = 30;
        double shotDir = atan2(chPos.x - sx, chPos.y - sy) * 180 / PI;
        for (int i = 0; i < bullet_count; i++) {
            double theta = angle_gap * i - angle_gap / 2 + shotDir;
            theta = PI * theta / 180;
            shProjectile shp;
            shp.Init(sx + radius * sin(theta), sy + radius * cos(theta), shotDir * PI / 180, bullet_speed);
            shp.InitSprite();
            pvec.push_back(shp);
        }
        attack_click ++;
    }
}

void StormHead::Att_spawn(std::vector<Golem*>& gols, std::vector<Goblin*>& gobs) {
    if (attack_click != 1 && attack_click != 3) return; 
    double sx = hitbox_x + hitbox_w * 0.5;
    double sy = hitbox_y + hitbox_h * 0.5;
    int mob_num = 2;
    int offsetX = 120 * (attack_click == 1 ? 1 : -1) * (sprites[curr_sprite].flip == SDL_FLIP_NONE ? 1 : -1);
    int gol_num = rand() % (mob_num + 1);
    int radius = 40;
    for (int i = 0; i < gol_num; i++) {
        Golem* g = new Golem();
        g->InitHitbox(sx + offsetX + (rand() % (2 * radius + 1) - radius), 
                        sy + (rand() % (2 * radius + 1) - radius), CHARACTER_WIDTH, CHARACTER_HEIGHT);
        g->InitSprite();
        g->InitClock();
        gols.push_back(g);
    }
    for (int i = 0; i < mob_num - gol_num; i++) {
        Goblin* g = new Goblin();
        g->InitHitbox(sx + offsetX + (rand() % (2 * radius + 1) - radius),
                        sy + (rand() % (2 * radius + 1) - radius), CHARACTER_WIDTH, CHARACTER_HEIGHT);
        g->InitSprite();
        g->InitClock();
        gobs.push_back(g);
    }
    attack_click++;
}

int StormHead::GetHealth() {
    return health;
}

void StormHead::Collide(SDL_Rect rcheck) {
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
        hitbox_y = new_vhitbox.y - vsign;
    }

}

bool StormHead::CollideProjectile(Projectile& p) {
    SDL_Rect rcheck = p.GetHitbox();
    if (!attack_done) return 0;
    if (!CheckCollisionRectangle(rcheck, GetHitbox())) return 0;
    
    health --;
    got_hit = true;
    for (int i = 0; i < 6; i++) sprites[i].SetFrameId(0);
    return 1;

}



void StormHead::Render(SDL_Renderer* renderer, int camX, int camY) {
    double render_x = spr_offsetX + hitbox_x,
        render_y = spr_offsetY + hitbox_y;
    this->sprites[curr_sprite].Render(render_x - camX, render_y - camY, renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 32, 64, 255);
    SDL_Rect hitbox = {hitbox_x - camX, hitbox_y - camY, hitbox_w, hitbox_h};
    SDL_RenderDrawRect(renderer, &hitbox);

    int spacing = 10;
    int healthbar_height = 4;
    SDL_Rect rect_healthbar = {hitbox_x - camX, hitbox_y + hitbox_h - camY + spacing, hitbox_w, healthbar_height};
    SDL_Rect rect_currhealth= {hitbox_x - camX, hitbox_y + hitbox_h - camY + spacing, hitbox_w * health / MAX_HEALTH, healthbar_height};
    SDL_SetRenderDrawColor(renderer, 240, 64, 64, 255);
    SDL_RenderFillRect(renderer, &rect_healthbar);
    SDL_SetRenderDrawColor(renderer, 64, 240, 64, 255);
    SDL_RenderFillRect(renderer, &rect_currhealth);
}


StormHead::StormHead() {
    hvel = 0;
    vvel = 0;
    hacc = 0;
    vacc = 0;
    hitbox_x = 0;
    hitbox_y = 0;
    hitbox_w = 0;
    hitbox_h = 0;
    attacking = false;
    spawning = false;
    got_hit = false;
    health = MAX_HEALTH;
    chasingSpeed = 0.5;
    curr_attack = 0;
    attack_click = 0;

    next_attack_interval = 5;
    shooting_interval = 0.2;
    shooting2_interval = 0.15;
    
    shot = 0;
    shotTotal = 3;

    chaseDist2 = 200 * 200;
    completelyDied = 0;

    for (int i = 0; i < SPRITE_COUNT; i++) {
        sprites[i].texture = NULL;
    }

    spr_offsetX = 0;
    spr_offsetY = 0;
    spr_width = 0;
    spr_height = 0;

}

StormHead::~StormHead() {
    
    hvel = 0;
    vvel = 0;
    hacc = 0;
    vacc = 0;
    hitbox_x = 0;
    hitbox_y = 0;
    hitbox_w = 0;
    hitbox_h = 0;
    attacking = false;
    spawning = false;
    got_hit = false;
    health = 0;
    chasingSpeed = 0;
    curr_attack = 0;
    attack_click = 0;

    shot = 0;
    shotTotal = 0;

    chaseDist2 = 0;
    completelyDied = 0;


    for (int i = 0; i < SPRITE_COUNT; i++) {
        sprites[i].texture = NULL;
    }

    spr_offsetX = 0;
    spr_offsetY = 0;
    spr_width = 0;
    spr_height = 0;
}

StormHead::StormHead(const StormHead& e){
    hvel = e.hvel;
    vvel = e.vvel;
    hacc = e.hacc;
    vacc = e.vacc;
    hitbox_x = e.hitbox_x;
    hitbox_y = e.hitbox_y;
    hitbox_w = e.hitbox_w;
    hitbox_h = e.hitbox_h;
    attacking = e.attacking;
    spawning  = e.spawning ;
    got_hit   = e.got_hit  ;
    health = e.health;
    chasingSpeed = e.chasingSpeed;
    curr_attack = e.curr_attack;
    attack_click = e.attack_click;

    completelyDied = e.completelyDied;

    sprites[SPR_IDLE]    = e.sprites[SPR_IDLE]   ;
    sprites[SPR_RUN]     = e.sprites[SPR_RUN]    ;
    sprites[SPR_ATTACK]  = e.sprites[SPR_ATTACK] ;
    sprites[SPR_ATTACK2] = e.sprites[SPR_ATTACK2];
    sprites[SPR_DAMAGED] = e.sprites[SPR_DAMAGED];
    sprites[SPR_DEATH]   = e.sprites[SPR_DEATH]  ;

    spr_offsetX = e.spr_offsetX ;
    spr_offsetY = e.spr_offsetY ;
    spr_width   = e.spr_width   ;
    spr_height  = e.spr_height  ;
}

StormHead& StormHead::operator=(const StormHead& e) {
    hvel = e.hvel;
    vvel = e.vvel;
    hacc = e.hacc;
    vacc = e.vacc;
    hitbox_x = e.hitbox_x;
    hitbox_y = e.hitbox_y;
    hitbox_w = e.hitbox_w;
    hitbox_h = e.hitbox_h;
    attacking = e.attacking;
    spawning  = e.spawning ;
    got_hit   = e.got_hit  ;
    health = e.health;
    chasingSpeed = e.chasingSpeed;
    curr_attack = e.curr_attack;
    attack_click = e.attack_click;

    completelyDied = e.completelyDied;

    

    sprites[SPR_IDLE]    = e.sprites[SPR_IDLE]   ;
    sprites[SPR_RUN]     = e.sprites[SPR_RUN]    ;
    sprites[SPR_ATTACK]  = e.sprites[SPR_ATTACK] ;
    sprites[SPR_ATTACK2] = e.sprites[SPR_ATTACK2];
    sprites[SPR_DAMAGED] = e.sprites[SPR_DAMAGED];
    sprites[SPR_DEATH]   = e.sprites[SPR_DEATH]  ;

    spr_offsetX = e.spr_offsetX ;
    spr_offsetY = e.spr_offsetY ;
    spr_width   = e.spr_width   ;
    spr_height  = e.spr_height  ;

    return *this;
}