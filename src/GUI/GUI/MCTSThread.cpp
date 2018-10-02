#include "MCTSThread.h"


MCTSThread::MCTSThread(Board & board, double c_puct, unsigned int n_playround, unsigned int limit)
    : pboard(&board) {
    player = new MCTSPlayer(c_puct, n_playround, limit);
}

void MCTSThread::run() {
    action = player->get_action(*pboard);
    emit finish(action);
}

