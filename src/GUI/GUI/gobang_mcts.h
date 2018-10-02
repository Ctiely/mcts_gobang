#ifndef GOBANG_MCTS_H
#define GOBANG_MCTS_H

#include <QWidget>
#include <QTimer>
#include "GoBang.h"


class gobang_mcts : public QWidget
{
    Q_OBJECT
public:
    explicit gobang_mcts(QWidget *parent = nullptr);
    ~gobang_mcts();
    void init();
    void play();
    void mcts_draw(unsigned int action);
    void draw(int i, int j);
    void gameover(int winner);
    pair<double, double> coordinate_transform_map2pixel(int i, int j);

    Board board;
    QPalette * palette;
    const QPixmap * black;
    const QPixmap * white;
    std::vector<Pieces *> pieces;
    int piece_now;
    unsigned int step;

private:
    QTimer * timer;
    MCTSPlayer * player;

public slots:
    void move();
};

#endif // GOBANG_MCTS_H
