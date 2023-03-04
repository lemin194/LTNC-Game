#include "GameFloor.h"

int GameFloor::HandleMouseEvents(SDL_Event e) {
    
}
void GameFloor::GameInit() {
	heart = new LTexture();
	heart->LoadFromFile("..\\assets\\sprites\\heart.png", renderer);
}


void GameFloor::Update() {}

void GameFloor::Draw() {


	SDL_Point txt_healthBar_pos = {8, 8};
    LTexture txt_healthBar;
    txt_healthBar.loadFromRenderedText("Health:", {255, 255, 255, 255}, font_ui, renderer);
    txt_healthBar.Render(txt_healthBar_pos.x, txt_healthBar_pos.y, renderer, NULL, 0, NULL, SDL_FLIP_NONE, 1);
    // SDL_Rect r = {txt_healthBar_pos.x, txt_healthBar_pos.y, txt_healthBar->GetWidth(), txt_healthBar->GetHeight()};
    // SDL_RenderDrawRect(renderer, &r);
    SDL_Point heart_pos = {txt_healthBar_pos.x + txt_healthBar.GetWidth(), txt_healthBar_pos.y + txt_healthBar.GetHeight() / 2};
    int spacing = 0;
    int health = player->health;
    for (int i = 1; i <= health; i++) {
        int curr_x = heart_pos.x + (i - 1) * (spacing + heart->GetWidth());
        heart->Render(curr_x, heart_pos.y - heart->GetHeight() / 2, renderer, NULL, 0, NULL, SDL_FLIP_NONE, 1);
    }

    txt_healthBar.Free();

}


GameFloor::GameFloor() {}


GameFloor::~GameFloor() {}

int GameFloor::GetHealth() {
    return player->health;
}

bool GameFloor::IsWin() {
    int sum = !stormhead->completelyDied;
    sum += gobs.size();
    sum += gols.size();
    return sum == 0;
}

bool GameFloor::IsLose() {
    return player->health <= 0;
}