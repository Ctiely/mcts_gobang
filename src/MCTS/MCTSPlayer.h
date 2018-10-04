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
    explicit MCTSPlayer(double c_puct=sqrt(2),
                        unsigned int n_playground=10000,
                        unsigned int limit=100,
                        string name="MCTSPlayer");

    string name;
    void reset();
    unsigned int get_action(Board & board);
private:
    MCTS mcts;
};


#endif //MCTS_GOBANG_MCTS_PLAYER_H
