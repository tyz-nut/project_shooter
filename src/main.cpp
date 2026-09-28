#include <iostream>
#include "game.h"

int main(int, char **)
{
    Game& game = Game::getInstance();
    game.init();
    game.run();
    game.clean();
    return 0; 
}