#include "game.h"
#include "Map.h"
#include "TextureManager.h"
#include "Collision.h"

#include "KeyboardController.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "TileComponent.h"
#include "PhysicsComponent.h"

#include "Vector2D.h"
#include <sstream>


Map* map;
Manager manager;

SDL_Renderer* game::renderer = nullptr;
SDL_Event game::event;

SDL_Rect game::camera = {0, 0, 100, 100};

bool game::isRunning = false;

int game::jumpSpeed = 10;


auto& player(manager.addEntity());
auto& sky(manager.addEntity());
auto& mountains(manager.addEntity());

auto& cloud1(manager.addEntity());
auto& cloud2(manager.addEntity());
auto& cloud3(manager.addEntity());
auto& cloud4(manager.addEntity());
auto& cloud5(manager.addEntity());


game::game()
{
}


game::~game()
{
}

void game::init(const char *title, int xpos, int ypos, int width, int height, int sc, bool fullscreen)
{
    
    if (fullscreen)
    {
        width = 2560 / 2;
        height = 1600 / 2;
    }
    
    int flags = 0;
    SDL_Rect playerData = {924 * sc, 773 * sc, 32, 64};
    
    scale = sc;
    
    scWidth = camera.w = width;
    scHeight = camera.h = height;
    
    mapWidth = 100;
    mapHeight = 50;
    
    
    
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    
    //std::cout << SDL_Init(SDL_INIT_EVERYTHING) << std::endl;
    
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        
        window = SDL_CreateWindow(title, xpos, ypos, scWidth, scHeight, flags);
        
        renderer = SDL_CreateRenderer(window, -1, 0);
        
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        
        isRunning = true;
    }
    
    map = new Map("assets/world_tileset.png", "assets/world_tileset_info.txt", scale);

    map->LoadMap("maps/colMap.txt", "maps/tileMap.txt");
    
    mapWidth = mapWidth * 32 * scale;
    mapHeight = mapHeight * 32 * scale;
    
    player.addComponent<TransformComponent>(playerData.x, playerData.y, playerData.w, playerData.h, scale);
    player.addComponent<ColliderComponent>("player", playerData.x, playerData.y, playerData.w, playerData.h, 6, 8, -12, -8, scale, false);
    player.addComponent<PhysicsComponent>(true);
    player.addComponent<SpriteComponent>("assets/char_animations.png", true);
    player.addComponent<KeyboardController>();
    player.addGroup(groupPlayers);
    
    sky.addComponent<TransformComponent>(0, 0, 1, 500, scale);
    sky.getComponent<TransformComponent>().width = camera.w;
    sky.addComponent<SpriteComponent>("assets/sky.png");
    sky.addGroup(groupBackground);
    
    mountains.addComponent<TransformComponent>(0, 800, 1172, 189, scale);
    mountains.addComponent<SpriteComponent>("assets/mountains.png");
    mountains.addGroup(groupBackground);
    
    cloud1.addComponent<TransformComponent>(10 * 32 * scale, camera.h / 4, 176, 46, scale);
    cloud1.addComponent<SpriteComponent>("assets/cloud1.png");
    cloud1.addGroup(groupClouds);
    
    cloud2.addComponent<TransformComponent>(5 * 32 * scale, camera.h / 5, 65, 13, scale);
    cloud2.addComponent<SpriteComponent>("assets/cloud2.png");
    cloud2.addGroup(groupClouds);
    
    cloud3.addComponent<TransformComponent>(20 * 32 * scale, camera.h / 6, 100, 23, scale);
    cloud3.addComponent<SpriteComponent>("assets/cloud3.png");
    cloud3.addGroup(groupClouds);
    
    cloud4.addComponent<TransformComponent>(15 * 32 * scale, camera.h / 7, 118, 22, scale);
    cloud4.addComponent<SpriteComponent>("assets/cloud4.png");
    cloud4.addGroup(groupClouds);
    
    cloud5.addComponent<TransformComponent>(28 * 32 * scale, camera.h / 8, 68, 14, scale);
    cloud5.addComponent<SpriteComponent>("assets/cloud5.png");
    cloud5.addGroup(groupClouds);
    
}

//auto& grass(manager.getGroup(game::groupGrass));
auto& players(manager.getGroup(game::groupPlayers));
auto& colliders(manager.getGroup(game::groupColliders));

auto& background(manager.getGroup(game::groupBackground));
auto& clouds(manager.getGroup(game::groupClouds));
auto& layer1(manager.getGroup(game::groupLayer1));
auto& layer2(manager.getGroup(game::groupLayer2));
auto& layer3(manager.getGroup(game::groupLayer3));
auto& layer4(manager.getGroup(game::groupLayer4));
auto& layer5(manager.getGroup(game::groupLayer5));
auto& layer6(manager.getGroup(game::groupLayer6));
auto& layer7(manager.getGroup(game::groupLayer7));

void game::handleEvents()
{
    
    SDL_PollEvent(&event);
    
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
            
        default:
            break;
    }
}

void game::update()
{
    player.getComponent<KeyboardController>().playerMovement();
    SDL_Rect playerCol;
    Vector2D playerPos, playerVel, lastPos;
    
    playerCol = player.getComponent<ColliderComponent>().collider;
    playerPos = player.getComponent<TransformComponent>().position;
    playerVel = player.getComponent<TransformComponent>().velocity;
    
    lastPos = player.getComponent<ColliderComponent>().lastPos;
    
    
    /*
     for (auto& c : clouds)
     {
     c->getComponent<TransformComponent>().position.y = camera.y + c->getComponent<TransformComponent>().startPosition.y - (player.getComponent<TransformComponent>().position.y / 30);
     }
     
     mountains.getComponent<TransformComponent>().position.y = camera.y + (camera.h / 4) - (player.getComponent<TransformComponent>().position.y / 15);
     */
    
    
    
    camera.x = player.getComponent<TransformComponent>().position.x - (scWidth / 2) + (playerCol.w / 2);
    camera.y = player.getComponent<TransformComponent>().position.y - (scHeight / 2) + (playerCol.h / 2);
    
    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x + camera.w > mapWidth)
        camera.x = mapWidth - camera.w;
    if (camera.y + camera.h > mapHeight)
        camera.y = mapHeight - camera.h;
    
    //make the program set this once, not every frame
    sky.getComponent<TransformComponent>().position.x = camera.x;
    //sky.position.y is set below
    
    if (camera.x != 0 && camera.x != mapWidth - camera.w)
    {
        for (auto& c : clouds)
        {
            c->getComponent<TransformComponent>().position.x = camera.x + c->getComponent<TransformComponent>().startPosition.x - (player.getComponent<TransformComponent>().position.x / 5);
        }
        
        mountains.getComponent<TransformComponent>().position.x = camera.x - (player.getComponent<TransformComponent>().position.x / 3);
    }
    else
    {
        if (camera.x == 0)
        {
            for (auto& c : clouds)
            {
                c->getComponent<TransformComponent>().position.x = c->getComponent<TransformComponent>().startPosition.x - (((scWidth / 2) - (playerCol.w / 2)) / 5);
            }
            
            mountains.getComponent<TransformComponent>().position.x = -(((scWidth / 2) - (playerCol.w / 2)) / 3);
        }
        else
        {
            for (auto& c : clouds)
            {
                c->getComponent<TransformComponent>().position.x = camera.x + c->getComponent<TransformComponent>().startPosition.x - ((mapWidth - (scWidth / 2) - (playerCol.w / 2)) / 5);
            }
            
            mountains.getComponent<TransformComponent>().position.x = camera.x - ((mapWidth - (scWidth / 2) - (playerCol.w / 2)) / 3);
        }
    }
    
    if (camera.y != 0 && camera.y != mapHeight - camera.h)
    {
        sky.getComponent<TransformComponent>().position.y = camera.y - (player.getComponent<TransformComponent>().position.y / 20) - (32 * scale);
        
        for (auto& c : clouds)
        {
            c->getComponent<TransformComponent>().position.y = camera.y + c->getComponent<TransformComponent>().startPosition.y - (player.getComponent<TransformComponent>().position.y / 30);
        }
        
        mountains.getComponent<TransformComponent>().position.y = mountains.getComponent<TransformComponent>().startPosition.y + camera.y + (camera.h / 4) - (player.getComponent<TransformComponent>().position.y / 3);
    }
    else
    {
        if (camera.y == 0)
        {
            sky.getComponent<TransformComponent>().position.y = camera.y - (((scHeight / 2) - (playerCol.h / 2)) / 20) - (32 * scale);
            
            for (auto& c : clouds)
            {
                c->getComponent<TransformComponent>().position.y = c->getComponent<TransformComponent>().startPosition.y - (((scHeight / 2) - (playerCol.h / 2)) / 30);
            }
            
            mountains.getComponent<TransformComponent>().position.y = mountains.getComponent<TransformComponent>().startPosition.y + (camera.h / 4) - (((scHeight / 2) - (playerCol.h / 2)) / 3);
        }
        else
        {
            sky.getComponent<TransformComponent>().position.y = camera.y - ((mapHeight - (scHeight / 2) - (playerCol.h / 2)) / 20) - (32 * scale);
            
            for (auto& c : clouds)
            {
                c->getComponent<TransformComponent>().position.y = camera.y + c->getComponent<TransformComponent>().startPosition.y - ((mapHeight - (scHeight / 2) - (playerCol.h / 2)) / 30);
            }
            
            mountains.getComponent<TransformComponent>().position.y = mountains.getComponent<TransformComponent>().startPosition.y + camera.y + (camera.h / 4) - ((mapHeight - (scHeight / 2) - (playerCol.h / 2)) / 3);
        }
    }
    
    manager.refresh();
    manager.update();
    
    /*
     Vector2D pVel = player.getComponent<TransformComponent>().velocity;
     int pSpeed = player.getComponent<TransformComponent>().speed;
     
     for (auto t : tiles)
     {
     t->getComponent<TileComponent>().destRect.x += -(pVel.x * pSpeed);
     t->getComponent<TileComponent>().destRect.y += -(pVel.y * pSpeed);
     }
     
     for (auto cc : colliders)
     {
     Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
     
     }
     */
    

}

void game::render()
{
    SDL_RenderClear(renderer);
    
    /*
     for (autp& g : grass)
     {
     g->draw();
     }
     */
    
    sky.draw();
    
    for (auto& c : clouds)
    {
        c->draw();
    }
    
    mountains.draw();
    
    for (auto& l1 : layer1)
    {
        if (
            l1->getComponent<TileComponent>().position.x + l1->getComponent<TileComponent>().destRect.w >= camera.x && l1->getComponent<TileComponent>().position.x < camera.x + camera.w &&
            l1->getComponent<TileComponent>().position.y + l1->getComponent<TileComponent>().destRect.h >= camera.y && l1->getComponent<TileComponent>().position.y < camera.y + camera.h
            )
        {
            //std::cout << "Calling draw() function" << std::endl;
            l1->draw();
        }
    }
    
    for (auto& l2 : layer2)
    {
        if (
            l2->getComponent<TileComponent>().position.x + l2->getComponent<TileComponent>().destRect.w >= camera.x && l2->getComponent<TileComponent>().position.x < camera.x + camera.w &&
            l2->getComponent<TileComponent>().position.y + l2->getComponent<TileComponent>().destRect.h >= camera.y && l2->getComponent<TileComponent>().position.y < camera.y + camera.h
            )
        {
            l2->draw();
        }
    }
    
    for (auto& l3 : layer3)
    {
        if (
            l3->getComponent<TileComponent>().position.x + l3->getComponent<TileComponent>().destRect.w >= camera.x && l3->getComponent<TileComponent>().position.x < camera.x + camera.w &&
            l3->getComponent<TileComponent>().position.y + l3->getComponent<TileComponent>().destRect.h >= camera.y && l3->getComponent<TileComponent>().position.y < camera.y + camera.h
            )
        {
            l3->draw();
        }
    }
    
    for (auto& l4 : layer4)
    {
        if (
            l4->getComponent<TileComponent>().position.x + l4->getComponent<TileComponent>().destRect.w >= camera.x && l4->getComponent<TileComponent>().position.x < camera.x + camera.w &&
            l4->getComponent<TileComponent>().position.y + l4->getComponent<TileComponent>().destRect.h >= camera.y && l4->getComponent<TileComponent>().position.y < camera.y + camera.h
            )
        {
            l4->draw();
        }
    }
    
    
    for (auto& p : players)
    {
        p->draw();
    }
    
    for (auto& l5 : layer5)
    {
        if (
            l5->getComponent<TileComponent>().position.x + l5->getComponent<TileComponent>().destRect.w >= camera.x && l5->getComponent<TileComponent>().position.x < camera.x + camera.w &&
            l5->getComponent<TileComponent>().position.y + l5->getComponent<TileComponent>().destRect.h >= camera.y && l5->getComponent<TileComponent>().position.y < camera.y + camera.h
            )
        {
            l5->draw();
        }
    }
    
    for (auto& l6 : layer6)
    {
        if (
            l6->getComponent<TileComponent>().position.x + l6->getComponent<TileComponent>().destRect.w >= camera.x && l6->getComponent<TileComponent>().position.x < camera.x + camera.w &&
            l6->getComponent<TileComponent>().position.y + l6->getComponent<TileComponent>().destRect.h >= camera.y && l6->getComponent<TileComponent>().position.y < camera.y + camera.h
            )
        {
            l6->draw();
        }
    }
    
    for (auto& l7 : layer7)
    {
        if (
            l7->getComponent<TileComponent>().position.x + l7->getComponent<TileComponent>().destRect.w >= camera.x && l7->getComponent<TileComponent>().position.x < camera.x + camera.w &&
            l7->getComponent<TileComponent>().position.y + l7->getComponent<TileComponent>().destRect.h >= camera.y && l7->getComponent<TileComponent>().position.y < camera.y + camera.h
            )
        {
            l7->draw();
        }
    }
    
    for (auto& c : colliders)
    {
        c->draw();
    }
    
    SDL_RenderPresent(renderer);
}

void game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    
}

bool game::running()
{
    return isRunning;
}
