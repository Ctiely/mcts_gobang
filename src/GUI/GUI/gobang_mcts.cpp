#include "gobang_mcts.h"

gobang_mcts::gobang_mcts(QWidget *parent, bool alphago_play, std::string model_path)
    : QWidget(parent),
      board(9, 5),
      piece_now(BLACK),
      step(0),
      mcts(nullptr),
      alphago(nullptr),
      alphago_play(alphago_play),
      model_path(std::move(model_path))
{
    init();
    play();
}

void gobang_mcts::init() {
    palette = new QPalette;
    //palette->setBrush(QPalette::Background,QBrush(QPixmap("../../../../img/board.png")));
    palette->setBrush(QPalette::Background,QBrush(QPixmap("../src/GUI/img/board.png")));

    setPalette(*palette);
    setCursor(Qt::PointingHandCursor);
    resize(WIDTH, HEIGHT);
    setMinimumSize(WIDTH, HEIGHT);
    setMaximumSize(WIDTH, HEIGHT);

    setWindowTitle("GoBang");
    //setWindowIcon(QIcon("../../../../img/black.png"));

    //black = new QPixmap("../../../../img/black.png");
    //white = new QPixmap("../../../../img/white.png");
    setWindowIcon(QIcon("../src/GUI/img/black.png"));

    black = new QPixmap("../src/GUI/img/black.png");
    white = new QPixmap("../src/GUI/img/white.png");
    for (unsigned int i = 0; i < board.size * board.size; ++i) {
        pieces.push_back(new Pieces(this));
    }
}

void gobang_mcts::play() {
    timer = new QTimer(this);
    timer->start(4000);
    if (alphago_play) {
        connect(timer, SIGNAL(timeout()), this, SLOT(alphago_move()));
    } else {
        connect(timer, SIGNAL(timeout()), this, SLOT(mcts_move()));
    }
}

void gobang_mcts::mcts_move() {
    if (board.over()) {
        return;
    }
    if (board.current_player() == 0) {
        mcts = new MCTSPlayer(5, 25000, 100);
    } else {
        mcts = new MCTSPlayer(5, 25000, 100);
    }
    unsigned int action = mcts->get_action(board);
    ai_draw(action);
}

void gobang_mcts::alphago_move() {
    if (board.over()) {
        return;
    }
    unsigned int action;
    if (board.current_player() == 0) {
        alphago = new MCTSPlayer_alphago_zero(model_path, board.size, 5, 15000);
        action = alphago->get_action(board);
    } else {
        mcts = new MCTSPlayer(5, 15000, 100);
        action = mcts->get_action(board);
    }
    ai_draw(action);
}

void gobang_mcts::draw(int i, int j) {
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

void gobang_mcts::gameover(int winner) {
    if (winner == BLACK)
        QMessageBox::information(this, "", "player1 Win!", QMessageBox::Yes);
    else if (winner == WHITE)
        QMessageBox::information(this, "", "player2 Win!", QMessageBox::Yes);
    else
        QMessageBox::information(this, "", "TIE!", QMessageBox::Yes);

    close();
}

void gobang_mcts::ai_draw(unsigned int action) {
    int i = (int)action / board.size, j = (int)action % board.size;
    draw(i, j);
    update();
}

pair<double, double> gobang_mcts::coordinate_transform_map2pixel(int i, int j) {
    pair<double, double> coordinate;
    coordinate.first = MARGIN + j * GRID - PIECE / 2.;
    coordinate.second = MARGIN + i * GRID - PIECE / 2.;
    return coordinate;
}
