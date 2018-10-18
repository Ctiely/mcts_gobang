#ifndef GOBANG_MCTS_H
#define GOBANG_MCTS_H

#include <QWidget>
#include <QTimer>
#include "GoBang.h"


class gobang_mcts : public QWidget
{
    Q_OBJECT
public:
    explicit gobang_mcts(QWidget *parent = nullptr, bool alphago_play=false, std::string model_path="");
    ~gobang_mcts() = default;
    void init();
    void play();
    void ai_draw(unsigned int action);
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
    MCTSPlayer * mcts;
    MCTSPlayer_alphago_zero * alphago;
    bool alphago_play;
    string model_path;
    
public slots:
    void mcts_move();
    void alphago_move();
};

#endif // GOBANG_MCTS_H
