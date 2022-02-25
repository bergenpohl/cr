#ifndef game_h
#define game_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

class ColliderComponent;

static int scWidth;
static int scHeight;


class game
{
public:
    game();
    ~game();
    
    void init(const char *title, int xpos, int ypos, int width, int height, int sc, bool fullscreen);
    
    void handleEvents();
    void update();
    void render();
    void clean();
    
    bool running();
    
    int scale;
    int mapWidth;
    int mapHeight;
    
    static int jumpSpeed;
    
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static bool isRunning;
    static SDL_Rect camera;
    
    enum groupLabels : std::size_t
    {
        groupTiles,
        groupMap,
        groupPlayers,
        groupBackground,
        groupClouds,
        groupLayer1,
        groupLayer2,
        groupLayer3,
        groupLayer4,
        groupLayer5,
        groupLayer6,
        groupLayer7,
        //groupEnemies,
        groupColliders
    };
    
    
private:
    SDL_Window *window;
};


#endif
