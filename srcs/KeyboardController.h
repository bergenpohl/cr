#pragma once

#include "game.h"
#include "ECS.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"

class KeyboardController : public Component
{
public:
    
    TransformComponent* transform;
    SpriteComponent* sprite;
    ColliderComponent* collider;
    
    
    bool up, down, left, right, jump, ajump, sprint = false;
    
    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }
    
    void update() override
    {
        
        if (game::event.type == SDL_KEYDOWN)
        {
            switch (game::event.key.keysym.sym)
            {
                    
                case SDLK_a:
                    left = true;
                    break;
                    
                case SDLK_d:
                    right = true;
                    break;
                    
                case SDLK_w:
                    up = true;
                    break;
                    
                case SDLK_s:
                    down = true;
                    break;
                    
                case SDLK_LSHIFT:
                    sprint = true;
                    break;
                    
                case SDLK_SPACE:
                    jump = true;
                    break;
                    
                case SDLK_h:
                    transform->position.Zero();
                    break;
                    
                case SDLK_p:
                    std::cout << '(' << transform->position.x << ',' << transform->position.y << ')' << std::endl;
                    break;
                    
                case SDLK_ESCAPE:
                    game::isRunning = false;
                    break;
                    
                default:
                    break;
            }
        }
        
        
        
        if (game::event.type == SDL_KEYUP)
        {
            
            switch (game::event.key.keysym.sym)
            {
                case SDLK_a:
                    left = false;
                    break;
                    
                case SDLK_d:
                    right = false;
                    break;
                    
                case SDLK_w:
                    up = false;
                    break;
                    
                case SDLK_s:
                    down = false;
                    break;
                    
                case SDLK_LSHIFT:
                    sprint = false;
                    break;
                    
                case SDLK_SPACE:
                    jump = false;
                    
                default:
                    break;
            }
        }
        
        
    }
    
    void playerMovement()
    {
        
        sprite->Play("Idle");
        transform->velocity.x = 0;
        transform->xSpeed = 3;
        
        
        if (up)
        {
            transform->velocity.y = -1;
        }
        
        if (down)
        {
            transform->velocity.y = 1;
        }
        
        
        
        if (left)
        {
            transform->velocity.x = -1;
            sprite->Play("Walk");
            sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
        }
        
        if (right)
        {
            transform->velocity.x = 1;
            sprite->Play("Walk");
            sprite->spriteFlip = SDL_FLIP_NONE;
        }
        
        
        if (up && down)
        {
            transform->velocity.y = 0;
        }
        
        
        if (left && right)
        {
            sprite->Play("Idle");
            transform->velocity.x = 0;
        }
        
        if (sprint)
        {
            sprite->Play("Run");
            transform->xSpeed = 10;
        }
        
        if (jump && !ajump && transform->grounded)
        {
            transform->velocity.y = -1;
            transform->ySpeed = transform->maxSpeed; //game::jumpSpeed;
            
            ajump = true;
        }
        
    }
    
    
};
