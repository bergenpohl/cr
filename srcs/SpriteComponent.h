#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

#include "SDL2/SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>


class SpriteComponent : public Component
{
public:
    SDL_Rect srcRect;
    
    int animIndex = 0;
    std::map<const char*, Animation> animations;
    
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    
    SpriteComponent() = default;
    SpriteComponent(const char* path)
    {
        setTex(path);
    }
    
    SpriteComponent(const char* path, bool isAnimated)
    {
        animated = isAnimated;
        
        Animation idle = Animation(0, 1, 100);
        Animation walk = Animation(1, 5, 180);
        Animation run = Animation(1, 5, 100);
        
        animations.emplace("Idle", idle);
        animations.emplace("Walk", walk);
        animations.emplace("Run", run);
        
        Play("Idle");
        
        setTex(path);
    }
    
    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }
    
    void setTex(const char* path)
    {
        texture = TextureManager::LoadTexture(path);
    }
    
    
    
    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        
        transform = &entity->getComponent<TransformComponent>();
        
        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
    }
    
    void update() override
    {
        if (animated)
        {
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
        }
        
        if (entity->hasComponent<ColliderComponent>())
        {
            destRect.x = static_cast<int>(transform->position.x - (entity->getComponent<ColliderComponent>().xoff * (transform->scale - 1)) - game::camera.x);
            destRect.y = static_cast<int>(transform->position.y - (entity->getComponent<ColliderComponent>().yoff * (transform->scale - 1)) - game::camera.y);
        }
        else
        {
            destRect.x = static_cast<int>(transform->position.x - game::camera.x);
            destRect.y = static_cast<int>(transform->position.y - game::camera.y);
        }
        
        srcRect.y = animIndex * transform->height;
        
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }
    
    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }
    
    void Play(const char* animName)
    {
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }
    
private:
    TransformComponent* transform;
    ColliderComponent* collider;
    
    SDL_Texture *texture;
    SDL_Rect destRect;
    
    bool animated = false;
    int frames = 0;
    int speed = 100;
};
