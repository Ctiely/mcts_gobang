//
// Created by Clytie on 2018/9/6.
//

#include "Board.h"
#include <iostream>
void Board::reset() {
    spaces = size * size;
    round = 0;
    states = map<int, int>();
    last_action = spaces;
    gameover = false;
    winner = -1;
    for (int i = 0; i < spaces; ++i) {
        availables.push_back(i);
    }
    vector<vector<vector<int> > >(2, vector<vector<int> >(size, vector<int>(size))).swap(moves);
}

void Board::check(Coordinate & cur) {
    int diff = win_n - 1;
    int left = ((cur.col - diff) > 0) ? (cur.col - diff) : 0;
    int right = (cur.col + diff < size - 1) ? (cur.col + diff) : (size - 1);
    int top = ((cur.row - diff) > 0) ? (cur.row - diff) : 0;
    int bottom = (cur.row + diff < size - 1) ? (cur.row + diff) : (size - 1);
    //cout << left << " " << right << " " << top << " " << bottom << endl;
    int vertical_sum = 0, horizontal_sum = 0, left_slope = 0, right_slope = 0;
    int player = current_player();
    for (int i = left; i <= right; ++i) {
        if (moves[player][cur.row][i] == 1) {
            ++horizontal_sum;
            if (horizontal_sum == win_n) {
                winner = player;
                gameover = true;
                return;
            }
        } else {
            horizontal_sum = 0;
        }
    }

    for (int i = top; i <= bottom; ++i) {
        if (moves[player][i][cur.col] == 1) {
            ++vertical_sum;
            if (vertical_sum == win_n) {
                winner = player;
                gameover = true;
                return;
            }
        } else {
            vertical_sum = 0;
        }
    }

    for (int i = -diff; i < win_n; ++i) {
        int left_top_row = cur.row + i;
        int left_top_col = cur.col + i;
        if (left_top_row >= 0 && left_top_row < size && left_top_col >= 0 && left_top_col < size) {
            if (moves[player][left_top_row][left_top_col] == 1) {
                ++left_slope;
                if (left_slope == win_n) {
                    winner = player;
                    gameover = true;
                    return;
                }
            } else {
                left_slope = 0;
            }
        }

        int right_top_row = cur.row + i;
        int right_top_col = cur.col - i;
        if (right_top_row >= 0 && right_top_row < size && right_top_col >= 0 && right_top_col < size) {
            if (moves[player][right_top_row][right_top_col] == 1) {
                ++right_slope;
                if (right_slope == win_n) {
                    winner = player;
                    gameover = true;
                    return;
                }
            } else {
                right_slope = 0;
            }
        }
    }
}

int Board::current_player() {
    return round % 2;
}

bool Board::over() {
    return gameover || spaces == 0;
}

void Board::step(int action) {
    if (!over() && states.find(action) == states.end()) {
        Coordinate cur(action, size);
        int player = current_player();
        moves[player][cur.row][cur.col] = 1;
        states.insert(pair<int, int>(action, player));
        availables.remove(action);
        last_action = action;
        --spaces;
        if (round < 2 * win_n - 2) {
            ++round;
            return;
        }
        check(cur);
        ++round;
    }
}

void Board::step(int row, int col) {
    int action = row * size + col;
    step(action);
}

void Board::render() {
    printf("  ");
    for (int row = 0; row < size; ++row) {
        printf(" %d", row);
    }
    printf("\n  ");
    for (int row = 0; row < size; ++row) {
        printf(" -");
    }
    printf("\n");
    for (int row = 0; row < size; ++row) {
        printf("%d |", row);
        for (int col = 0; col < size; ++col) {
            if (moves[0][row][col] == 1) {
                printf("0");
            } else if (moves[1][row][col] == 1) {
                printf("1");
            } else {
                printf("+");
            }
            if (col != size - 1) {
                printf(" ");
            } else {
                printf("|\n");
            }
        }
    }
    printf("  ");
    for (int row = 0; row < size; ++row) {
        printf(" -");
    }
    printf("\n");
}

Board Board::copy() {
    Board board(size, win_n);
    board.spaces = spaces;
    board.round = round;
    board.states = states;
    board.moves = moves;
    board.availables = availables;
    board.last_action = last_action;
    board.winner = winner;
    board.gameover = gameover;
    return board;
}