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
    QPushButton * mcts_alphago;
    QPushButton * person_alphago;

public slots:
    void gobang_person_mcts();
    void gobang_mcts_mcts();
    void gobang_person_mcts_full();
    void gobang_person_alphago();
    void gobang_mcts_alphago();
};

#endif // GAME_H
