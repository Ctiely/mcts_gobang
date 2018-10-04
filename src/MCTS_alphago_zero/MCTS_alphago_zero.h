//
// Created by Clytie on 2018/10/3.
//

#ifndef MCTS_GOBANG_MCTS_ALPHAGO_ZERO_H
#define MCTS_GOBANG_MCTS_ALPHAGO_ZERO_H

#include <string>
#include "../env/Board.h"
#include "../network/network.h"
#include "../TreeNode/TreeNode.h"
using namespace std;

class MCTS_alphago_zero {
public:
    explicit MCTS_alphago_zero(const string & model_path,
                               unsigned int size,
                               double c_pust=5,
                               unsigned int n_playround=10000);
    ~MCTS_alphago_zero() = default;

    network policy_value_network;

    pair<vector<unsigned int>, vector<float> > get_action_probs(Board & board, float tau=1e-3);
    void update_with_action(unsigned int last_action);
    unsigned int depth();
private:
    TreeNode * root;
    double c_pust;
    unsigned int n_playround;

    vector<float> softmax(vector<float> & x);
    int evaluate(int winner, unsigned int current_player);
    void playout(Board & board);
    unsigned int depth(TreeNode * node);
};


#endif //MCTS_GOBANG_MCTS_ALPHAGO_ZERO_H
