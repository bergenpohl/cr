#pragma once
#include <string>
#include <vector>
#include "ECS.h"
#include "SDL2/SDL.h"

class Map
{
public:
    SDL_Texture* texture;
    std::vector<Entity*> tiles;
    
    Map(const char* mfp, const char* tsfp, int ms);
    ~Map();
    
    void LoadMap(std::string colPath, std::string tilePath);
    void AddTile(int srcX, int srcY, int i);
    void AddTile(int xpos, int ypos, int layer, int index);
    
private:
    const char* mapFilePath;
    const char* tileSetFilePath;
    int mapScale;
    int tileWidth, tileHeight;
    //int scaledSize;
    
};

