#ifndef LTEXTURE_H_
#define LTEXTURE_H_

#include "GameBase.h"

class LTexture
{
public:
	LTexture();

	~LTexture();

	void Free();
    
	bool loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* gFont, SDL_Renderer *gRenderer );

	bool LoadFromFile(std::string path, SDL_Renderer *gRenderer);

	void Render(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip, double angle, const SDL_Point*  center, const SDL_RendererFlip flip, int resize = 1);

	int GetWidth();

	int GetHeight();

	SDL_Texture* mTexture;

private:
	int mWidth;
	int mHeight;
};

#endif