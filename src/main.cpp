
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
#include "header/Floor1.h"
#include "header/Button.h"
#include "header/shProjectile.h"
#include "header/LTimer.h"
#include "header/GameGUI.h"




Character c;
TileSet tileSet;


LTexture gTextTexture1;

SDL_Texture* bgImage;
SDL_Rect camera;
std::vector<Projectile> projector_vec;
std::vector<Golem*> gols;
std::vector<Goblin*> gobs;


SDL_Surface* screenSurface = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;

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

LTexture* heart;

TTF_Font* GameFloor::gFont;
TTF_Font* GameFloor::font_ui;
TTF_Font* font_ui;


Sprite spr;



int loadMedia() {
	bgImage = myLoadTexture("..\\assets\\img.png", renderer);

	if (bgImage == NULL) {
		std::cout << "cant load bg.\n";
		return 0;
	}

	TileSet::texture = new LTexture();
	TileSet::texture->LoadFromFile("..\\assets\\sprites\\tileSet_x2.png", renderer);


	std::string *paths = new std::string[2] {
						"..\\assets\\sprites\\char\\char.png",
						"..\\assets\\sprites\\char\\char.png"
						};

	for (int i = 0; i < 2; i ++) Character::textures[i] = new LTexture();
	Character::textures[0]->LoadFromFile("..\\assets\\sprites\\char\\char.png", renderer);
	Character::textures[1]->LoadFromFile("..\\assets\\sprites\\char\\char.png", renderer);

	Weapon::texture = new LTexture();
	Weapon::texture->LoadFromFile("..\\assets\\sprites\\char\\weapon-sheet.png", renderer);

	Golem::texture = new LTexture();
	Golem::texture->LoadFromFile("..\\assets\\sprites\\enemy\\golem_.png", renderer);

	Projectile::texture = new LTexture();
	Projectile::texture->LoadFromFile("..\\assets\\sprites\\projectile.png", renderer);

	shProjectile::texture = new LTexture();
	shProjectile::texture->LoadFromFile("..\\assets\\sprites\\enemy\\stormhead\\bullet.png", renderer);

	Goblin::texture = new LTexture();
	Goblin::texture->LoadFromFile("..\\assets\\sprites\\enemy\\goblin_.png", renderer);

	StormHead::texture_idle = new LTexture();
	StormHead::texture_idle->LoadFromFile("..\\assets\\sprites\\enemy\\stormhead\\sh-idle.png", renderer);

	StormHead::texture_run = new LTexture();
	StormHead::texture_run->LoadFromFile("..\\assets\\sprites\\enemy\\stormhead\\sh-run.png", renderer);

	StormHead::texture_attack = new LTexture();
	StormHead::texture_attack->LoadFromFile("..\\assets\\sprites\\enemy\\stormhead\\sh-attack.png", renderer);

	StormHead::texture_attack2 = new LTexture();
	StormHead::texture_attack2->LoadFromFile("..\\assets\\sprites\\enemy\\stormhead\\sh-attack2.png", renderer);

	StormHead::texture_damaged = new LTexture();
	StormHead::texture_damaged->LoadFromFile("..\\assets\\sprites\\enemy\\stormhead\\sh-damaged.png", renderer);

	StormHead::texture_death = new LTexture();
	StormHead::texture_death->LoadFromFile("..\\assets\\sprites\\enemy\\stormhead\\sh-death.png", renderer);

	StormHead::texture_death = new LTexture();
	StormHead::texture_death->LoadFromFile("..\\assets\\sprites\\enemy\\stormhead\\sh-death.png", renderer);

	heart = new LTexture();
	heart->LoadFromFile("..\\assets\\sprites\\heart.png", renderer);


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

	GameFloor::gFont = TTF_OpenFont( "..\\assets\\font\\ARCADECLASSIC.TTF", 64 );
    if( GameFloor::gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    }
	font_ui = TTF_OpenFont( "..\\assets\\font\\font_ui.TTF", 48 );
    if( font_ui == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    }
	GameFloor::font_ui = font_ui;

	// spr.texture = Character::textures[0];
	spr.texture = new LTexture();
	spr.texture->LoadFromFile("..\\assets\\sprites\\projectile.png", renderer);
	spr.center = {4, 4};
	spr.clipTime = 0.1;
	spr.InitClock();
	spr.AddClip({0, 0, 8, 8});

	return 1;
}






int mySdlInit() {
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
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

SDL_Renderer* GameFloor::renderer;
LTimer capTimer;

GameFloor* curr_floor;

LTexture* txt_healthBar;
LTexture* txt_dies     ;
GameGUI* menu;
GameGUI* winning;
GameGUI* losing;



void close() {
	TileSet::texture->Free();
	Character::textures[0]->Free();
	Character::textures[1]->Free();
	Weapon::texture->Free();
	Projectile::texture->Free();
	Golem::texture->Free();

	delete menu;
	delete winning;
	delete losing;
	delete curr_floor;
	txt_dies     ->Free();
	txt_healthBar->Free();

	spr.texture->Free();


	gTextTexture1.Free();
    TTF_CloseFont( GameFloor::gFont );


	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(bgImage);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
	SDL_Quit();

	std::cout << "closed.";
}


int main( int argc, char* args[] )
{
	srand(time(NULL));


	int ok = mySdlInit();

	SDL_Surface* icon = myLoadSurface("..\\assets\\icon.png", screenSurface->format, renderer);
	SDL_SetWindowIcon(window, icon);


	loadMedia();

	GameFloor::renderer = renderer;



	txt_healthBar = new LTexture();
	txt_dies      = new LTexture();

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
	


	txt_healthBar->loadFromRenderedText("Health:", {255, 255, 255, 255}, font_ui, renderer);
	txt_dies->loadFromRenderedText("You Died. Press R to restart.", {255, 255, 255, 255}, font_ui, renderer);
	auto restart = [&] () {
		curr_floor = new Floor1();
		curr_floor->GameInit();
		curr_floor->player->health = 5;
		
	};
	restart();

	auto render_txt = [&] (LTexture* txt_texture, int x, int y) {
		txt_texture->Render(x, y, renderer, NULL, 0, NULL, SDL_FLIP_NONE, 1);
	};

	int pause = 0;
	int quit = 0;
	bool Playing = false;
	bool IsWin = false, IsLose = false;
	while( true )
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
				break;
			}
			if (!Playing) {
				int btn_click = menu->HandleMouseEvent(e);
				if (btn_click == 2) {
					quit = true;
					break;
				}
				if (btn_click == 1) {
					Playing = true;
					restart();
				}
			}
			else {
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_r) {
						restart();
					}
				}

				if (!IsWin && !IsLose) {
					curr_floor->HandleMouseEvents(e);
				}
				else if (IsWin) {
					int btn_click = winning->HandleMouseEvent(e);
					if (btn_click == 2) {
						Playing = true;
						IsWin = false;
						IsLose = false;
						restart();
					}
					if (btn_click == 3) {
						Playing = false;
						IsWin = false;
						IsLose = false;
					}
				}
				else if (IsLose) {
					int btn_click = losing->HandleMouseEvent(e);
					if (btn_click == 2) {
						Playing = true;
						IsWin = false;
						IsLose = false;
						restart();
					}
					if (btn_click == 3) {
						Playing = false;
						IsWin = false;
						IsLose = false;
					}
				}
			}
		}
		if (quit) break;



		SDL_SetRenderDrawColor(renderer, 0x2A, 0x2A, 0x2A, 0xFF);
		SDL_RenderClear(renderer);
		if (!Playing) {

			/// Draw

			menu->Render(renderer);

		}

		else  {
			if (!IsLose && !IsWin) {
				
				curr_floor->Update();

				int &health = curr_floor->player->health;

				const Uint8* key_state = SDL_GetKeyboardState(NULL);

				if (key_state[SDL_SCANCODE_KP_PLUS]) health++;

				/// Draw
				
				curr_floor->Draw();


				if (curr_floor->IsLose()) {
					IsLose = true;
				}
				if (curr_floor->IsWin()) {
					IsWin = true;
				}
			}
			else if (IsLose) {
				losing->Render(renderer);
			}
			else if (IsWin) {
				winning->Render(renderer);
			}
		}
		SDL_SetRenderDrawColor(renderer, 0x2A, 0x2A, 0x2A, 0xFF);


		SDL_RenderPresent(renderer);



		int frameTicks = capTimer.getTicks();
		if( frameTicks < SCREEN_TICKS_PER_FRAME )
		{
			//Wait remaining time
			SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
		}

	}

	close();


	SDL_FreeSurface(icon);

	return 0;
}

