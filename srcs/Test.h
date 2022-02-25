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


