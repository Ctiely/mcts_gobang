#include "game.h"

game::game(QWidget *parent) : QWidget(parent)
{
    person_mcts = new QPushButton(this);
    mcts_mcts = new QPushButton(this);

    setWindowTitle(tr("五子棋"));
    setMinimumSize(200, 100);
    setMaximumSize(200, 100);
    setWindowIcon(QIcon("../../../../img/board.png"));

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(person_mcts);
    mainLayout->addWidget(mcts_mcts);

    person_mcts->setText(tr("玩家 vs 电脑"));
    connect(person_mcts, SIGNAL(clicked()), this, SLOT(gobang_person_mcts()));

    mcts_mcts->setText(tr("电脑 vs 电脑"));
    connect(mcts_mcts, SIGNAL(clicked()), this, SLOT(gobang_mcts_mcts()));
}

void game::gobang_person_mcts() {
    GoBang * person = new GoBang;
    person->show();
}

void game::gobang_mcts_mcts() {
    gobang_mcts * mcts = new gobang_mcts;
    mcts->show();
}
