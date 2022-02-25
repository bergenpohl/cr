#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "TextureManager.h"

#include "ECS.h"
#include "TransformComponent.h"

class ColliderComponent : public Component
{
public:
    
    SDL_Rect collider;
    Vector2D lastPos = {0, 0};
    std::string tag;
    
    SDL_Texture* tex;
    SDL_Rect srcR, destR;
    
    TransformComponent* transform;
    
    int xoff, yoff, woff, hoff = 0;
    int scale = 3;
    
    bool dDraw = true;
    
    ColliderComponent()
    {
        
    }
    
    ColliderComponent(std::string t)
    {
        tag = t;
    }
    
    ColliderComponent(std::string t, int xpos, int ypos, int width, int height, int sc)
    {
        tag = t;
        
        collider.x = lastPos.x = xpos;
        collider.y = lastPos.y = ypos;
        collider.w = width * sc;
        collider.h = height * sc;
    }
    
    ColliderComponent(std::string t, int xpos, int ypos, int width, int height, int xo, int yo, int wo, int ho, int sc, bool dD)
    {
        tag = t;
        
        collider.x = lastPos.x = xpos + (xo * sc);
        collider.y = lastPos.y = ypos + (yo * sc);
        collider.w = (width + wo) * sc;
        collider.h = (height + ho) * sc;
        
        xoff = xo;
        yoff = yo;
        woff = wo;
        hoff = ho;
        dDraw = dD;
    
    }
    
    ~ColliderComponent()
    {
        
    }
    
    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        
        transform = &entity->getComponent<TransformComponent>();
        
        
        srcR = {0, 0, 32, 32};
        
        if (tag == "player")
        {
            tex = TextureManager::LoadTexture("assets/cColTex.png");
            destR = {collider.x - game::camera.x, collider.y - game::camera.y, collider.w, collider.h};
        }
        else
        {
            tex = TextureManager::LoadTexture("assets/yColTex.png");
            destR = {collider.x, collider.y, collider.w, collider.h};
        }
        
        
    }
    
    void update() override
    {
        if (tag[0] != 't' && tag[1] != '-')
        {
            lastPos.x = collider.x;
            lastPos.y = collider.y;
            
            collider.x = static_cast<int>(transform->position.x) + xoff;
            collider.y = static_cast<int>(transform->position.y) + yoff;
            
            
        }
        
        destR.x = collider.x - game::camera.x;
        destR.y = collider.y - game::camera.y;
    }
    
    void draw() override
    {
        if (dDraw)
        {
            TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
        }
    }
    
    
};
