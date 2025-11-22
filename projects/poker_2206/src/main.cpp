#include "Game.h"
#include <iostream>

int main() {
    try {
        Poker::Game game;
        game.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}