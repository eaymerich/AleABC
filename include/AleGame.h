#ifndef ALEGAME_H
#define ALEGAME_H

#include <string>
#include <SDL2/SDL.h>

class AleGame {
public:
    AleGame();
    virtual ~AleGame();
    void start();
private:
    void initSDL();
    void destroySDL();
    SDL_Texture* loadTexture(std::string path);

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool goon = true;
};

#endif // ALEGAME_H
