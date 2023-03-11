#include "GameFloor.h"

int GameFloor::HandleMouseEvents(SDL_Event e) {
    
	
	if (e.type != SDL_MOUSEBUTTONDOWN) return 0;
	int leftClicked = 0, rightClicked = 0;

	leftClicked = e.button.button == SDL_BUTTON_LEFT;
	rightClicked = e.button.button == SDL_BUTTON_RIGHT;

	if (leftClicked) player->ShootProjectile(camera.x, camera.y, projectile_vec);
	if (rightClicked) player->SpecialAttack(camera.x, camera.y, projectile_vec);
	SDL_GetMouseState(&mouseX, &mouseY);
	// if (rightClicked) {
	// 	Golem* g = new Golem();
	// 	g->InitHitbox(mouseX + camera.x, mouseY + camera.y, CHARACTER_WIDTH, CHARACTER_HEIGHT);
	// 	g->InitSprite();
	// 	gols.push_back(g);
	// }
    return 0;
}
void GameFloor::GameInit(string path) {
    
    ifstream f(path);
    
	projectile_vec.clear();
	epvec.clear();
	gols.clear();
	gobs.clear();

	
	tileSet.Init(32, 32);

    string path_map;
    f >> path_map;
	tileSet.LoadMap(path_map);

	LEVEL_WIDTH = tileSet.GetLevelWidth();
	LEVEL_HEIGHT = tileSet.GetLevelHeight();
	
	camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    int playerX, playerY; f >> playerX >> playerY;
	player = new Character();
	player->InitHitbox(playerX - CHARACTER_WIDTH / 2, playerY - CHARACTER_HEIGHT / 2, CHARACTER_WIDTH, CHARACTER_HEIGHT);
	player->InitSprite();

    int stX, stY; f >> stX >> stY;
	stormhead = new StormHead();
	stormhead->InitHitbox(stX - STORM_WIDTH / 2, stY - STORM_HEIGHT / 2,
							STORM_WIDTH, STORM_HEIGHT);
	stormhead->InitSprite();
	stormhead->InitClock();


    int gobCount, golCount;
    f >> golCount;
    for (int i = 0; i < golCount; i++) {
        int X, Y; f >> X >> Y;
        Golem* g = new Golem();
        g->InitHitbox(X - CHARACTER_WIDTH / 2, Y - CHARACTER_HEIGHT / 2, CHARACTER_WIDTH, CHARACTER_HEIGHT);
        g->InitSprite();
        g->InitClock();
        gols.push_back(g);
    }
    f >> gobCount;
    for (int i = 0; i < gobCount; i++) {
        int X, Y; f >> X >> Y;
        Goblin* g = new Goblin();
        g->InitHitbox(X - CHARACTER_WIDTH / 2, Y - CHARACTER_HEIGHT / 2, CHARACTER_WIDTH, CHARACTER_HEIGHT);
        g->InitSprite();
        g->InitClock();
        gobs.push_back(g);
    }


	auto [px, py, pw, ph] = player->GetHitbox();
	double cam_cen_x = camera.x + SCREEN_WIDTH * 0.5;
	double cam_cen_y = camera.y + SCREEN_HEIGHT * 0.5;
	camera.x += (px + pw * 0.5 - cam_cen_x);
	camera.y += (py + ph * 0.5 - cam_cen_y);

    
    SDL_Color textColor = { 145, 125, 125 };

	heart = new LTexture();
	heart->LoadFromFile("..\\assets\\sprites\\heart.png", renderer);
}


void GameFloor::Update() {
    
    
	if (player->health > 0) {
		player->ReceiveMouseInput(camera.x, camera.y);
		const Uint8* state = SDL_GetKeyboardState(NULL);
		
		int hsign = state[SDL_SCANCODE_D] - state[SDL_SCANCODE_A];
		int vsign = state[SDL_SCANCODE_S] - state[SDL_SCANCODE_W];
		
		player->AddForce(hsign, vsign);
		if (hsign != 0 || vsign != 0) {
			player->AnimateMoving(hsign);
		}
		else player->AnimateIdle();

		player->Update();
	}

	auto [px, py, pw, ph] = player->GetHitbox();
	

	if (true) {
		auto it = gols.begin();
		while (it != gols.end()) {
			if ((*it)->completelyDied) {
				(*it)->~Golem();
				
				it = gols.erase(it);
			}
			else {
				it++;
			}
		}
	}

	if (true) {
		auto it = gobs.begin();
		while (it != gobs.end()) {
			if ((*it)->completelyDied) {
				(*it)->~Goblin();
				it = gobs.erase(it);
			}
			else {
				it++;
			}
		}
	}
	

	for (auto g : gols)
		g->ChaseCharacter(*player);

	for (auto g : gobs)
		g->ChaseCharacter(*player);

	SDL_Point chPos = {px + pw / 2, py + ph / 2};
	
	stormhead->Update(epvec, gols, gobs, chPos);


	for (int t = 0; t < 2; t++) {
		std::vector<Projectile> new_vec;
		for (int i = 0; i < projectile_vec.size(); i++) {
			projectile_vec[i].Update();
			
			

			int ok = 1;
			for (auto g : gols) if (g->GetHealth() > 0 && g->CollideProjectile(projectile_vec[i]))
			{ ok = 0; break; }
			for (auto g : gobs) if (g->GetHealth() > 0 && g->CollideProjectile(projectile_vec[i]))
			{ ok = 0; break; }
			if (stormhead->GetHealth() > 0 && stormhead->CollideProjectile(projectile_vec[i])) { ok = 0; }

			if (!ok) continue;

			if (!tileSet.CollideProjectile(projectile_vec[i])) {
				new_vec.push_back(projectile_vec[i]);
			}
		}
		projectile_vec.clear();
		for (auto p : new_vec) projectile_vec.push_back(p);

		std::vector<shProjectile> new_epvec;
		for (int i = 0; i < epvec.size(); i++) {
			epvec[i].Update();
			if (tileSet.CollideProjectile(epvec[i])) continue;
			if (player->CollideProjectile(epvec[i])) continue;
		
			new_epvec.push_back(epvec[i]);
			
		}
		epvec = new_epvec;
	}




	tileSet.CollideCharacter(*player);
	
	for (auto g : gols) { tileSet.CollideEnemy(*g); }
	for (auto g : gobs) { tileSet.CollideEnemy(*g); }
	


	player->PostUpdate();
	
	for (auto g : gols) g->PostUpdate();
	for (auto g : gobs) g->PostUpdate();

	

	for (auto g : gols) g->Animate();
	for (auto g : gobs) g->Animate();
	stormhead->Animate();


	player->AnimateWeapon();
}

void GameFloor::Draw() {
    


	tileSet.RenderMap(renderer, 0, 0, camera.x, camera.y);
	
	


	for (auto g : gols) {
		g->Render(renderer, camera.x, camera.y);
	}

	for (auto g : gobs) {
		g->Render(renderer, camera.x, camera.y);
	}
	for (auto p : projectile_vec) {
		p.Render(renderer, camera.x, camera.y);
	}
	for (auto p : epvec) {
		p.Render(renderer, camera.x, camera.y);
	}

	if (!stormhead->completelyDied) stormhead->Render(renderer, camera.x, camera.y);
	
	
	player->Render(renderer, camera.x, camera.y);


	SDL_Point ui_pos = {8, 8};
	SDL_Point curr_pos = ui_pos;
    LTexture txt_health, txt_mana;
    txt_health.loadFromRenderedText("Health:", {255, 255, 255, 255}, font_ui, renderer);
    txt_mana.loadFromRenderedText("Mana:", {255, 255, 255, 255}, font_ui, renderer);
    txt_health.Render(curr_pos.x, curr_pos.y, renderer, NULL, 0, NULL, SDL_FLIP_NONE, 1);
    // SDL_Rect r = {curr_pos.x, curr_pos.y, txt_health->GetWidth(), txt_health->GetHeight()};
    // SDL_RenderDrawRect(renderer, &r);
    SDL_Point heart_pos = {curr_pos.x + txt_health.GetWidth(), curr_pos.y + txt_health.GetHeight() / 2};
    int spacing = 0;
    int health = player->health;
    for (int i = 1; i <= health; i++) {
        int curr_x = heart_pos.x + (i - 1) * (spacing + heart->GetWidth());
        heart->Render(curr_x, heart_pos.y - heart->GetHeight() / 2, renderer, NULL, 0, NULL, SDL_FLIP_NONE, 1);
    }
	curr_pos.y += txt_health.GetHeight();
	SDL_Point mana_pos = {heart_pos.x, curr_pos.y + txt_mana.GetHeight() / 2};
    txt_mana.Render(curr_pos.x, curr_pos.y, renderer, NULL, 0, NULL, SDL_FLIP_NONE, 1);
	int manaBar_height = 24, manaBar_width = 144;
	SDL_Rect manaBar_bg_rect = {mana_pos.x, mana_pos.y - manaBar_height / 2, manaBar_width, manaBar_height};
	SDL_Rect manaBar_rect = {mana_pos.x, mana_pos.y - manaBar_height / 2, manaBar_width * player->mana / player->MAX_MANA, manaBar_height};
	SDL_SetRenderDrawColor(renderer, 0, 32, 128, 255);
	SDL_RenderFillRect(renderer, &manaBar_bg_rect);
	SDL_SetRenderDrawColor(renderer, 32, 128, 255, 255);
	SDL_RenderFillRect(renderer, &manaBar_rect);

    txt_health.Free();
	txt_mana.Free();
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