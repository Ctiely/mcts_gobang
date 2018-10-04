//
// Created by Clytie on 2018/10/3.
//

#include "MCTS_alphago_zero.h"

MCTS_alphago_zero::MCTS_alphago_zero(const string & model_path,
                                     unsigned int size,
                                     double c_pust,
                                     unsigned int n_playround)
        : c_pust(c_pust), n_playround(n_playround), policy_value_network(model_path, size) {
    root = new TreeNode(nullptr, 1.0);

}

vector<float> MCTS_alphago_zero::softmax(vector<float> & x) {
    vector<float> probs;
    float max_value = *max_element(x.begin(), x.end());
    for (const auto & ix : x) {
        probs.emplace_back(exp(ix - max_value));
    }
    float sum_value = accumulate(probs.begin(), probs.end(), 0.0f);
    for (int i = 0; i < probs.size(); ++i) {
        probs[i] = probs[i] / sum_value;
    }
    return probs;
}

int MCTS_alphago_zero::evaluate(int winner, unsigned int current_player) {
    if (winner == -1) {
        return 0;
    } else {
        return 2 * (winner == current_player) - 1;
    }
}

void MCTS_alphago_zero::playout(Board & board) {
    TreeNode * cur = root;
    while (!cur->is_leaf()) {
        pair<unsigned int, TreeNode *> best = cur->select(c_pust);
        cur = best.second;
        board.step(best.first);
    }
    double leaf_value;
    if (!board.over()) {
        pair<vector<pair<unsigned int, double> >, float> action_probs_value = policy_value_network.policy_value_fn(board);
        cur->expend(action_probs_value.first);
        leaf_value = action_probs_value.second;
    } else {
        leaf_value = evaluate(board.winner, board.current_player());
    }
    cur->update(-leaf_value);
}

pair<vector<unsigned int>, vector<float> > MCTS_alphago_zero::get_action_probs(Board & board, float tau) {
    for (int i = 0; i < n_playround; ++i) {
        Board board_copy = board.copy();
        playout(board_copy);
    }
    vector<unsigned int> acts;
    vector<float> log_act_probs;
    for (const auto & child : root->children) {
        acts.emplace_back(child.first);
        log_act_probs.emplace_back(log(child.second->n_visits + 1e-10) / tau);
    }
    return make_pair(acts, softmax(log_act_probs));
}

void MCTS_alphago_zero::update_with_action(unsigned int last_action) {
    if (last_action == (unsigned int)-1) {
        root = new TreeNode(nullptr, 1.0);
        return;
    }
    unordered_map<unsigned int, TreeNode *>::const_iterator itr = root->children.find(last_action);
    if (itr != root->children.end()) {
        root = itr->second;
        root->parent = nullptr;
    } else {
        root = new TreeNode(nullptr, 1.0);
    }
}

unsigned int MCTS_alphago_zero::depth(TreeNode * node) {
    if (node->is_leaf()) {
        return 1;
    }
    unsigned int max_depth = 0;
    for (const auto & child : node->children) {
        unsigned int child_depth = depth(child.second);
        if (child_depth > max_depth) {
            max_depth = child_depth;
        }
    }
    return max_depth + 1;
}

unsigned int MCTS_alphago_zero::depth() {
    return depth(root);
}