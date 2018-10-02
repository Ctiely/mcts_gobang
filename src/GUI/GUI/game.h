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
    QPushButton * person_mcts_full;

public slots:
    void gobang_person_mcts();
    void gobang_mcts_mcts();
    void gobang_person_mcts_full();
};

#endif // GAME_H
