#include "MCTSThread.h"


MCTSThread::MCTSThread(Board & board, unsigned int n_playround, unsigned int limit)
    : pboard(&board) {
    player = new MCTSPlayer(5, n_playround, limit);
}

void MCTSThread::run() {
    action = player->get_action(*pboard);
    emit finish(action);
}

