#pragma once

//#include "Components.h"
#include "Vector2D.h"
#include <string>

class TransformComponent : public Component
{
public:
    
    Vector2D position;
    Vector2D startPosition;
    Vector2D velocity;
    
    float xSpeed = 1;
    float ySpeed = 1;
    
    int maxSpeed = 30;
    
    int height = 32;
    int width = 32;
    int scale = 3;
    
    int paralax = 1;
    std::string tag = "foreground";
    
    bool grounded = true;
    
    TransformComponent()
    {
        startPosition.Zero();
        position.Zero();
    }
    
    TransformComponent(int sc)
    {
        startPosition.Zero();
        position.Zero();
        scale = sc;
    }
    
    TransformComponent(float x, float y)
    {
        position.x = startPosition.x = x;
        position.y = startPosition.y = y;
    }
    
    TransformComponent(float x, float y, int w, int h, int sc)
    {
        position.x = startPosition.x = x;
        position.y = startPosition.y = y;
        width = w;
        height = h;
        scale = sc;
    }
    
    void init() override
    {
        velocity.Zero();
    }
    
    void update() override
    {
        if (tag == "foreground")
        {
            position.x += velocity.x * xSpeed;
            position.y += velocity.y * ySpeed;
        }
    }
    
};


