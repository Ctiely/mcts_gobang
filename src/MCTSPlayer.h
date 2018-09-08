//
// Created by Clytie on 2018/9/8.
//

#ifndef MCTS_GOBANG_MCTS_PLAYER_H
#define MCTS_GOBANG_MCTS_PLAYER_H

#include <string>
#include "MCTS.h"
using namespace std;

class MCTSPlayer {
public:
    explicit MCTSPlayer(double c_puct=sqrt(2), unsigned int n_playground=5000, unsigned int limit=1000, string name="MCTSPlayer")
            : name(move(name)), mcts(c_puct, n_playground, limit) {
        srandom((unsigned int)time(nullptr));
    }

    void reset();
    unsigned int get_action(Board & board);
private:
    MCTS mcts;
    string name;
};


#endif //MCTS_GOBANG_MCTS_PLAYER_H
