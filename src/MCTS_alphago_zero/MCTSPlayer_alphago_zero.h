//
// Created by Clytie on 2018/10/4.
//

#ifndef MCTS_GOBANG_MCTSPLAYER_ALPHAGO_ZERO_H
#define MCTS_GOBANG_MCTSPLAYER_ALPHAGO_ZERO_H

#include "../utils.h"
#include "MCTS_alphago_zero.h"
using namespace std;

class MCTSPlayer_alphago_zero {
public:
    explicit MCTSPlayer_alphago_zero(const string & model_path,
                                     unsigned int size,
                                     double c_pust=5,
                                     unsigned int n_playround=10000,
                                     string name="MCTSPlayer_alphago_zero");
    ~MCTSPlayer_alphago_zero() = default;

    string name;
    void reset();
    unsigned int get_action(Board & board, float tau=1e-3);
private:
    MCTS_alphago_zero mcts_alphago_zero;
};


#endif //MCTS_GOBANG_MCTSPLAYER_ALPHAGO_ZERO_H
