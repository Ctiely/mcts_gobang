#include "MCTSThread.h"


MCTSThread::MCTSThread(Board & board,
                       double c_puct,
                       unsigned int n_playround,
                       unsigned int limit)
    : pboard(&board) {
    player = new MCTSPlayer(c_puct, n_playround, limit);
}

void MCTSThread::run() {
    action = player->get_action(*pboard);
    emit finish(action);
}

AlphaGoThread::AlphaGoThread(Board & board,
                             string model_path,
                             unsigned int size,
                             double c_pust,
                             unsigned int n_playground)
        : pboard(&board) {
    player = new MCTSPlayer_alphago_zero(model_path, size, c_pust, n_playground);
}

void AlphaGoThread::run() {
    action = player->get_action(*pboard);
    emit finish(action);
}