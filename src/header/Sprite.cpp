#include "Sprite.h"


void Sprite::InitClock() {

    mClock = clock();
}
void Sprite::AddClip(SDL_Rect clip) {
    clipVec.push_back(clip);
    // std::cout << ((clipVec.back()).w * 32 + clipVec.back().h) << "k\n";
}
void Sprite::ClearClip() {
    clipVec.clear();
}

void Sprite::SetClipTime(double clipTime) {
    this->clipTime = clipTime;
}

void Sprite::SetFrameId(int frameId) {
    if (!clipVec.size()) this->frameId = frameId;
    else this->frameId = frameId % clipVec.size();
}

void Sprite::SetLoop(int loop) {
    this->loop = loop;
}


int Sprite::Animate() {
    if (clipTime == 0) return 0;
    clock_t curr_time = clock();
    double secondPassed = ((double)curr_time - (double)mClock) / (double)CLOCKS_PER_SEC; 
    if (secondPassed < clipTime) return 0;
    int framesPassed = round(secondPassed / clipTime);

    int ret = 0;
    if (framesPassed > 0 && frameId + framesPassed >= clipVec.size()) ret = 1;
    if (loop) {
        frameId = (frameId + framesPassed) % clipVec.size();
    }
    else {
        frameId = frameId + framesPassed;
        if (ret) frameId = clipVec.size() - 1;
    }
    mClock = curr_time;

    // if (ret) std::cout << ret << "\n";
    return ret;
    
}

int Sprite::GetFrameId() {
    return frameId;
}

void Sprite::Render(double x, double y, SDL_Renderer* gRenderer) {
    // if (angle != 0) std::cout << angle << "\n";
    // std::cout << clipVec[frameId].x << " " << clipVec[frameId].y << " " << clipVec[frameId].w << " " << clipVec[frameId].h << "clipVec\n";
    // SDL_SetRenderDrawColor(gRenderer, 32, 255, 128, 255);
    // SDL_Rect r = clipVec[frameId];
    // r.x = x; r.y = y;
    // SDL_RenderDrawRect(gRenderer, &r);
    texture->Render((int)x, (int)y, gRenderer, &(clipVec[frameId]), this->angle, &(this->center), this->flip, resize);
}

int Sprite::GetClipSize() {
    return clipVec.size();
}

void Sprite::DEBUG_PRINT() {
    std::cout << clipVec[129].x << "-" << clipVec[129].y << "-"
    << clipVec[129].h << "-" << clipVec[129].w << "sprite\n";
}

Sprite::Sprite() {
    
    angle = 0;
    center = {0, 0};
    flip = SDL_FLIP_NONE;
    texture = NULL;
    clipTime = 0;
    clipVec.clear();
    
    frameId = 0;
    loop = 1;
    resize   = 1;
}

Sprite::~Sprite() {
    
    angle = 0;
    center = {0, 0};
    flip = SDL_FLIP_NONE;
    texture = NULL;
    clipTime = 0;
    clipVec.clear();
    
    frameId = 0;
    loop = 1;
    resize   = 1;
}

Sprite::Sprite(const Sprite& s) {
    this->angle    = s.angle   ;
    this->center   = s.center  ;
    this->flip     = s.flip    ;
    this->texture  = s.texture ;
    this->clipTime = s.clipTime;
    this->clipVec  = s.clipVec ;
    this->frameId  = s.frameId ;
    this->loop     = s.loop    ;
    this->resize   = s.resize  ;
}

Sprite& Sprite::operator=(const Sprite& s) {
    
    this->angle    = s.angle   ;
    this->center   = s.center  ;
    this->flip     = s.flip    ;
    this->texture  = s.texture ;
    this->clipTime = s.clipTime;
    this->clipVec  = s.clipVec ;
    this->frameId  = s.frameId ;
    this->loop     = s.loop    ;
    this->resize   = s.resize  ;
    return *this;
}