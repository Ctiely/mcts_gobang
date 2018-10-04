//
// Created by Clytie on 2018/9/8.
//

#include "MCTSPlayer.h"

MCTSPlayer::MCTSPlayer(double c_puct,
                       unsigned int n_playground,
                       unsigned int limit,
                       string name)
        : name(move(name)), mcts(c_puct, n_playground, limit) {
    srandom((unsigned int)time(nullptr));
}

void MCTSPlayer::reset() {
    mcts.update_with_action((unsigned int)-1);
}

unsigned int MCTSPlayer::get_action(Board & board) {
    unsigned int action = board.size * board.size;
    if (!board.over()) {
        action = mcts.get_action(board);
        reset();
    }
    return action;
}
