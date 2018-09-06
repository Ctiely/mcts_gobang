#include <iostream>
#include "src/Board.h"

int main() {
    Board game(10);
    game.step(0);
    game.step(1);
    game.step(10);
    game.step(2);
    game.step(20);
    game.step(3);
    game.step(30);
    game.step(4);
    game.step(11);
    game.step(12);
    game.step(22);
    game.step(21);
    game.step(31);
    game.step(33);
    game.step(44);
    game.step(5);
    cout << game.over() << endl;
    game.render();
    return 0;
}