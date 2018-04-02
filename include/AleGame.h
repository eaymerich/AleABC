#ifndef ALEGAME_H
#define ALEGAME_H

#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class AleGame {
public:
    AleGame();
    virtual ~AleGame();
    void start();
private:
    void initSDL();
    void destroySDL();
    void loadTextures();
    void unloadTextures();
    SDL_Texture* loadTexture(std::string path);
    SDL_Texture* renderGlyph(TTF_Font* font, uint16_t letter);
    void gameLoop();
    void processInput();

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool goon = true;
    uint16_t selected_letter = 'L';
    std::unordered_map<uint16_t, SDL_Texture*> animal_texture;
    std::unordered_map<uint16_t, SDL_Texture*> letter_texture;
};

#endif // ALEGAME_H
