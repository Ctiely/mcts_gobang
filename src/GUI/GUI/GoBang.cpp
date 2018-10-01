#include "GoBang.h"


Pieces::Pieces(QWidget * parent)
    : QLabel(parent) {
    setMouseTracking(true);
    setVisible(true);
    setScaledContents(true);
}

GoBang::GoBang(QWidget *parent)
    : QWidget(parent), board(9, 5), piece_now(BLACK), step(0), x(0), y(0)
{
    init();
}

GoBang::~GoBang()
{

}
void GoBang::init() {
    palette = new QPalette;
    palette->setBrush(QPalette::Background,QBrush(QPixmap("/Users/clytie/Documents/mcts_gobang/src/GUI/img/board.png")));
    setPalette(*palette);
    setCursor(Qt::PointingHandCursor);
    resize(WIDTH, HEIGHT);
    setMinimumSize(QSize(WIDTH, HEIGHT));
    setMaximumSize(QSize(WIDTH, HEIGHT));

    setWindowTitle("GoBang");
    setWindowIcon(QIcon("/Users/clytie/Documents/mcts_gobang/src/GUI/img/black.png"));

    black = new QPixmap("/Users/clytie/Documents/mcts_gobang/src/GUI/img/black.png");
    white = new QPixmap("/Users/clytie/Documents/mcts_gobang/src/GUI/img/white.png");
    for (unsigned int i = 0; i < board.size * board.size; ++i) {
        pieces.push_back(new Pieces(this));
    }
    ai_done = true;
}

void GoBang::mousePressEvent(QMouseEvent * e) {
    if (e->button() == Qt::LeftButton && ai_done) {
        double x = e->x();
        double y = e->y();
        pair<int, int> action = coordinate_transform_pixel2map(x, y);
        int i = action.first, j = action.second;
        if (i != -1 && j != -1 && board.moves[0][i][j] == 0 && board.moves[1][i][j] == 0) {
            draw(i, j);
            ai_done = false;
            MCTSThread * mcts = new MCTSThread(board);
            connect(mcts, SIGNAL(finish(unsigned int)), this, SLOT(mcts_draw(unsigned int))); //里面不加参数
            mcts->start();
        }
    }
}

void GoBang::draw(int i, int j) {
    pair<double, double> coordinate = coordinate_transform_map2pixel(i, j);
    double x = coordinate.first, y = coordinate.second;
    if (piece_now == BLACK) {
        pieces[step]->setPixmap(*black);
        piece_now = WHITE;
    } else {
        pieces[step]->setPixmap(*white);
        piece_now = BLACK;
    }

    board.step(i * board.size + j);
    pieces[step]->setGeometry(x, y, PIECE, PIECE);
    step = board.round;

    if (board.spaces == 0) {
        gameover(-1);
    }

    int winner = board.winner;
    if (winner != EMPTY) {
        gameover(winner);
    }
}

void GoBang::mcts_draw(unsigned int action) {
    int i = (int)action / board.size, j = (int)action % board.size;
    if (step != 0) {
        draw(i, j);
        pair<double, double> coordinate = coordinate_transform_map2pixel(i, j);
        x = coordinate.first, y = coordinate.second;
    }
    ai_done = true;
    update();
}

void GoBang::paintEvent(QPaintEvent * e) {
    QPainter painter(this);
    if (step != 0) {
        QPen pen(Qt::black, 2, Qt::SolidLine);
        painter.setPen(pen);
        painter.drawLine(x - 5, y - 5, x + 3, y + 3);
        painter.drawLine(x + 3, y, x + 3, y + 3);
        painter.drawLine(x, y + 3, x + 3, y + 3);
    }
}

pair<int, int> GoBang::coordinate_transform_pixel2map(double x, double y) {
    pair<int, int> action;
    int i = int(round((y - MARGIN) / GRID)), j = int(round((x - MARGIN) / GRID));
    action.first = (i >= 0 && i <= (int)board.size) ? i : (-1);
    action.second = (j >= 0 && j <= (int)board.size) ? j : (-1);
    return action;
}

pair<double, double> GoBang::coordinate_transform_map2pixel(int i, int j) {
    pair<double, double> coordinate;
    coordinate.first = MARGIN + j * GRID - PIECE / 2.;
    coordinate.second = MARGIN + i * GRID - PIECE / 2.;
    return coordinate;
}

void GoBang::gameover(int winner) {
    QMessageBox::StandardButton reply;
    if (winner == BLACK)
        reply = QMessageBox::question(this, "", "You Win! Continue?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    else if (winner == WHITE)
        reply = QMessageBox::question(this, "", "You Lose! Continue?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    else
        reply = QMessageBox::question(this, "", "TIE! Continue?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        piece_now = BLACK;
        board.reset();
        step = 0;
        for (const auto & piece : pieces) {
            piece->clear();
        }
        update();
    } else {
        close();
    }
}
