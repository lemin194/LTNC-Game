#ifndef TILESET_H_
#define TILESET_H_

#include "Sprite.h"
#include "Character.h"
#include "Enemy.h"
#include "Projectile.h"
#include <vector>
class TileSet{
public:
    static LTexture* texture;
    void Init(int w, int h) ;
    void LoadMap(std::string path);
    void LoadMapCollision(std::string path);

    void CollideCharacter(Character& c);
    void CollideEnemy(Enemy& e);
    int CollideProjectile(Projectile& p);
    int GetLevelWidth();
    int GetLevelHeight();

    void RenderMap(SDL_Renderer* gRenderer, int x, int y, int cam_offset_x, int cam_offset_y) ;

private:
    int tile_w, tile_h, map_w, map_h;
    Sprite tile;
    std::vector<std::vector<int>> vec;
    std::vector<std::vector<int>> vec_col;
};


#endif