#include "Golem.h"

void Golem::InitSprite() {
    // std::cout << hitbox_h << "-" << hitbox_w << "hitbox\n";
    spr_width = 64; spr_height = 64;
    spr_offsetX = hitbox_w * 0.5 - 32;
    spr_offsetY = hitbox_h * 0.5 - 44;

    for (int i = 0; i <= 3; i++) {
        sprites[i].texture = Golem::texture;
        sprites[i].SetClipTime(0.08);
        sprites[i].center = {(int)(32 - hitbox_w / 2), (int)(44 - hitbox_h / 2)};
        sprites[i].flip = SDL_FLIP_NONE;
        sprites[i].InitClock();
    }

    

    for (int j = 0; j < 4; j ++) {
        
        for (int i = 0; i <= 1; i ++) {
            sprites[i].AddClip({(int)spr_width * j, (int)spr_height * i, (int)spr_width, (int)spr_height});
        }
        sprites[2].AddClip({(int) spr_width * j, (int) spr_height * 8, (int) spr_width, (int) spr_height});
        sprites[3].AddClip({(int) spr_width * j, (int) spr_height * 9, (int) spr_width, (int) spr_height});
    }

    sprites[0].SetLoop(1);
    sprites[1].SetLoop(1);
    sprites[2].SetLoop(1);
    sprites[3].SetLoop(0);


    sprites[3].SetClipTime(0.12);
}

void Golem::Animate() {
    if (health > 0) {
        if (got_hit) {
            curr_sprite = 2;
            if (sprites[curr_sprite].Animate()) got_hit = false;
        }
        else {
            if (!chasing && !dawdling) curr_sprite = 0;
            else  curr_sprite = 1;
            sprites[curr_sprite].Animate();
        }
    }
    else {
        if (curr_sprite != 3) {
            curr_sprite = 3;
            sprites[curr_sprite].SetFrameId(0);
            sprites[curr_sprite].InitClock();
        }
        else {
            clock_t curr_time = clock();
            if (sprites[curr_sprite].Animate() && ((double) curr_time - (double)dying_clock) / CLOCKS_PER_SEC >= dyingDuration) {
                completelyDied = true;
            }
        }
    }

    if (hvel > 0.01) {
        sprites[curr_sprite].flip = SDL_FLIP_NONE;
    }
    else if (hvel < -0.01) {
        sprites[curr_sprite].flip = SDL_FLIP_HORIZONTAL;
    }
}
void Golem::Render(SDL_Renderer* renderer, int camX, int camY) {

    this->sprites[curr_sprite].Animate();
    double render_x = spr_offsetX + hitbox_x,
        render_y = spr_offsetY + hitbox_y;
        // std::cout << hitbox_x << "\n";

    // std::cout << curr_sprite << "-" << (this->sprites[curr_sprite].center.x) << "-" << (this->sprites[curr_sprite].center.y) << "\n";
    this->sprites[curr_sprite].Render(render_x - camX, render_y - camY, renderer);


    // SDL_SetRenderDrawColor(renderer, 255, 32, 64, 255);
    // SDL_Rect hitbox = {hitbox_x - camX, hitbox_y - camY, hitbox_w, hitbox_h};
    // SDL_RenderDrawRect(renderer, &hitbox);


    int spacing = 5;
    int healthbar_height = 2;
    SDL_Rect rect_healthbar = {hitbox_x - camX, hitbox_y + hitbox_h - camY + spacing, hitbox_w, healthbar_height};
    SDL_Rect rect_currhealth= {hitbox_x - camX, hitbox_y + hitbox_h - camY + spacing, hitbox_w * health / MAX_HEALTH, healthbar_height};
    SDL_SetRenderDrawColor(renderer, 240, 64, 64, 255);
    SDL_RenderFillRect(renderer, &rect_healthbar);
    SDL_SetRenderDrawColor(renderer, 64, 240, 64, 255);
    SDL_RenderFillRect(renderer, &rect_currhealth);
}

Golem::Golem() {
    spr_offsetX = 0;
    spr_offsetY = 0;
    spr_width = 0;
    spr_height = 0;
    curr_sprite = 0;
    for (int i = 0; i < 5; i++) {
        sprites[i].texture = NULL;
        sprites[i].InitClock();
    }
    kb_coeficient = 0.5;
    chasingSpeed = 2.5;
    health = MAX_HEALTH;;

}
Golem::~Golem(){
    spr_offsetX = 0;
    spr_offsetY = 0;
    spr_width = 0;
    spr_height = 0;
    curr_sprite = 0;
    chasingSpeed = 0;
    for (int i = 0; i < 5; i++) {
        sprites[i].texture = NULL;
    }
    health = 0;
    Enemy::~Enemy();
}

Golem::Golem(const Golem& g) : Enemy(g) {
    spr_offsetX  = g.spr_offsetX ;
    spr_offsetY  = g.spr_offsetY ;
    spr_width    = g.spr_width   ;
    spr_height   = g.spr_height  ;
    curr_sprite  = g.curr_sprite ;
    chasingSpeed = g.chasingSpeed;
    for (int i = 0; i < 5; i++) {
        sprites[i] = g.sprites[i];
    }
}

Golem& Golem::operator= (const Golem& g) {
    Enemy::operator=(g);
    
    spr_offsetX  = g.spr_offsetX ;
    spr_offsetY  = g.spr_offsetY ;
    spr_width    = g.spr_width   ;
    spr_height   = g.spr_height  ;
    curr_sprite  = g.curr_sprite ;
    chasingSpeed = g.chasingSpeed;
    for (int i = 0; i < 5; i++) {
        sprites[i] = g.sprites[i];
    }

    return *this;
}