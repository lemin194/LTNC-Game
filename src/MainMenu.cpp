#include "MainMenu.h"

void MainMenu::Init() {
    int w = 144;
    int h = 32;
    int yOffset = 64;
    b_play   .Init(SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - yOffset, w, h);
    b_setting.Init(SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - yOffset + 2 * h, w, h);
    b_exit   .Init(SCREEN_WIDTH / 2 - w / 2, SCREEN_HEIGHT / 2 - yOffset + 4 * h, w, h);
}

void MainMenu::InitText(TTF_Font* gFont, SDL_Renderer* renderer) {
    b_play   .InitText("Play", gFont, renderer);
    b_setting.InitText("Setting", gFont, renderer);
    b_exit   .InitText("Exit", gFont, renderer);
}

int MainMenu::HandleMouseEvents(SDL_Event e) {
    if (b_play   .HandleMouseEvent(e)) return 1;
    b_setting.HandleMouseEvent(e);
    if (b_exit   .HandleMouseEvent(e)) return 3;
    
}

void MainMenu::Update() {

}

void MainMenu::Draw(SDL_Renderer* renderer) {
    b_play   .Render(renderer);
    b_setting.Render(renderer);
    b_exit   .Render(renderer);
}