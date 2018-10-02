#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "GoBang.h"
#include "gobang_mcts.h"

class game : public QWidget
{
    Q_OBJECT
public:
    explicit game(QWidget *parent = nullptr);
    QPushButton * person_mcts;
    QPushButton * mcts_mcts;

public slots:
    void gobang_person_mcts();
    void gobang_mcts_mcts();
};

#endif // GAME_H
