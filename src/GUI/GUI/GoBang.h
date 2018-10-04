#ifndef GOBANG_H
#define GOBANG_H

#include <vector>
#include <utility>

#include <QIcon>
#include <QLabel>
#include <QWidget>
#include <QThread>
#include <QPalette>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

#include "MCTSThread.h"
#include "../../MCTS/MCTSPlayer.h"
#include "../../env/Board.h"


const int WIDTH = 700;
const int HEIGHT = 700;
const int MARGIN = 68;
const double GRID = (WIDTH - 2 * MARGIN) / (9 - 1);
const int PIECE = 60;

const int WIDTH_FULL = 540;
const int HEIGHT_FULL = 540;
const int MARGIN_FULL = 25;
const double GRID_FULL = (WIDTH_FULL - 2 * MARGIN_FULL) / (15 - 1);
const int PIECE_FULL = 35;

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
    GoBang(QWidget *parent = 0, unsigned int size=9);
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
