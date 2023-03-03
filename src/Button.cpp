#include "Button.h"

void Button::Init(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

int Button::HandleMouseEvent(SDL_Event e) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    int leftClicked = e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT;
    SDL_Rect mouseR = {mouseX, mouseY, 1, 1};
    SDL_Rect m_rect = {x, y, w, h};
    if (!CheckCollisionRectangle(mouseR, m_rect)) {
        hover = false;
        clicked = false;
        return false;
    }
    hover = true;
    if (leftClicked) {clicked = true; return true;}
    else clicked = false;
    return false;
}

void Button::Render(SDL_Renderer* renderer) {
    if (clicked) {
        SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
    }
    else if (hover) {
        SDL_SetRenderDrawColor(renderer, 196, 196, 196, 255);
    }
    else SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

    
    SDL_Rect m_rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &m_rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &m_rect);

    txt.Render(x + w / 2 - txt.GetWidth() / 2, y + h / 2 - txt.GetHeight() / 2,
                renderer, NULL, 0, NULL, SDL_FLIP_NONE, 1);

}

Button::~Button() {
    txt.Free();
    gFont = NULL;
}

void Button::InitText(std::string txt, TTF_Font* gFont, SDL_Renderer* renderer) {
    this->gFont = gFont;
    this->txt.loadFromRenderedText(txt, {255, 255, 255, 255}, gFont, renderer);
}