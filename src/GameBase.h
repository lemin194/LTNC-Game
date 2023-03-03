#ifndef GAMEBASE_
#define GAMEBASE_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <math.h>


const std::string WINDOW_TITLE = "Project LTNC";


static int LEVEL_WIDTH = 640;
static int LEVEL_HEIGHT = 640;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_FPS = 70;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;


const double PI = 3.1415926535897932;

const int CHARACTER_WIDTH = 20;
const int CHARACTER_HEIGHT = 24;

const int STORM_WIDTH = 40;
const int STORM_HEIGHT = 48;



enum { SDL_ERROR, IMG_ERROR, MIX_ERROR, TTF_ERROR };


void LogError(std::string msg, int error_code = SDL_ERROR);

SDL_Texture* myLoadTexture( std::string path, SDL_Renderer* gRenderer );

SDL_Surface* myLoadSurface ( std::string path, SDL_PixelFormat* fmt, SDL_Renderer* renderer );

bool CheckCollisionRectangle(SDL_Rect r1, SDL_Rect r2);

#endif