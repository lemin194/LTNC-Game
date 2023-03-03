#include "Floor1.h"


void Floor1::GameInit() {
    
	projectile_vec.clear();
	epvec.clear();
	gols.clear();
	gobs.clear();

	
	tileSet.Init(32, 32);

	tileSet.LoadMap("..\\assets\\maps\\map1.txt");
	tileSet.LoadMapCollision("..\\assets\\maps\\map1_collision.txt");

	LEVEL_WIDTH = tileSet.GetLevelWidth();
	LEVEL_HEIGHT = tileSet.GetLevelHeight();
	
	camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	player = new Character();
	player->InitHitbox(128, 128, CHARACTER_WIDTH, CHARACTER_HEIGHT);
	player->InitSprite();

	stormhead = new StormHead();
	stormhead->InitHitbox(LEVEL_WIDTH / 2 - STORM_WIDTH / 2, LEVEL_HEIGHT / 2 - STORM_HEIGHT / 2,
							STORM_WIDTH, STORM_HEIGHT);
	stormhead->InitSprite();
	stormhead->InitClock();


	auto [px, py, pw, ph] = player->GetHitbox();
	double cam_cen_x = camera.x + SCREEN_WIDTH * 0.5;
	double cam_cen_y = camera.y + SCREEN_HEIGHT * 0.5;
	camera.x += (px + pw * 0.5 - cam_cen_x);
	camera.y += (py + ph * 0.5 - cam_cen_y);

    
    SDL_Color textColor = { 145, 125, 125 };
    gTextTexture1.loadFromRenderedText( "WASD to move", textColor, gFont, renderer );

}

int Floor1::HandleMouseEvents(SDL_Event e){
	
	if (e.type != SDL_MOUSEBUTTONDOWN) return 0;
	int leftClicked = 0, rightClicked = 0;

	leftClicked = e.button.button == SDL_BUTTON_LEFT;
	rightClicked = e.button.button == SDL_BUTTON_RIGHT;

	if (leftClicked) player->ShootProjectile(camera.x, camera.y, projectile_vec);
	SDL_GetMouseState(&mouseX, &mouseY);
	// if (rightClicked) {
	// 	Golem* g = new Golem();
	// 	g->InitHitbox(mouseX + camera.x, mouseY + camera.y, CHARACTER_WIDTH, CHARACTER_HEIGHT);
	// 	g->InitSprite();
	// 	gols.push_back(g);
	// }
    return 0;
}

void Floor1::Update() {
    
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
	if (stormhead->GetHealth() > 0)
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

void Floor1::Draw() {


	tileSet.RenderMap(renderer, 0, 0, camera.x, camera.y);
	
	gTextTexture1.Render(LEVEL_WIDTH / 2 - gTextTexture1.GetWidth() / 2 - camera.x, 256 - camera.y, renderer, NULL, 0, NULL, SDL_FLIP_NONE);
	


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

}