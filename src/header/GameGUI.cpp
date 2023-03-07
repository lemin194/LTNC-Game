#include "GameGUI.h"

GameGUI::GameGUI(TTF_Font* font, vector<string> options) {
    this->font = font;
    for (auto s : options) {
        this->options.push_back(s);
    }
    yOffset = (options.size() - 1) * h;
}

GameGUI::~GameGUI() {
    
}


int GameGUI::HandleMouseEvent(SDL_Event e) {
    
    int posX = SCREEN_WIDTH / 2 - w / 2;
    int posY = SCREEN_HEIGHT / 2 - yOffset;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
    for (int i = 0; i < options.size(); i++) {
        
        int x = posX, y = posY + 2 * h * i;
        SDL_Rect m_rect = {x, y, w, h};
        if (CheckCollisionRectangle(m_rect, mouseRect)) {
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                return i + 1;
            }
        }

    }
    return 0;

}

void GameGUI::Render(SDL_Renderer* renderer) {
    int posX = SCREEN_WIDTH / 2 - w / 2;
    int posY = SCREEN_HEIGHT / 2 - yOffset;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
    for (int i = 0; i < options.size(); i++) {
        int x = posX, y = posY + 2 * h * i;
        SDL_Rect m_rect = {x, y, w, h};
        if (CheckCollisionRectangle(m_rect, mouseRect)) {
            SDL_SetRenderDrawColor(renderer, 196, 196, 196, 255);
        }
        else SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

        
        SDL_RenderFillRect(renderer, &m_rect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &m_rect);

        LTexture texttexture;
        texttexture.loadFromRenderedText(options[i], {255, 255, 255, 255}, font, renderer);
        texttexture.Render(x + w / 2 - texttexture.GetWidth() / 2, y + h / 2 - texttexture.GetHeight() / 2,
                    renderer, NULL, 0, NULL, SDL_FLIP_NONE, 1);
        texttexture.Free();
    }



}