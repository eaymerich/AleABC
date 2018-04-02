#include <cctype>
#include <iostream>

#include "AleGame.h"
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 600

#define N_TILDE_LOWER 241
#define N_TILDE_UPPER 209

using std::cout;
using std::endl;

AleGame::AleGame() {}

AleGame::~AleGame() {
    unloadTextures();
    destroySDL();
    cout << "End AleGame." << endl;
}

void AleGame::start() {
    cout << "Start AleGame!" << endl;
    initSDL();
    loadTextures();
    gameLoop();
}

void AleGame::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        throw std::runtime_error("Unable to initialize SDL");
    }

    window = SDL_CreateWindow("AleABC",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              //SDL_WINDOW_FULLSCREEN);
                              SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        throw std::runtime_error("Unable to create window");
    }

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        throw std::runtime_error("Unable to create renderer");
    }

    int img_flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(img_flags);
    if ((initted & img_flags) != img_flags) {
        throw std::runtime_error("Unable to initialize SDL2_image library");
    }

    if (TTF_Init() != 0) {
        throw std::runtime_error("Unable to initialize SDL2_ttf library");
    }
}

void AleGame::destroySDL() {
    TTF_Quit();
    IMG_Quit();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void AleGame::loadTextures() {
    // Load Animal Textures
    animal_texture['A'] = loadTexture("./images/ardilla.png");
    animal_texture['B'] = loadTexture("./images/buho.jpg");
    animal_texture['C'] = loadTexture("./images/caballo.jpg");
    animal_texture['D'] = loadTexture("./images/danta.jpg");
    animal_texture['E'] = loadTexture("./images/elefante.jpg");
    animal_texture['F'] = loadTexture("./images/foca.jpg");
    animal_texture['G'] = loadTexture("./images/gato.jpg");
    animal_texture['H'] = loadTexture("./images/hamster.jpg");
    animal_texture['I'] = loadTexture("./images/iguana.jpg");
    animal_texture['J'] = loadTexture("./images/jaguar.jpg");
    animal_texture['K'] = loadTexture("./images/koala.jpg");
    animal_texture['L'] = loadTexture("./images/leon.jpg");
    animal_texture['M'] = loadTexture("./images/manati.jpg");
    animal_texture['N'] = loadTexture("./images/nutria.jpg");
    animal_texture[N_TILDE_UPPER] = loadTexture("./images/nu.jpg");
    animal_texture['O'] = loadTexture("./images/oso.jpg");
    animal_texture['P'] = loadTexture("./images/panda.jpg");
    animal_texture['Q'] = loadTexture("./images/quetzal.jpg");
    animal_texture['R'] = loadTexture("./images/raton.jpg");
    animal_texture['S'] = loadTexture("./images/sapo.jpg");
    animal_texture['T'] = loadTexture("./images/tigre.jpg");
    animal_texture['U'] = loadTexture("./images/urraca.jpg");
    animal_texture['V'] = loadTexture("./images/vaca.jpg");
    animal_texture['W'] = loadTexture("./images/wombat.jpg");
    animal_texture['X'] = loadTexture("./images/xifoforo.jpg");
    animal_texture['Y'] = loadTexture("./images/yak.jpg");
    animal_texture['Z'] = loadTexture("./images/zorro.png");

    // Load Letter Textures
    TTF_Font* font = TTF_OpenFont("./fonts/Roboto-Black.ttf", 512);
    if (font == nullptr) {
        throw std::runtime_error("Unable to open font");
    }

    for (uint16_t letter = 'A'; letter <= 'Z'; letter++) {
        letter_texture[letter] = renderGlyph(font, letter);
    }
    letter_texture[N_TILDE_UPPER] = renderGlyph(font, N_TILDE_UPPER);

    TTF_CloseFont(font);
}

void AleGame::unloadTextures() {
    for (auto element : animal_texture) {
        SDL_DestroyTexture(element.second);
    }
    animal_texture.clear();
    for (auto element : letter_texture) {
        SDL_DestroyTexture(element.second);
    }
    letter_texture.clear();
}

SDL_Texture* AleGame::loadTexture( std::string path ) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr) {
        throw std::runtime_error("Error loading texture " + path);
    }
    return texture;
}

SDL_Texture* AleGame::renderGlyph(TTF_Font* font, uint16_t letter) {
    SDL_Color color = SDL_Color{255,255,255,255};
    SDL_Surface* surface = TTF_RenderGlyph_Blended(font, letter, color);
    if (surface == nullptr) {
        throw std::runtime_error("Unable to render Glyph into surface");
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
        throw std::runtime_error("Unable to create texture from surface");
    }

    SDL_FreeSurface(surface);
    return texture;
}

void AleGame::gameLoop() {
    while (goon) {
        // Read input
        processInput();

        // Render
        SDL_RenderClear(renderer);

        // Render animal
        SDL_Texture *tex = animal_texture[selected_letter];
        int w, h;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        float ratio = (float)w / (float)h;
        SDL_Rect dst;
        dst.w = SCREEN_WIDTH / 2;
        dst.h = (int)(dst.w / ratio);
        dst.x = (SCREEN_WIDTH / 2) + ((SCREEN_WIDTH / 2) - dst.w) / 2;
        dst.y = (SCREEN_HEIGHT - dst.h) / 2;
        SDL_RenderCopy(renderer, tex, nullptr, &dst);

        // Render letter
        tex = letter_texture[selected_letter];
        SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
        dst.x = ((SCREEN_WIDTH / 2) - dst.w) / 2;
        dst.y = (SCREEN_HEIGHT - dst.h) / 2;
        SDL_RenderCopy(renderer, tex, nullptr, &dst);

        SDL_RenderPresent(renderer);
    }
}

void AleGame::processInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            goon = false;
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                goon = false;
            } else if (e.key.keysym.sym == N_TILDE_LOWER) {
                selected_letter = N_TILDE_UPPER;
            } else if (e.key.keysym.sym >= SDLK_a &&
                      e.key.keysym.sym <= SDLK_z) {
                selected_letter = toupper(e.key.keysym.sym);
            }
        }
    }
}
