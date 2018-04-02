#include <iostream>
#include "AleGame.h"

int main() {
    AleGame aleGame;
    try {
        aleGame.start();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
