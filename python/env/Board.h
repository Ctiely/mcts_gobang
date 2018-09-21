//
// Created by Clytie on 2018/9/6.
//

#ifndef MCTS_GOBANG_BOARD_H
#define MCTS_GOBANG_BOARD_H

#include <list>
#include <vector>
#include <map>
using namespace std;

class Board {
public:
    explicit Board(int boardsize, int win_n=5)
            : size(boardsize), win_n(win_n) {
        reset();
    };
    ~Board() = default;

    void reset();
    void step(int action);
    void step(int row, int col);
    int current_player();
    bool over();
    void render();

    Board copy();

    int size;
    int win_n;
    int spaces;
    int round;
    map<int, int> states;
    vector<vector<vector<int> > > moves;
    list<int> availables;
    int last_action;
    int winner;
    bool gameover;
private:
    struct Coordinate {
        Coordinate(int move, int size) {
            row = move / size;
            col = move % size;
        }
        int row;
        int col;
    };

    void check(Coordinate & cur);
};


#endif //MCTS_GOBANG_BOARD_H
