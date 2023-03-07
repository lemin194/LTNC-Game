
#include "Character.h"

void Character::InitHitbox(int x, int y, int w, int h) {
    hitbox_x = x;
    hitbox_y = y;
    hitbox_w = w;
    hitbox_h = h;

    firing_clock = clock();
    firingRate = .12;

}

SDL_Rect Character::GetHitbox() {
    return {hitbox_x, hitbox_y, hitbox_w, hitbox_h};
}

void Character::InitSprite() {
    sprites[0].texture = textures[0];
    sprites[1].texture = textures[1];


    curr_sprite = 0;

    spr_width = 32;
    spr_height = 56;
    spr_offsetX = (hitbox_w - spr_width ) * 0.5;
    spr_offsetY = (hitbox_h - spr_height) * 0.5 - 16;


    for (int i = 0; i <= 0; i++) {
        sprites[0].AddClip({0 + 32 * i, 8, 32, 56});
    }
    for (int i = 0; i <= 1; i++) {
        sprites[1].AddClip({0 + 32 * i, 8, 32, 56});
    }
    sprites[1].SetClipTime(0.16);

    for (int i = 0; i < 2; i++) {
        sprites[i].angle = 0;
        sprites[i].center = {-(int) spr_offsetX, -(int) spr_offsetY };
        sprites[i].flip = SDL_FLIP_NONE;
    }

    sprites[curr_sprite].SetFrameId(0);

    
    weapon.InitSprite();
    weapon.UpdatePos(hitbox_x + hitbox_w * 0.5, hitbox_y + hitbox_h * 0.5);

}

void Character::AddForce(double hacc, double vacc) {
    this->hvel += hacc;
    this->vvel += vacc;
}

void Character::Update() {
    this->hvel = FRICTION * (this->hacc + this->hvel);
    this->vvel = FRICTION * (this->vacc + this->vvel);
    if (this->hvel >  MAX_SPEED) this->hvel =  MAX_SPEED;
    if (this->hvel < -MAX_SPEED) this->hvel = -MAX_SPEED;
    if (this->vvel >  MAX_SPEED) this->vvel =  MAX_SPEED;
    if (this->vvel < -MAX_SPEED) this->vvel = -MAX_SPEED;

    if (got_hit) {
        clock_t curr_time = clock();
        if (((double) curr_time - (double)gothit_clock) / CLOCKS_PER_SEC >= gothit_duration) {
            got_hit = false;
            gothit_clock = clock();
        }
    }
}

void Character::Collide(SDL_Rect rcheck) {
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

void Character::PostUpdate() {
    

    hitbox_x += this->hvel;
    hitbox_y += this->vvel;
    weapon.UpdatePos(hitbox_x + hitbox_w * 0.5, hitbox_y + hitbox_h * 0.5);
}

void Character::ReceiveMouseInput(int &camX, int &camY) {

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

	double cam_cen_x = camX + SCREEN_WIDTH * 0.5;
	double cam_cen_y = camY + SCREEN_HEIGHT * 0.5;
    int px = hitbox_x + hitbox_w * 0.5;
    int py = hitbox_y + hitbox_h * 0.5;
    double tarX = (2 * px + mouseX) / 3;
    double tarY = (2 * py + mouseY) / 3;
	camX += (tarX - cam_cen_x) * 0.1;
	camY += (tarY - cam_cen_y) * 0.1;

    double deltax = mouseX + camX - px;
    double deltay = mouseY + camY - py;
    double rad = std::atan2(deltax, -deltay); 
    
    
    for (int i = 0; i <= 1; i ++) 
    {
        if (mouseX + camX < px) sprites[i].flip = SDL_FLIP_HORIZONTAL;
        if (mouseX + camX > px) sprites[i].flip = SDL_FLIP_NONE;
    }


    double amp = 120;
    if (rad < 0) {
        weapon.Flip(SDL_FLIP_HORIZONTAL);
        double new_rad = (rad + PI / 2) * amp / 180;
        weapon.UpdateAngle(new_rad * 180 / PI);
        
    }
    else {
        weapon.Flip(SDL_FLIP_NONE);
        double new_rad = (rad - PI / 2) * amp / 180;
        weapon.UpdateAngle(new_rad * 180 / PI);
    }



}

void Character::ShootProjectile(int &camX, int &camY, std::vector<Projectile> & pvec) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

	double cam_cen_x = camX + SCREEN_WIDTH * 0.5;
	double cam_cen_y = camY + SCREEN_HEIGHT * 0.5;
    int px = hitbox_x + hitbox_w * 0.5;
    int py = hitbox_y + hitbox_h * 0.5;
    double tarX = (2 * px + mouseX) / 3;
    double tarY = (2 * py + mouseY) / 3;
	camX += (tarX - cam_cen_x) * 0.1;
	camY += (tarY - cam_cen_y) * 0.1;

    double deltax = mouseX + camX - px;
    double deltay = mouseY + camY - py;
    double rad = std::atan2(deltax, -deltay); 

    double currClock = clock();
    if (((double)currClock - (double)firing_clock) / (double)CLOCKS_PER_SEC >= firingRate) {
        
        weapon.wielding = 1;
        weapon.sprite.InitClock();
        weapon.sprite.SetClipTime(0.05);
        weapon.sprite.SetFrameId(1);
        firing_clock = clock();

        Projectile p;
        p.InitSprite();
        p.Init(px, py, PI - rad, 6);
        for (int i = 0; i < 2; i++) p.Update();
        pvec.push_back(p);
    }

    Mix_PlayChannel(-1, sound_shoot, 0);
}

bool Character::CollideProjectile(shProjectile& p) {
    if (got_hit) return 0;
    SDL_Rect rcheck = p.GetHitbox();
    

    double px = rcheck.x + rcheck.w * 0.5, py = rcheck.y + rcheck.h * 0.5;
    double mx = hitbox_x + hitbox_w * 0.5, my = hitbox_y + hitbox_h * 0.5;
    double mr = std::min(hitbox_w, hitbox_h) / 2;
    double dist2 = (px - mx) * (px - mx) + (py - my) * (py - my);
    int collide = dist2 < (mr + rcheck.w / 2) * (mr + rcheck.w / 2);
    if (!collide) return 0;


    double deltax = mx - px, deltay = my - py;
    auto [pspeed, pdir] = p.GetSpeed();
    double pvelx = sin(pdir);
    double pvely = cos(pdir);

    // std::cout << deltax * 0.5 << "-" << pvelx << "\n";
    double theta = std::atan2(-deltay, deltax);
    double mult = 9;
    AddForce(kb_coefficient * pvelx * mult, kb_coefficient * pvely * mult);
    
    health --;
    got_hit = true;
    gothit_clock = clock();
    Mix_PlayChannel(-1, sound_gothit, 0);
    return 1;
    
}

bool Character::TakeDamage(SDL_Point ePos, int damage) {
    if (got_hit) {
        clock_t curr_time = clock();
        if (((double) curr_time - (double)gothit_clock) / CLOCKS_PER_SEC >= gothit_duration) {
            got_hit = false;
        }
        else return 0;
    }

    double px = ePos.x, py = ePos.y;
    double mx = hitbox_x + hitbox_w * 0.5, my = hitbox_y + hitbox_h * 0.5;

    double deltax = mx - px, deltay = my - py;
    double mult = 9;
    double theta = std::atan2(-deltay, deltax);
    AddForce(kb_coefficient * std::cos(theta) * mult, - kb_coefficient * std::sin(theta) * mult);
    

    health --;
    got_hit = true;
    gothit_clock = clock();
    Mix_PlayChannel(-1, sound_gothit, 0);
    return 1;
}


void Character::AnimateWeapon() {
    if (this->weapon.sprite.Animate())
        this->weapon.wielding = 0;
    if (!weapon.wielding) {
        this->weapon.sprite.SetFrameId(0);
        this->weapon.sprite.SetClipTime(0);
    }
}

void Character::AnimateMoving(int hsign) {
    if (curr_sprite != 1) {
        curr_sprite = 1;
        sprites[curr_sprite].SetFrameId(0);
    }
}

void Character::AnimateIdle() {
    if (curr_sprite == 0) return;
    curr_sprite = 0;
    sprites[curr_sprite].SetFrameId(0);
}

void Character::Render(SDL_Renderer* gRenderer, int camX, int camY) {

    SDL_SetRenderDrawColor(gRenderer, 255, 32, 64, 255);
    SDL_Rect hitbox = {hitbox_x - camX, hitbox_y - camY, hitbox_w, hitbox_h};
    SDL_RenderDrawRect(gRenderer, &hitbox);
    // std::cout << hitbox_x << "-" << hitbox_y << "-" << hitbox_h << "-" << spr_height << "\n";

    this->sprites[curr_sprite].Animate();
    double render_x = spr_offsetX + hitbox_x,
        render_y = spr_offsetY + hitbox_y;

    this->sprites[curr_sprite].Render(render_x - camX, render_y - camY, gRenderer);
    
    this->weapon.Render(gRenderer, camX, camY);
}

Character::Character() {
    
    for (int i = 0; i < 3; i++) {
        sprites[i].texture = NULL;
    }
    health = 5;
    weapon.sprite.texture = NULL;
    hvel = 0;
    vvel = 0;
    hacc = 0;
    vacc = 0;
    hitbox_x = 0;
    hitbox_y = 0;
    hitbox_w = 0;
    hitbox_h = 0;


    spr_offsetX = 0;
    spr_offsetY = 0;
    spr_width = 0;
    spr_height = 0;
    
    curr_sprite = 0;

    firing_clock = clock();
    firingRate = 0;


    got_hit = 1;
    gothit_clock = clock();

    kb_coefficient = 0.7;
    gothit_duration = 0.4;
}

Character::~Character() {
    
    for (int i = 0; i < 3; i++) {
        sprites[i].texture = NULL;
    }
    health = 0;
    weapon.sprite.texture = NULL;
    hvel = 0;
    vvel = 0;
    hacc = 0;
    vacc = 0;
    hitbox_x = 0;
    hitbox_y = 0;
    hitbox_w = 0;
    hitbox_h = 0;


    spr_offsetX = 0;
    spr_offsetY = 0;
    spr_width = 0;
    spr_height = 0;
    
    curr_sprite = 0;

    firing_clock = NULL;
    firingRate = 0;


    got_hit = 0;
    gothit_clock = NULL;

    kb_coefficient = 0;
    gothit_duration = 0;
}