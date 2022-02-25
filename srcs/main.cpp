#include <iostream>
#include "game.h"
#include <SDL2/SDL.h>

int screenWidth = 1500;
int screenHeight = 900;

game *session = nullptr;

int main(int argc, char* argv[])
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    
    //int frame_count, ms_passed = 0;
    
    Uint32 frameStart;
    int frameTime;
    
    
    session = new game();
    
    session->init("Circi's Realm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 3, false);
    
    while (session->running()) {
        
        frameStart = SDL_GetTicks();
        
        session->handleEvents();
        session->update();
        session->render();
        
        frameTime = SDL_GetTicks() - frameStart;
        
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
        
        /*
        ms_passed += SDL_GetTicks() - frameStart;
        frame_count++;
        
        if (ms_passed >= 1000)
        {
            std::cout << frame_count << std::endl;
            ms_passed = 0;
            frame_count = 0;
        }
         */
        
    }
    
    session->clean();
    
    return 0;
}
