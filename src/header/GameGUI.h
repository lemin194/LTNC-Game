#ifndef GAMEGUI_H_
#define GAMEGUI_H_

#include "GameBase.h"
#include "Button.h"

class GameGUI {
public:
    GameGUI(TTF_Font* font, vector<string> options);
    ~GameGUI();
    int HandleMouseEvent(SDL_Event e);
    void Render(SDL_Renderer* renderer);

private:
    vector<string> options;
    TTF_Font* font;
    const int w = 240, h = 32;

};

#endif