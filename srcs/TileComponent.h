#pragma once

#include "ECS.h"
#include "Vector2D.h"
#include "game.h"
#include "TextureManager.h"


extern Manager manager;

class TileComponent : public Component
{
public:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    Vector2D position;
    
    SDL_Rect* srcRectP, destRectP;
    
    int identifier = -1;
    int index = 0;
    
    int scale;
    
    TileComponent() = default;
    
    ~TileComponent()
    {
        
    }
    
    TileComponent(int srcX, int srcY, int tileWidth, int tileHeight, int mScale, int ind, SDL_Texture* tex)
    {
        index = ind;
        
        texture = tex;
        
        srcRect.x = srcX * tileWidth;
        srcRect.y = srcY * tileHeight;
        srcRect.w = tileWidth;
        srcRect.h = tileHeight;
        
        scale = mScale;
    }
    
    TileComponent(int xPos, int yPos, int tIndex)
    {
        identifier = 1;
        
        auto& tiles(manager.getGroup(game::groupTiles));
        
        for (auto& t : tiles)
        {
            if (t->getComponent<TileComponent>().index == tIndex)
            {
                texture = t->getComponent<TileComponent>().texture;
                
                srcRectP = &t->getComponent<TileComponent>().srcRect;
                scale = t->getComponent<TileComponent>().scale;
                
                position.x = xPos * srcRectP->w * scale;
                position.y = yPos * srcRectP->h * scale;
                
                destRect.x = xPos * srcRectP->w * scale;
                destRect.y = yPos * srcRectP->h * scale;
                destRect.w = srcRectP->w * scale;
                destRect.h = srcRectP->h * scale;
                
                return;
            }
        }
        
        
        
    }
    
    
    void update() override
    {
        destRect.x = position.x - game::camera.x;
        destRect.y = position.y - game::camera.y;
    }
    
    void draw() override
    {
        if (identifier != -1)
        {
            TextureManager::Draw(texture, srcRectP, destRect, SDL_FLIP_NONE);
            
        }
        
        //Do nothing if identifier == -1 (aka the tile is a sprite holder)
        
    }
};

