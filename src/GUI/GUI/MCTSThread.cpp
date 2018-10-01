#include "MCTSThread.h"


MCTSThread::MCTSThread(Board & board)
    : pboard(&board) {
    player = new MCTSPlayer(5, 15000, 100);
}

void MCTSThread::run() {
    action = player->get_action(*pboard);
    emit finish(action);
}

