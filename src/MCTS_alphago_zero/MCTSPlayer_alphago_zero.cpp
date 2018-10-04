//
// Created by Clytie on 2018/10/4.
//

#include "MCTSPlayer_alphago_zero.h"

MCTSPlayer_alphago_zero::MCTSPlayer_alphago_zero(const string & model_path,
                                                 unsigned int size,
                                                 double c_pust,
                                                 unsigned int n_playround,
                                                 string name)
        : mcts_alphago_zero(model_path, size, c_pust, n_playround), name(move(name)) {
    srandom((unsigned int)time(nullptr));
}

void MCTSPlayer_alphago_zero::reset() {
    mcts_alphago_zero.update_with_action((unsigned int)-1);
}

unsigned int MCTSPlayer_alphago_zero::get_action(Board & board, float tau) {
    unsigned int action = board.size * board.size;
    if (!board.over()) {
        pair<vector<unsigned int>, vector<float> > actions_probs =  mcts_alphago_zero.get_action_probs(board, tau);
        action = utils::sampling<unsigned int>(actions_probs.first, actions_probs.second);
        reset();
    }
    return action;
}