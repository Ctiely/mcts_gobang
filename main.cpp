#include <iostream>

#include "src/env/Board.h"
#include "src/MCTS/MCTSPlayer.h"
#include "src/network/network.h"
#include "src/MCTS_alphago_zero/MCTSPlayer_alphago_zero.h"

void play() {
    unsigned int win_n = 5;
    unsigned int size = 10;
    Board board(size, win_n);
    MCTSPlayer player(5, 30000, 100);
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
}

void play_alphago_zero() {
    string model_path = "../python/models/model-2000";
    unsigned int win_n = 5;
    unsigned int size = 10;
    Board board(size, win_n);
    MCTSPlayer_alphago_zero player(model_path, size, 5, 15000);
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
}

void test_network() {
    const string model_path = "../python/models/model-2000";
    Board board(10, 5);
    board.step(5, 5);
    board.step(5, 6);
    board.step(4, 5);
    network policy_value_network(model_path, 10);
    auto result = policy_value_network.policy_value_fn(board);
    cout << result.second << endl;
    for (const auto & item : result.first) {
        cout << item.first << "," << item.second << " ";
    }
    cout << endl;
}

void test_alphago_zero() {
    const string model_path = "../python/models/model-2000";
    Board board(10, 5);
    board.step(5, 5);
    board.step(5, 6);
    board.step(4, 5);
    MCTS_alphago_zero player(model_path, 10);
    auto result = player.policy_value_network.policy_value_fn(board);
    cout << result.second << endl;
    for (const auto & item : result.first) {
        cout << item.first << "," << item.second << " ";
    }
    cout << endl;
}

void test_MCTS() {
    Board board(10, 5);
    board.step(5, 5);
    MCTS tree(5, 1000, 100);
    tree.get_action(board);
    unsigned int depth = tree.depth();
    cout << "depth " << depth << endl;
}

void test_zip() {
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
    for (const auto & item : t) {
        cout << item.first << " " << item.second << endl;
    }
}
void test_board() {
    Board game(10, 5);
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
}

void test_sampling() {
    vector<unsigned int> vec = {1, 2, 3, 5, 8};
    srandom((unsigned int)time(nullptr));
    for (int i = 0; i < 10; ++i) {
        vector<float> probs = {0.1, 0.3, 0.4, 0.5, 0.1};
        cout << utils::sampling(vec, probs) << endl;
    }
}

int main() {
    //test_board();
    //test_MCTS();
    //test_zip();
    //test_network();
    //test_alphago_zero();
    //test_sampling();
    //play();
    play_alphago_zero();
    return 0;
}
