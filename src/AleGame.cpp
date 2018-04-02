#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "AleGame.h"

using std::cout;
using std::endl;

AleGame::AleGame() {}

AleGame::~AleGame() {
    SDL_Delay(3000);
    destroySDL();
    cout << "End AleGame." << endl;
}

void AleGame::start() {
    cout << "Start AleGame!" << endl;
    initSDL();

    SDL_Texture *tex = loadTexture("./images/leon.jpg");

    SDL_RenderClear(renderer);
    // Render animal
    SDL_Rect dst;
    dst.x = 512;
    dst.y = 0;
    dst.w = 512;
    dst.h = 600;
    SDL_RenderCopy(renderer, tex, nullptr, &dst);
    SDL_RenderPresent(renderer);
}

void AleGame::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        throw std::runtime_error("Lol");
    }

    window = SDL_CreateWindow("AleABC",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              640, 480,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        throw std::runtime_error("Lol");
    }

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        throw std::runtime_error("Lol");
    }

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
}

void AleGame::destroySDL() {
    IMG_Quit();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* AleGame::loadTexture( std::string path ) {
    //The final texture
    SDL_Texture* newTexture = NULL;
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL ) {
        printf("Unable to load image %s! SDL_image Error: %s\n",
               path.c_str(),
               IMG_GetError());
    } else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL ) {
            printf("Unable to create texture from %s! SDL Error: %s\n",
                   path.c_str(),
                   SDL_GetError() );
        }
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}
