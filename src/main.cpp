
#include "header/GameBase.h"
#include "header/LTexture.h"
#include "header/Character.h"
#include "header/Sprite.h"
#include "header/TileSet.h"
#include "header/Projectile.h"
#include "header/Enemy.h"
#include "header/Golem.h"
#include "header/Goblin.h"
#include "header/StormHead.h"
#include "header/GameFloor.h"
#include "header/shProjectile.h"
#include "header/Fireball.h"
#include "header/LTimer.h"
#include "header/GameGUI.h"




Character c;
TileSet tileSet;


SDL_Rect camera;
std::vector<Projectile> projector_vec;
std::vector<Golem*> gols;
std::vector<Goblin*> gobs;


SDL_Surface* screenSurface = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

LTexture* Weapon::texture;
LTexture* Character::textures[3];
LTexture* TileSet::texture;
LTexture* Projectile::texture;
LTexture* Golem::texture;
LTexture* Goblin::texture;
LTexture* StormHead::texture_idle;
LTexture* StormHead::texture_run;
LTexture* StormHead::texture_attack;
LTexture* StormHead::texture_attack2;
LTexture* StormHead::texture_damaged;
LTexture* StormHead::texture_death;
LTexture* shProjectile::texture;
LTexture* Fireball::texture;

LTexture* heart;

TTF_Font* GameFloor::gFont;
TTF_Font* GameFloor::font_ui;
TTF_Font* font_ui;

Mix_Chunk* sound_lose = NULL;
Mix_Chunk* sound_win = NULL;
Mix_Chunk* Character::sound_shoot = NULL;
Mix_Chunk* Character::sound_gothit = NULL;
Mix_Chunk* StormHead::sound_shoot = NULL;
Mix_Chunk* StormHead::sound_spawn = NULL;
Mix_Chunk* StormHead::sound_gothit = NULL;
Mix_Chunk* StormHead::sound_dies = NULL;
Mix_Chunk* Enemy::sound_gothit = NULL;




SDL_Renderer* GameFloor::renderer;
LTimer capTimer;

GameFloor* curr_floor;
GameGUI* menu;
GameGUI* winning;
GameGUI* losing;
GameGUI* floor_choosing;



int loadMedia() {
	

	TileSet::texture = new LTexture();
	TileSet::texture->LoadFromFile("../assets/sprites/tileSet_x2.png", renderer);


	std::string *paths = new std::string[2] {
						"../assets/sprites/char/char.png",
						"../assets/sprites/char/char.png"
						};

	for (int i = 0; i < 2; i ++) Character::textures[i] = new LTexture();
	Character::textures[0]->LoadFromFile("../assets/sprites/char/char.png", renderer);
	Character::textures[1]->LoadFromFile("../assets/sprites/char/char.png", renderer);

	Weapon::texture = new LTexture();
	Weapon::texture->LoadFromFile("../assets/sprites/char/weapon-sheet.png", renderer);

	Golem::texture = new LTexture();
	Golem::texture->LoadFromFile("../assets/sprites/enemy/golem_.png", renderer);

	Projectile::texture = new LTexture();
	Projectile::texture->LoadFromFile("../assets/sprites/projectile.png", renderer);

	shProjectile::texture = new LTexture();
	shProjectile::texture->LoadFromFile("../assets/sprites/enemy/stormhead/bullet.png", renderer);

	Fireball::texture = new LTexture();
	Fireball::texture->LoadFromFile("../assets/sprites/char/fb-sheet.png", renderer);

	Goblin::texture = new LTexture();
	Goblin::texture->LoadFromFile("../assets/sprites/enemy/goblin_.png", renderer);

	StormHead::texture_idle = new LTexture();
	StormHead::texture_idle->LoadFromFile("../assets/sprites/enemy/stormhead/sh-idle.png", renderer);

	StormHead::texture_run = new LTexture();
	StormHead::texture_run->LoadFromFile("../assets/sprites/enemy/stormhead/sh-run.png", renderer);

	StormHead::texture_attack = new LTexture();
	StormHead::texture_attack->LoadFromFile("../assets/sprites/enemy/stormhead/sh-attack.png", renderer);

	StormHead::texture_attack2 = new LTexture();
	StormHead::texture_attack2->LoadFromFile("../assets/sprites/enemy/stormhead/sh-attack2.png", renderer);

	StormHead::texture_damaged = new LTexture();
	StormHead::texture_damaged->LoadFromFile("../assets/sprites/enemy/stormhead/sh-damaged.png", renderer);

	StormHead::texture_death = new LTexture();
	StormHead::texture_death->LoadFromFile("../assets/sprites/enemy/stormhead/sh-death.png", renderer);

	StormHead::texture_death = new LTexture();
	StormHead::texture_death->LoadFromFile("../assets/sprites/enemy/stormhead/sh-death.png", renderer);

	heart = new LTexture();
	heart->LoadFromFile("../assets/sprites/heart.png", renderer);


	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
	}

	//Initialize SDL_ttf
	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	GameFloor::gFont = TTF_OpenFont( "../assets/font/ARCADECLASSIC.TTF", 64 );
    if( GameFloor::gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    }
	font_ui = TTF_OpenFont( "../assets/font/font_ui.TTF", 48 );
    if( font_ui == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    }
	GameFloor::font_ui = font_ui;

	
	//Initialize SDL_mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	}

	sound_lose = Mix_LoadWAV("../assets/sound/sound_lose.wav" );
	if (sound_lose == NULL ){
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	sound_win = Mix_LoadWAV("../assets/sound/sound_win.wav" );
	Character::sound_shoot = Mix_LoadWAV("../assets/sound/sound_player_shoot.wav");
	Character::sound_gothit = Mix_LoadWAV("../assets/sound/sound_player_gothit.wav");
	StormHead::sound_shoot = Mix_LoadWAV("../assets/sound/sound_stormhead_shoot.wav");
	StormHead::sound_spawn = Mix_LoadWAV("../assets/sound/sound_stormhead_spawn.wav");
	StormHead::sound_gothit = Mix_LoadWAV("../assets/sound/sound_enemy_gothit.wav");
	StormHead::sound_dies = Mix_LoadWAV("../assets/sound/sound_stormhead_dies.wav");
	Enemy::sound_gothit = Mix_LoadWAV("../assets/sound/sound_enemy_gothit.wav");
	return 1;
}






int mySdlInit() {
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return 0;
	}
	window = SDL_CreateWindow( WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_UNDEFINED,
							 SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if( window == NULL ) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return 0;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
		return 0;
	}

	SDL_SetRenderDrawColor(renderer, 0x2A, 0x2A, 0x2A, 0xFF);

	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		return 0;
	}

	screenSurface = SDL_GetWindowSurface( window );

	return 1;
}



void close() {
	TileSet::texture->Free();
	Character::textures[0]->Free();
	Character::textures[1]->Free();
	Weapon::texture->Free();
	Projectile::texture->Free();
	Golem::texture->Free();
	Goblin::texture->Free();
	Fireball::texture->Free();
	StormHead::texture_attack2->Free();
	StormHead::texture_attack->Free();
	StormHead::texture_damaged->Free();
	StormHead::texture_death->Free();
	StormHead::texture_idle->Free();
	StormHead::texture_run->Free();


	TileSet::texture = nullptr;
	Character::textures[0] = nullptr;
	Character::textures[1] = nullptr;
	Weapon::texture = nullptr;
	Projectile::texture = nullptr;
	Golem::texture = nullptr;
	Goblin::texture = nullptr;
	Fireball::texture = nullptr;
	StormHead::texture_attack2 = nullptr;
	StormHead::texture_attack = nullptr;
	StormHead::texture_damaged = nullptr;
	StormHead::texture_death = nullptr;
	StormHead::texture_idle = nullptr;
	StormHead::texture_run = nullptr;

	
	delete menu;
	delete winning;
	delete losing;
	delete curr_floor;
	


    TTF_CloseFont( GameFloor::gFont );


	Mix_FreeChunk(sound_lose);
	sound_lose = nullptr;
	Mix_FreeChunk(sound_win);
	sound_win = nullptr;
	Mix_FreeChunk(Character::sound_shoot);
	Character::sound_shoot = nullptr;
	Mix_FreeChunk(Character::sound_gothit);
	Character::sound_gothit = nullptr;
	Mix_FreeChunk(StormHead::sound_shoot);
	StormHead::sound_shoot = nullptr;
	Mix_FreeChunk(StormHead::sound_spawn);
	StormHead::sound_spawn = nullptr;
	Mix_FreeChunk(StormHead::sound_gothit);
	StormHead::sound_gothit = nullptr;
	Mix_FreeChunk(StormHead::sound_dies);
	StormHead::sound_dies = nullptr;
	Mix_FreeChunk(Enemy::sound_gothit);
	Enemy::sound_gothit = nullptr;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
	SDL_Quit();

	std::cout << "closed.";
}


int main( int argc, char* args[] )
{
	srand(time(NULL));


	int ok = mySdlInit();

	SDL_Surface* icon = myLoadSurface("../assets/icon.png", screenSurface->format, renderer);
	SDL_SetWindowIcon(window, icon);


	loadMedia();

	GameFloor::renderer = renderer;



	vector<string> menu_options;
	menu_options.push_back("Play");
	menu_options.push_back("Exit");
	menu = new GameGUI(font_ui, menu_options);

	vector<string> winning_options;
	winning_options.push_back("You Win!");
	winning_options.push_back("Restart");
	winning_options.push_back("Main Menu");
	winning = new GameGUI(font_ui, winning_options);

	vector<string> losing_options;
	losing_options.push_back("You Lose!");
	losing_options.push_back("Restart");
	losing_options.push_back("Main Menu");
	losing = new GameGUI(font_ui, losing_options);
	
	vector<string> path_floors;
	path_floors.push_back("../assets/maps/floor1.txt");
	path_floors.push_back("../assets/maps/floor2.txt");
	vector<string> floor_names;
	floor_names.push_back("Floor 1");
	floor_names.push_back("Floor 2");
	int floor_id = 0;
	floor_choosing = new GameGUI(font_ui, floor_names);

	auto restart = [&] () {
		curr_floor = new GameFloor();
		curr_floor->GameInit(path_floors[floor_id]);
		curr_floor->player->health = 5;
		
	};

	int pause = 0;
	int quit = 0;
	enum GameState {MainMenu, FloorChoosing, Playing, Win, Lose};
	int curr_gamestate = GameState::MainMenu;


	while( true )
	{
		capTimer.start();
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
				break;
			}
			if (e.key.keysym.sym == SDLK_ESCAPE) {
				curr_gamestate = GameState::MainMenu;
			}
			if (curr_gamestate == GameState::MainMenu) {
				int btn_click = menu->HandleMouseEvent(e);
				if (btn_click == 2) {
					quit = true;
					break;
				}
				if (btn_click == 1) {
					curr_gamestate = GameState::FloorChoosing;
				}
			}
			else if (curr_gamestate == GameState::FloorChoosing) {
				int btn_click = floor_choosing->HandleMouseEvent(e);
				if (btn_click > 0) {
					floor_id = btn_click - 1;
					curr_gamestate = GameState::Playing;
					restart();
				}
			}
			else if (curr_gamestate == GameState::Playing) {
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_r) {
						restart();
					}
				}

				curr_floor->HandleMouseEvents(e);
			}
			else if (curr_gamestate == GameState::Win) {
				int btn_click = winning->HandleMouseEvent(e);
				if (btn_click == 2) {
					curr_gamestate = GameState::Playing;
					restart();
				}
				if (btn_click == 3) {
					curr_gamestate = GameState::MainMenu;
				}
			}
			else if (curr_gamestate == GameState::Lose) {
				int btn_click = losing->HandleMouseEvent(e);
				if (btn_click == 2) {
					curr_gamestate = GameState::Playing;
					restart();
				}
				if (btn_click == 3) {
					curr_gamestate = GameState::MainMenu;
				}
			}
		}
		if (quit) break;



		SDL_SetRenderDrawColor(renderer, 0x2A, 0x2A, 0x2A, 0xFF);
		SDL_RenderClear(renderer);
		if (curr_gamestate == GameState::MainMenu) {

			/// Draw

			menu->Render(renderer);

		}
		else if (curr_gamestate == GameState::FloorChoosing) {
			floor_choosing -> Render(renderer);
		}

		else if (curr_gamestate == GameState::Playing){
			curr_floor->Update();

			int &health = curr_floor->player->health;

			const Uint8* key_state = SDL_GetKeyboardState(NULL);

			if (key_state[SDL_SCANCODE_KP_PLUS]) health++;

			/// Draw
			
			curr_floor->Draw();


			if (curr_floor->IsLose()) {
				curr_gamestate = GameState::Lose;
				Mix_PlayChannel(-1, sound_lose, 0);
			}
			if (curr_floor->IsWin()) {
				curr_gamestate = GameState::Win;
				Mix_PlayChannel(-1, sound_win, 0);
			}
		}
		else if (curr_gamestate == GameState::Lose) {
			losing->Render(renderer);
		}
		else if (curr_gamestate == GameState::Win) {
			winning->Render(renderer);
		}
		

		
		SDL_SetRenderDrawColor(renderer, 0x2A, 0x2A, 0x2A, 0xFF);


		SDL_RenderPresent(renderer);


		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICKS_PER_FRAME )
		{
			//Wait remaining time
			SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks);
		}

	}

	close();


	SDL_FreeSurface(icon);

	return 0;
}

