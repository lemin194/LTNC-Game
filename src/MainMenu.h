#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "GameFloor.h"
#include "Button.h"
class MainMenu {

public:
    
    int HandleMouseEvents(SDL_Event e);
    void Init();
    void InitText(TTF_Font* gFont, SDL_Renderer* renderer);
    void Update();
    void Draw(SDL_Renderer* renderer);

private :
    Button b_play, b_setting, b_exit;
    

};

#endif