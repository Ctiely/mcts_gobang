#include <iostream>
#include "src/Board.h"
#include "src/MCTSPlayer.h"

int main() {
    unsigned int win_n = 5;
    unsigned int size = 10;
    Board board(size, win_n);
    MCTSPlayer player(5, 15000, 100);
    unsigned int row = 5, col = 5, mcts_action = 0;
    board.render();
    while (!board.over()) {
        if (board.round % 2 == 1) {
            mcts_action = player.get_action(board);
            cout << mcts_action / size << " " << mcts_action % size << endl;
            board.step(mcts_action);
        } else {
            cout << "input your action: ";
            cin >> row;
            cin >> col;
            board.step(row, col);
        }
        cout << board.over() << endl;
        board.render();
    }

    /*
    board.step(55);
    MCTSPlayer player(5, 2000, 1000);
    cout << player.get_action(board) << endl;
    board.render();
    board.step(99);


    board.step(54);
    //cout << player.get_action(board) << endl;
    board.step(96);
    board.step(53);
    //cout << player.get_action(board) << endl;
    */
    /*
    game.step(0);
    game.step(1);
    game.step(10);
    game.step(2);
    game.step(20);
    game.step(3);
    game.step(30);
    game.step(4);
    game.step(11);
    game.step(12);
    game.step(22);
    Board copy = game.copy();
    game.step(21);
    game.step(31);
    game.step(33);
    game.step(44);
    game.step(5);
    cout << game.round << game.over() << game.winner << endl;
    game.render();
    copy.render();
    cout << copy.spaces << copy.over();

    vector<double> ivec(5, 0.1);
    for (int i = 0; i < 5; ++i) {
       // ivec.push_back(i);
    }

    cout << endl;
    list<string> svec;
    for (int j = 0; j < 10; ++j) {
        svec.push_back(to_string(j));
    }

    vector<pair<string, double> > t = utils::zip<string, double>(svec, ivec);
    list<int> l = {1, 2, 3, 4};
    for (int k = 0; k < t.size(); ++k) {
        cout << t[k].first << t[k].second << endl;
    }*/
    return 0;
}