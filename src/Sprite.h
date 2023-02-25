#ifndef SPRITE_H_
#define SPRITE_H_

#include "LTexture.h"

class Sprite{
public:
    LTexture* texture;

    Sprite();
    ~Sprite();
    Sprite(const Sprite& s);
    Sprite& operator=(const Sprite& s);

    void InitClock();
    void SetClipTime(double clipTime);
    void AddClip(SDL_Rect clip);
    void ClearClip();
    int Animate();
    void SetFrameId(int frameId);
	void Render(double x, double y, SDL_Renderer* gRenderer);
    int GetClipSize();
    void DEBUG_PRINT();
    int GetFrameId();
    void SetLoop(int loop);


    double angle;
    SDL_Point center;
    SDL_RendererFlip flip;
    double clipTime;
    std::vector<SDL_Rect> clipVec;
    int resize = 1;
    clock_t mClock;

private:
    int frameId = 0;
    int loop = 1;


};

#endif