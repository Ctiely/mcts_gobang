#ifndef GOBANG_H
#define GOBANG_H

#include <QWidget>
#include <QLabel>
#include <QPalette>
#include <QIcon>
#include <vector>
#include <QThread>
#include <QMouseEvent>
#include <QMessageBox>
#include <utility>
#include <QPainter>
#include "MCTSThread.h"
#include "../../MCTSPlayer.h"
#include "../../Board.h"

const int WIDTH = 700;
const int HEIGHT = 700;
const int MARGIN = 68;
const int GRID = (WIDTH - 2 * MARGIN) / (9 - 1);
const int PIECE = 60;
const int EMPTY = -1;
const int BLACK = 0;
const int WHITE = 1;

class Pieces : public QLabel {
    Q_OBJECT
public:
    Pieces(QWidget * parent = 0);
};

class GoBang : public QWidget
{
    Q_OBJECT

public:
    GoBang(QWidget *parent = 0);
    ~GoBang();

    void init();

    Board board;
    QPalette * palette;
    const QPixmap * black;
    const QPixmap * white;
    std::vector<Pieces *> pieces;
    bool ai_done;
    int piece_now;
    unsigned int step;

    void mousePressEvent(QMouseEvent * e);
    void draw(int i, int j);
    pair<double, double> coordinate_transform_map2pixel(int i, int j);
    pair<int, int> coordinate_transform_pixel2map(double x, double y);
    void gameover(int winner);
    void paintEvent(QPaintEvent * e);

public slots:
    void mcts_draw(unsigned int action);

private:
    double x, y;
};
#endif // GOBANG_H
