//
// Created by Clytie on 2018/9/8.
//

#include "MCTSPlayer.h"

void MCTSPlayer::reset() {
    mcts.update_with_action((unsigned int)-1);
}

unsigned int MCTSPlayer::get_action(Board & board) {
    unsigned int action = board.size * board.size;
    if (!board.over() && board.spaces > 0) {
        action = mcts.get_action(board);
        mcts.update_with_action((unsigned int)-1);
        //mcts.update_with_action(action);
    }
    return action;
}
