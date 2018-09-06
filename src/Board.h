//
// Created by Clytie on 2018/9/6.
//

#ifndef MCTS_GOBANG_BOARD_H
#define MCTS_GOBANG_BOARD_H

#include <list>
#include <vector>
#include <unordered_map>
using namespace std;

class Board {
public:
    explicit Board(unsigned int boardsize)
        : size(boardsize) {
        reset();
    };
    ~Board() = default;

    void reset();
    void step(unsigned int action);
    unsigned int current_player();
    bool over();
    void render();

    unsigned int size;
    unsigned int spaces;
    unsigned int round;
    unordered_map<unsigned int, unsigned int> states;
    vector<vector<vector<unsigned int> > > moves;
    list<unsigned int> availables;
    unsigned int last_action;
    int winner;
private:
    struct Coordinate {
        Coordinate(unsigned int move, unsigned int size) {
            row = move / size;
            col = move % size;
        }
        unsigned int row;
        unsigned int col;
    };

    void check(Coordinate & cur);

    bool gameover;
};


#endif //MCTS_GOBANG_BOARD_H
