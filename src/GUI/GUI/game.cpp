#include "game.h"

game::game(QWidget *parent) : QWidget(parent)
{
    person_mcts = new QPushButton(this);
    mcts_mcts = new QPushButton(this);
    person_mcts_full = new QPushButton(this);

    setWindowTitle(tr("五子棋"));
    setMinimumSize(200, 200);
    setMaximumSize(200, 200);
    setWindowIcon(QIcon("../../../../img/chessboard.jpg"));

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(person_mcts, 0);
    mainLayout->addWidget(mcts_mcts, 1);
    mainLayout->addWidget(person_mcts_full, 2);

    person_mcts->setText(tr("玩家 vs 电脑: 9 × 9"));
    connect(person_mcts, SIGNAL(clicked()), this, SLOT(gobang_person_mcts()));

    mcts_mcts->setText(tr("电脑 vs 电脑: 9 × 9"));
    connect(mcts_mcts, SIGNAL(clicked()), this, SLOT(gobang_mcts_mcts()));

    person_mcts_full->setText(tr("玩家 vs 电脑: 15 × 15"));
    connect(person_mcts_full, SIGNAL(clicked()), this, SLOT(gobang_person_mcts_full()));
}

void game::gobang_person_mcts() {
    GoBang * person = new GoBang;
    person->show();
}

void game::gobang_mcts_mcts() {
    gobang_mcts * mcts = new gobang_mcts;
    mcts->show();
}

void game::gobang_person_mcts_full() {
    GoBang * person_full = new GoBang(nullptr, 15);
    person_full->show();
}
