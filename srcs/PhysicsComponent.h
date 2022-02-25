//
//  Physics.hpp
//  Circi's Realm
//
//  Created by Bergen Pohl on 9/13/19.
//  Copyright © 2019 Bergen Pohl. All rights reserved.
//

extern Manager manager;

#pragma once

#include <string>
#include <cmath>
#include "SDL2/SDL.h"

#include "game.h"

#include "ECS.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "SpriteComponent.h"
#include "KeyboardController.h"

class PhysicsComponent : public Component
{
public:
    
    PhysicsComponent()
    {
    }
    
    PhysicsComponent(bool g)
    {
        hasGravity = g;
    }
    PhysicsComponent(bool g, float cg)
    {
        hasGravity = g;
        curvatureGrav = cg;
    }
    
    ~PhysicsComponent()
    {
    }
    
    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        
        transform = &entity->getComponent<TransformComponent>();
        
        if (!entity->hasComponent<ColliderComponent>())
        {
            entity->addComponent<ColliderComponent>();
        }
        
        collider = &entity->getComponent<ColliderComponent>();
        
        
    }
    
    void update() override
    {
        //----------------------------------
        //--------- Init variables ---------
        //----------------------------------
        
        stopX = stopY = false;
        
        Vector2D myPos = transform->position;
        Vector2D myVel = transform->velocity;
        
        SDL_Rect myCol = collider->collider;
        Vector2D lastPos = collider->lastPos;
        
        transform->grounded = false;
        
        //-------------------------
        //------- Collision -------
        //-------------------------
        
        //
        // NOTE: The collision looks wonky and doesn't work well at high speeds
        //
        
        auto& colliders(manager.getGroup(game::groupColliders));
        
        for (auto& c : colliders)
        {
            SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
            std::string cTag = c->getComponent<ColliderComponent>().tag;
            moved = false;
            
            if (Collision::AABB(cCol, myCol))
            {
                
                if (cTag == "t- block")
                {
                    physics_block(myCol, myPos, myVel, lastPos, cCol);
                }
                else if (cTag == "t- d- slab")
                {
                    
                }
                else if (cTag == "t- d- slope 1x")
                {
                    
                }
                else if (cTag == "t- d- slope 1/2x")
                {
                    
                }
                else if (cTag == "")
                {
                    
                }
                
            }
        }
        
        if (stopY)
        {
            transform->velocity.y = 0;
            transform->ySpeed = 0;
            transform->grounded = true;
        }
        else
        {
            transform->grounded = false;
        }
        
        if (stopX)
        {
            transform->velocity.x = 0;
            transform->xSpeed = 0;
        }
        
        //----------------------
        //------ Jumping -------
        //----------------------
        
        if (transform->ySpeed < .5 && (transform->velocity.y == 0 || transform->velocity.y == -1))
        {
            transform->ySpeed = 1;
            transform->velocity.y = 1;
        }
        
        if (hasGravity && !transform->grounded && transform->velocity.y > 0 && transform->ySpeed < transform->maxSpeed)
        {
            transform->ySpeed += pow((curvatureGrav / 60), 2);
            
            if (transform->ySpeed > transform->maxSpeed)
            {
                transform->ySpeed = transform->maxSpeed;
            }
        }
        else if (hasGravity && transform->velocity.y <= 0)
        {
            transform->ySpeed -= pow((curvatureGrav / 60), 2);
        }
        
    }
    
    void physics_block(SDL_Rect &myCol, Vector2D &myPos, Vector2D &myVel, Vector2D &lastPos, SDL_Rect &cCol)
    {
        int &myColX = myCol.x;
        int &myColY = myCol.y;
        
        
        logic_block(myVel.x, myColX, transform->position.x, myCol.w, myCol.h, lastPos.y, collider->xoff, cCol.x, cCol.y, cCol.w, cCol.h, stopX, true);
        myPos = transform->position;
        myCol = collider->collider;
        logic_block(myVel.y, myColY, transform->position.y, myCol.h, myCol.w, lastPos.x, collider->yoff, cCol.y, cCol.x, cCol.h, cCol.w, stopY, false);
        myPos = transform->position;
        myCol = collider->collider;
        
    }
    
    /*
     void physics_block(SDL_Rect &myCol, Vector2D &myPos, Vector2D &myVel, Vector2D &lastPos, SDL_Rect &cCol)
     {
         //If the object is moving on x-axis
         if ( myVel.x != 0 )
         {
             
             // If the object is moving right
             // If the object's left side isn't touching the collider's right
             //-
             // If the object's last position bottom side was less than the collider's top side
             // AND if the object's last position top side was above the collider's bottom side
             //-
             if (
                 myVel.x > 0 &&
                 
                 myCol.x != cCol.x + cCol.w &&
                 
                 !(lastPos.y + myCol.h <= cCol.y || lastPos.y >= cCol.y + cCol.h)
                 )
             {
                 // Then set a new position
                 transform->position.x = cCol.x - myCol.w - collider->xoff;
                 collider->collider.x = cCol.x - myCol.w;
                 
                 stopX = true;
                 moved = true;
                 
                 myPos = transform->position;
                 myCol = collider->collider;
             }
             
             // DEFAULT: If the object is moving left
             // If the object's right side isn't touching the collider's left side
             //-
             // If the object's last position bottom side was less than the collider's top side
             // AND if the object's last position top side was above the collider's bottom side
             //-
             else if (
                      myCol.x + myCol.w != cCol.x &&
                      
                      !(lastPos.y + myCol.h <= cCol.y || lastPos.y >= cCol.y + cCol.h)
                      )
             {
                 // Then set a new position
                 transform->position.x = cCol.x + cCol.w - collider->xoff;
                 collider->collider.x = cCol.x + cCol.w;
                 stopX = true;
                 moved = true;
                 
                 myPos = transform->position;
                 myCol = collider->collider;
             }
         }
         
         //If the object is moving on the y-axis
         if (
             myVel.y != 0
             )
         {
             
             // If the object is moving down
             // If the object's top side isn't touching the collider's bottom side
             //-
             // If the object's last position right side was not touching the collider's left side
             // AND if the object's last position left side was not touching the collider's right side
             //-
             if (
                 myVel.y > 0 &&
                 
                 myCol.y != cCol.y + cCol.h &&
                 
                 !(lastPos.x + myCol.w <= cCol.x || lastPos.x >= cCol.x + cCol.w)
                 )
             {
                 // Then set a new position
                 if (!moved)
                 {
                     transform->position.y = cCol.y - myCol.h - collider->yoff;
                     collider->collider.y = cCol.y - myCol.h;
                     stopY = true;
                     
                     if (entity->hasComponent<KeyboardController>())
                     {
                         entity->getComponent<KeyboardController>().ajump = false;
                     }
                     
                     myPos = transform->position;
                     myCol = collider->collider;
                 }
             }
             
             // DEFAULT: If the object is moving up
             // If the object's bottom side isn't touching the collider's top side
             //-
             // If the object's last position left side was not touching the collider's right side
             // AND if the object's last position right side was not touching the collider's left side
             //-
             else if (
                      myCol.y + myCol.h != cCol.y &&
                      
                      !(lastPos.x + myCol.w <= cCol.x || lastPos.x >= cCol.x + cCol.w)
                      )
             {
                 // Then set a new position
                 if (!moved)
                 {
                     transform->position.y = cCol.y + cCol.h - collider->yoff;
                     collider->collider.y = cCol.y + cCol.h;
                     stopY = true;
                     
                     myPos = transform->position;
                     myCol = collider->collider;
                 }
             }
         }
     }
     */
    
    void logic_block(int &myVelQ, int &myColPosQ, int &myTransfPosQ, int &myColDimQ1, int &myColDimQ2, int &myLastPosQ, int &myColQoff, int &cColPosQ1, int &cColPosQ2, int &cColDimQ1, int &cColDimQ2, bool stopQ, bool isX)
    {
        
        if ( myVelQ != 0 )
        {

            if (
                myVelQ > 0 &&
                
                myColPosQ != cColPosQ1 + cColDimQ1 &&
                
                !(myLastPosQ + myColDimQ2 <= cColPosQ2 || myLastPosQ >= cColPosQ2 + cColDimQ2)
                )
            {
                
                if (!moved)
                {
                    myTransfPosQ = cColPosQ1 - myColDimQ1 - myColQoff;
                    
                    if (!isX)
                    {
                        entity->getComponent<ColliderComponent>().collider.y = cColPosQ1 - myColDimQ1;
                        
                        if (entity->hasComponent<KeyboardController>())
                        {
                            entity->getComponent<KeyboardController>().ajump = false;
                        }
                    }
                    else
                    {
                        entity->getComponent<ColliderComponent>().collider.x  = cColPosQ1 - myColDimQ1;
                    }
                    
                    set_conditions_back(stopQ);
                    
                }
            }

            else if (
                     myColPosQ + myColDimQ1 != cColPosQ1 &&
                     
                     !(myLastPosQ + myColDimQ2 <= cColPosQ2 || myLastPosQ >= cColPosQ2 + cColDimQ2)
                     )
            {
                if (!moved)
                {
                    myTransfPosQ = cColPosQ1 + cColDimQ1 - myColQoff;
                
                    if (!isX)
                    {
                        entity->getComponent<ColliderComponent>().collider.y = cColPosQ1 - myColDimQ1;

                    }
                    else
                    {
                        entity->getComponent<ColliderComponent>().collider.x  = cColPosQ1 - myColDimQ1;
                    }
                    
                    set_conditions_back(stopQ);
                }
            }
        }
    }

    void set_conditions_back(bool &stopQ)
    {
        stopQ = true;
        moved = true;
    }

    
private:
    TransformComponent* transform;
    ColliderComponent* collider;
    
    bool stopX, stopY;
    bool moved;
    bool hasGravity = false;
    
    float curvatureGrav = 50;
    
};
