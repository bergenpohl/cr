#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
    SDL_Surface* tempSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    
    return tex;
    
}

//SDL_Texture* TextureManager::AssignTexture(SDL_Texture* texture)
//{
//    SDL_Texture* tex = texture;
//
//    return tex;
//
//}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(game::renderer, tex, &src, &dest, NULL, NULL, flip);
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect *src, SDL_Rect dest, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(game::renderer, tex, src, &dest, NULL, NULL, flip);
}



