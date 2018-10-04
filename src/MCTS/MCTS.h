//
// Created by Clytie on 2018/9/7.
//

#ifndef MCTS_GOBANG_MCTS_H
#define MCTS_GOBANG_MCTS_H

#include <cmath>
#include <ctime>
#include <random>
#include <cfloat>
#include <vector>
#include <cassert>
#include <iostream>
#include <unordered_map>

#include "../utils.h"
#include "../env/Board.h"
#include "../TreeNode/TreeNode.h"
using namespace std;

class MCTS {
public:
    explicit MCTS(double c_puct=5.0, unsigned int n_playround=10000, unsigned int limit=100);
    ~MCTS() = default;

    unsigned int get_action(Board & board);
    void update_with_action(unsigned int last_action);
    unsigned int depth();

    unsigned int max_round;
private:
    TreeNode * root;
    double c_pust;
    unsigned int n_playround;

    unsigned int depth(TreeNode * node);
    void playout(Board & board);
    int evaluate_rollout(Board & board);
};


#endif //MCTS_GOBANG_MCTS_H
