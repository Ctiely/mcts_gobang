#ifndef MCTSTHREAD_H
#define MCTSTHREAD_H

#include <QThread>
#include <QObject>

#include "../../env/Board.h"
#include "../../MCTS/MCTSPlayer.h"
#include "../../MCTS_alphago_zero/MCTSPlayer_alphago_zero.h"

class MCTSThread : public QThread
{
    Q_OBJECT
public:
    MCTSThread(Board & board,
               double c_pust=5.0,
               unsigned int n_playground=15000,
               unsigned int limit=100);
    void run();
    Board * pboard;
    MCTSPlayer * player;

signals:
    void finish(unsigned int action);

private:
    unsigned int action;
};

class AlphaGoThread : public QThread
{
    Q_OBJECT
public:
    AlphaGoThread(Board & board,
                  string model_path,
                  unsigned int size,
                  double c_pust=5.0,
                  unsigned int n_playground=10000);
    void run();
    Board * pboard;
    MCTSPlayer_alphago_zero * player;

signals:
    void finish(unsigned int action);

private:
    unsigned int action;
};
#endif // MCTSTHREAD_H
