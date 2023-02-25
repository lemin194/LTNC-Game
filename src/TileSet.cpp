#include "TileSet.h"
#include "Character.h"

void TileSet::Init(int tileWidth, int tileHeight) {
    tile.texture = texture;
    this->tile_w = tileWidth; this->tile_h = tileHeight;
    vec.clear();
    
    for (int i = 0; i < texture->GetHeight() / tile_h; i++) {
        for (int j = 0; j < texture->GetWidth() / tile_w; j++) {
            tile.AddClip({j * tile_w, i * tile_h, tile_w, tile_h});
        }
    }
}

int TileSet::GetLevelWidth() {
    return map_w * tile_w;
}

int TileSet::GetLevelHeight() {
    return map_h * tile_h;
}

void TileSet::LoadMap(std::string path) {
    this->vec.clear();
    std::ifstream f(path);
    f >> this->map_w >> this->map_h;
    this->vec.resize(map_h);

    for (int i = 0; i < map_h; i++) {
        for (int j = 0; j < map_w; j++) {
            int x; f >> x;
            this->vec[i].push_back(x);
        }
    }
}

void TileSet::LoadMapCollision(std::string path) {
    this->vec_col.clear();
    std::ifstream f(path);
    vec_col.resize(map_h);
    f >> this->map_w >> this->map_h;

    for (int i = 0; i < map_h; i++) {
        for (int j = 0; j < map_w; j++) {
            int x; f >> x;
            // std::cout << (x > 0) << " ";
            this->vec_col[i].push_back(x > 0);
        }
        // std::cout << "\n";
    }
    // std::cout << "k";
}

void TileSet::CollideCharacter(Character &c) {
    auto [cx, cy, tmp1, tmp2] = c.GetHitbox();

    int gridX = cx / tile_w;
    int gridY = cy / tile_h;

    if (gridX >= map_w || gridX < 0 || gridY >= map_h || gridY < 0) return;

    for (int i = gridY - 1; i <= gridY + 1; i++) {
        for (int j = gridX - 1; j <= gridX + 1; j++) {
            if (i >= map_h || i < 0 || j >= map_w || j < 0) {
                SDL_Rect r = {j * tile_w, i * tile_h, tile_w, tile_h};
                c.Collide(r);
                continue;
            }
            else if (vec_col[i][j]) {
                SDL_Rect r = {j * tile_w, i * tile_h, tile_w, tile_h};
                c.Collide(r);
            }
        }
    } 

}

void TileSet::CollideEnemy(Enemy &e) {
    auto [cx, cy, tmp1, tmp2] = e.GetHitbox();

    int gridX = cx / tile_w;
    int gridY = cy / tile_h;

    if (gridX >= map_w || gridX < 0 || gridY >= map_h || gridY < 0) return;

    for (int i = gridY - 1; i <= gridY + 1; i++) {
        for (int j = gridX - 1; j <= gridX + 1; j++) {
            if (i >= map_h || i < 0 || j >= map_w || j < 0) {
                SDL_Rect r = {j * tile_w, i * tile_h, tile_w, tile_h};
                e.Collide(r);
                continue;
            }
            else if (vec_col[i][j]) {
                SDL_Rect r = {j * tile_w, i * tile_h, tile_w, tile_h};
                e.Collide(r);
            }
        }
    } 

}

int TileSet::CollideProjectile(Projectile& c) {
    auto [cx, cy, tmp1, tmp2] = c.GetHitbox();

    int gridX = cx / tile_w;
    int gridY = cy / tile_h;
    
    if (gridX >= map_w || gridX < 0 || gridY >= map_h || gridY < 0) return 1;

    for (int i = gridY - 1; i <= gridY + 1; i++) {
        for (int j = gridX - 1; j <= gridX + 1; j++) {
            if (i >= map_h || i < 0 || j >= map_w || j < 0) {
                SDL_Rect r = {j * tile_w, i * tile_h, tile_w, tile_h};
                if (CheckCollisionRectangle(r, c.GetHitbox())) return 1;
                continue;
            }
            else if (vec_col[i][j]) {
                SDL_Rect r = {j * tile_w, i * tile_h, tile_w, tile_h};
                if (CheckCollisionRectangle(r, c.GetHitbox())) return 1;
            }
        }
    }

    return 0;
}


void TileSet::RenderMap(SDL_Renderer* gRenderer, int x, int y, int cam_offset_x, int cam_offset_y) {
    for (int i = 0; i < map_h; i++) {
        for (int j = 0; j < map_w; j++) {
            int render_x = x + j * tile_w, render_y = y + i * tile_h;
            tile.SetFrameId(vec[i][j] - 1);
            tile.Render(render_x - cam_offset_x, render_y - cam_offset_y, gRenderer);
        }
    }
}