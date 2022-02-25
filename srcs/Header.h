{
    if (
        myVel.x != 0
        )
    {
        if (
            myVel.x > 0 &&
            
            myCol.x != cCol.x + cCol.w &&
            
            !(lastPos.y + myCol.h <= cCol.y || lastPos.y >= cCol.y + cCol.h)
            )
        {
            if (!moved)
            {
                transform->position.x = cCol.x - myCol.w - collider->xoff;
                myCol.x = cCol.x - myCol.w;
                if (!true)
                {
                    if (entity->hasComponent<KeyboardController>())
                    {
                        entity->getComponent<KeyboardController>().ajump = false;
                    }
                }
                set_conditions_back(stopX);
            }
        }
        else if (
                 myCol.x + myCol.w != cCol.x &&
                 !(lastPos.y + myCol.h <= cCol.y || lastPos.y >= cCol.y + cCol.h)
                 )
        {
            if (!moved)
            {
                transform->position.x = cCol.x + cCol.w - collider->xoff;
                myCol.x = cCol.x + cCol.w;
                set_conditions_back(stopX);
            }
        }
    }
}








//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

//If the object is moving on x-axis
if ( myVel.x != 0 )
{
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

