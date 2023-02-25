#include "LTexture.h"

LTexture::LTexture()
{
	mTexture = NULL;

	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture() {
    Free();
}

void LTexture::Free() {
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
    }
    mWidth = 0;
    mHeight = 0;

}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* gFont, SDL_Renderer *gRenderer )
{
    //Get rid of preexisting texture
    Free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return mTexture != NULL;
}


bool LTexture::LoadFromFile(std::string path, SDL_Renderer *gRenderer) {
    Free();
    SDL_Texture* loadedTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    
	if (loadedSurface == NULL)
	{
		LogError("Cant load image.", IMG_ERROR);
        return 0;
	}
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));
    loadedTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (loadedTexture == NULL) {
        LogError("Cant create texture from surface." + path, SDL_ERROR);
        return 0;
    }
    mWidth = loadedSurface->w; mHeight = loadedSurface->h;
    
    SDL_FreeSurface(loadedSurface);

    mTexture = loadedTexture;
    return 1;

}

void LTexture::Render(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip, double angle, const SDL_Point*  center, const SDL_RendererFlip flip, int resize) {
    SDL_Rect renderSpace = {x, y, mWidth * resize, mHeight * resize};
    if (clip != NULL) {
        renderSpace.w = clip->w * resize;
        renderSpace.h = clip->h * resize;
    }

    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderSpace, angle, center, flip);
}

int LTexture::GetWidth() {
    return mWidth;
}

int LTexture::GetHeight() {
    return mHeight;
}