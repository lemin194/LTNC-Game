#ifndef BUTTON_H_
#define BUTTON_H_

#include "GameBase.h"
#include "LTexture.h"


class Button {
public:
    ~Button();
    void Init(int x, int y, int w, int h);
    void InitText(std::string txt, TTF_Font* gFont, SDL_Renderer* renderer);
    int HandleMouseEvent(SDL_Event e) ;
    void Render(SDL_Renderer* renderer);
    

private:
    int x, y, w, h;
    LTexture txt;
    int hover, clicked;
    TTF_Font* gFont;

};


#endif