#include "game.h"

game::game(QWidget *parent) : QWidget(parent)
{
    person_mcts = new QPushButton(this);
    mcts_mcts = new QPushButton(this);
    person_mcts_full = new QPushButton(this);
    person_alphago = new QPushButton(this);
    mcts_alphago = new QPushButton(this);

    setWindowTitle(tr("五子棋"));
    setMinimumSize(200, 200);
    setMaximumSize(200, 200);
    //setWindowIcon(QIcon("../../../../img/chessboard.jpg"));
    setWindowIcon(QIcon("../src/GUI/img/chessboard.jpg"));

    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(person_mcts, 0);
    mainLayout->addWidget(mcts_mcts, 1);
    mainLayout->addWidget(person_mcts_full, 2);
    mainLayout->addWidget(person_alphago, 3);
    mainLayout->addWidget(mcts_alphago, 4);

    person_mcts->setText(tr("玩家 vs mcts: 9 × 9"));
    connect(person_mcts, SIGNAL(clicked()), this, SLOT(gobang_person_mcts()));

    mcts_mcts->setText(tr("mcts vs mcts: 9 × 9"));
    connect(mcts_mcts, SIGNAL(clicked()), this, SLOT(gobang_mcts_mcts()));

    person_mcts_full->setText(tr("玩家 vs mcts: 15 × 15"));
    connect(person_mcts_full, SIGNAL(clicked()), this, SLOT(gobang_person_mcts_full()));
    
    person_alphago->setText(tr("玩家 vs alphago: 9 × 9"));
    connect(person_alphago, SIGNAL(clicked()), this, SLOT(gobang_person_alphago()));
    
    mcts_alphago->setText(tr("mcts vs alphago: 9 × 9"));
    connect(mcts_alphago, SIGNAL(clicked()), this, SLOT(gobang_mcts_alphago()));
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

void game::gobang_person_alphago() {
    GoBang * alphago = new GoBang(nullptr, 9, true, "../src/models/model-10000");
    alphago->show();
}

void game::gobang_mcts_alphago() {
    gobang_mcts * alphago_mcts = new gobang_mcts(nullptr, true, "../src/models/model-10000");
    alphago_mcts->show();
}