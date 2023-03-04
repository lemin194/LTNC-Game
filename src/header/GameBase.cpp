#include "GameBase.h"

void LogError(std::string msg, int error_code)
{
	if (error_code == SDL_ERROR)
	{
		std::cout << msg << SDL_GetError() << std::endl;
	}
	if (error_code == IMG_ERROR)
	{
		std::cout << msg << IMG_GetError() << std::endl;
	}
	if (error_code == MIX_ERROR)
	{
		// std::cout << msg << Mix_GetError() << std::endl;
	}
	if (error_code == TTF_ERROR)
	{
		// std::cout << msg << TTF_GetError() << std::endl;
	}
}



SDL_Texture* myLoadTexture( std::string path, SDL_Renderer* renderer)
{
	SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load ( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), IMG_GetError() );
		return NULL;
    }
	
	newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface);
	if( newTexture == NULL )
	{
		printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}

	SDL_FreeSurface( loadedSurface );

	return newTexture;
}
SDL_Surface* myLoadSurface ( std::string path, SDL_PixelFormat* fmt, SDL_Renderer* renderer )
{
	SDL_Surface* optimizedSurface;
    SDL_Surface* loadedSurface = IMG_Load ( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), IMG_GetError() );
		return NULL;
    }
	
	optimizedSurface = SDL_ConvertSurface( loadedSurface, fmt, 0 );
	if( optimizedSurface == NULL )
	{
		printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}

	SDL_FreeSurface( loadedSurface );

	return optimizedSurface;
}

bool CheckCollisionRectangle(SDL_Rect r1, SDL_Rect r2) {
    int a1 = std::min(r1.x + r1.w, r2.x + r2.w);
    int a2 = std::max(r1.x, r2.x);
    int b1 = std::min(r1.y + r1.h, r2.y + r2.h);
    int b2 = std::max(r1.y, r2.y);
    return a1 > a2 && b1 > b2;
}

