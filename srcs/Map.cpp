#include "Map.h"
#include "game.h"
#include <fstream>
#include "ECS.h"
//#include "Components.h"

#include "ColliderComponent.h"
#include "TextureManager.h"
#include "TileComponent.h"

#include <cstdlib>
#include <string>
#include <ctype.h>
#include <stdlib.h>

extern Manager manager;

//Read till back
int RTB(std::ifstream& file)
{
    char c = ' ';
    bool run = true;
    std::string myNum;
    while (run)
    {
        file.get(c);
        if (isdigit(c))
        {
            myNum += c;
        }
        else
        {
            run = false;
        }
    }
    
    return stoi(myNum);
}

Map::Map(const char* mfp, const char* tsfp, int ms) : mapFilePath(mfp), tileSetFilePath(tsfp), mapScale(ms)
{
    texture = TextureManager::LoadTexture(mfp);
}

Map::~Map()
{
    SDL_DestroyTexture(texture);
}

void Map::LoadMap(std::string colPath, std::string tilePath)
{
    
    //---------------------------------------------------------------------------------------
    //    Find Number of Tiles in the Set  &&  Find Number of Tiles Long the TileSheet is
    //---------------------------------------------------------------------------------------
    
    std::ifstream tileSetFile;
    int numTiles, tilesLong;
    char c = ' ';
    std::string tag;
    
    tileSetFile.open(tileSetFilePath);
    if (!tileSetFile.is_open())
    {
        std::cout << "Tile set file did NOT open!" << std::endl;
    }
    
    tileSetFile.get(c);
    tilesLong = RTB(tileSetFile);
    numTiles = RTB(tileSetFile);
    
    tileSetFile.close();
    
    
    //----------------------------------------------
    //    Open, Read, and Assign Values for Tiles
    //----------------------------------------------
    std::ifstream mapFile;
    
    int x, y, index, layer;
    std::string temp;
    
    int mapWidth, mapHeight, layers;
    
    bool run;
    
    mapFile.open(tilePath);
    if (!mapFile.is_open())
    {
        std::cout << "Map file did NOT open!" << std::endl;
    }
    
    run = true;
    while (run)
    {
        mapFile.get(c);
        if (c == '%')
        {
            mapWidth = RTB(mapFile);
            mapHeight = RTB(mapFile);
            tileWidth = RTB(mapFile);
            tileHeight = RTB(mapFile);
            layers = RTB(mapFile);
            
            run = false;
        }
        else if (mapFile.eof())
        {
            std::cout << "IN tileMap.txt, Couldn't find values for: map width, map height, tile width, tile height, and layers..." << std::endl;
            
            mapWidth = 100;
            mapHeight = 100;
            tileWidth = 32;
            tileHeight = 32;
            layers = 8;
            
            run = false;
        }
    }
    mapFile.clear();
    mapFile.seekg(0, std::ios_base::beg);
    
    //(int srcX, int srcY, int tileWidth, int tileHeight, int mScale, SDL_Texture* tex)
    //---------------------------------
    //     Load tiles on tile sheet
    //---------------------------------
    
    
    
    for (int t = numTiles; t > 0; t--)
    {
        
        
        x = t % tilesLong;
        y = t / tilesLong;
        
        AddTile(x, y, t);
    }
    
    
    //----------------------------
    //     Load tiles for map
    //----------------------------
    
    run = true;
    while (run)
    {
        mapFile.get(c);
        mapFile.get(c);
        if (mapFile.eof())
        {
            run = false;
            continue;
        }
        
        mapFile.unget();
        mapFile.unget();
        mapFile.get(c);
        
        if (c == '<')
        {
            x = RTB(mapFile);
            y = RTB(mapFile);
            layer = RTB(mapFile);
            index = RTB(mapFile);
            
            AddTile(x, y, layer, index);
            
        }
    }
    
    mapFile.close();
    
    //-----
    // Load colliders for map
    //
    
    mapFile.open(colPath);
    
    if (!mapFile.is_open())
    {
        std::cout << "Map file did NOT open!" << std::endl;
    }
    
    run = true;
    while (run)
    {
        
        mapFile.get(c);
        if (c == '%')
        {
            if (mapWidth != RTB(mapFile))
            {
                std::cout << "Map width does not match between tileMap.txt and colMap.txt..." << std::endl;
            }
            if (mapHeight != RTB(mapFile))
            {
                std::cout << "Map height does not match between tileMap.txt and colMap.txt..." << std::endl;
            }
            if (tileWidth != RTB(mapFile))
            {
                std::cout << "Tile width does not match between tileMap.txt and colMap.txt..." << std::endl;
            }
            if (tileHeight != RTB(mapFile))
            {
                std::cout << "Tile height does not match between tileMap.txt and colMap.txt..." << std::endl;
            }
            if (layers != RTB(mapFile))
            {
                std::cout << "Layer thickness does not match between tileMap.txt and colMap.txt..." << std::endl;
            }
            
            run = false;
        }
        else if (mapFile.eof())
        {
            std::cout << "IN colMap.txt, Couldn't find values for: map width, map height, tile width, tile height, and layers..." << std::endl;
        }
    }
    mapFile.clear();
    mapFile.seekg(0, std::ios_base::beg);
    
    run = true;
    while (run)
    {
        mapFile.get(c);
        mapFile.get(c);
        if (mapFile.eof())
        {
            run = false;
            continue;
        }
        
        mapFile.unget();
        mapFile.unget();
        mapFile.get(c);
        
        
        if (c == '<')
        {
            
            x = RTB(mapFile);
            y = RTB(mapFile);
            index = RTB(mapFile);
            
            
            switch (index)
            {
                case 0:
                    tag = "t- block";
                    break;
                    
                    
                case 1:
                    tag = "t- d- slab";
                    break;
                    
                case 2:
                    tag = "t- d- slope 1x";
                    break;
                    
                case 3:
                    tag = "t- d- slope 1/2x";
                    break;
                    
                case 4:
                    tag = "t- d- slope 1/2x + .5";
                    break;
                    
                case 5:
                    tag = "t- d- slope -1x";
                    break;
                    
                case 6:
                    tag = "t- d- slope -1/2x";
                    break;
                    
                case 7:
                    tag = "t- d- slope -1/2x - .5";
                    break;
                    
                    
                    
                case 8:
                    tag = "t- u- slab";
                    break;
                    
                case 9:
                    tag = "t- u- slope 1x";
                    break;
                    
                case 10:
                    tag = "t- u- slope 1/2x";
                    break;
                    
                case 11:
                    tag = "t- u- slope 1/2x + .5";
                    break;
                    
                case 12:
                    tag = "t- u- slope -1x";
                    break;
                    
                case 13:
                    tag = "t- u- slope -1/2x";
                    break;
                    
                case 14:
                    tag = "t- u- slope -1/2x - .5";
                    break;
                    
                    
                default:
                    tag = "t- block";
                    break;
            }
            
            auto& tcol(manager.addEntity());
            tcol.addComponent<ColliderComponent>(tag, x * tileWidth * mapScale, y * tileHeight * mapScale, tileWidth, tileHeight, mapScale);
            tcol.addGroup(game::groupColliders);
            
        }
        
    }
    
    mapFile.close();
    
}

void Map::AddTile(int srcX, int srcY, int i)
{
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, tileWidth, tileHeight, mapScale, i, texture);
    tile.addGroup(game::groupTiles);
}

void Map::AddTile(int xpos, int ypos, int layer, int index)
{
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(xpos, ypos, index);
    
switch (layer)
    {
        case 0:
            tile.addGroup(game::groupLayer1);
            break;
            
        case 1:
            tile.addGroup(game::groupLayer2);
            break;
            
        case 2:
            tile.addGroup(game::groupLayer3);
            break;
            
        case 3:
            tile.addGroup(game::groupLayer4);
            break;
            
        case 4:
            tile.addGroup(game::groupLayer5);
            break;
            
        case 5:
            tile.addGroup(game::groupLayer6);
            break;
            
        case 6:
            tile.addGroup(game::groupLayer7);
            break;

    }
}
