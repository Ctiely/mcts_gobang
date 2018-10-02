#ifndef MCTSTHREAD_H
#define MCTSTHREAD_H

#include <QThread>
#include <QObject>

#include "../../Board.h"
#include "../../MCTSPlayer.h"

class MCTSThread : public QThread
{
    Q_OBJECT
public:
    MCTSThread(Board & board, double c_pust=5.0, unsigned int n_playground=15000, unsigned int limit=100);
    void run();
    Board * pboard;
    MCTSPlayer * player;

signals:
    void finish(unsigned int action);

private:
    unsigned int action;
};
#endif // MCTSTHREAD_H
