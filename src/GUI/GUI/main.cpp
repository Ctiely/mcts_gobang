#include "game.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    game g;
    g.show();

    return a.exec();
}
