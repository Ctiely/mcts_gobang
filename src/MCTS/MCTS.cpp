//
// Created by Clytie on 2018/9/7.
//

#include "MCTS.h"

MCTS::MCTS(double c_puct, unsigned int n_playround, unsigned int limit)
        : c_pust(c_puct), n_playround(n_playround), max_round(limit) {
    root = new TreeNode(nullptr, 1.0);
}

void MCTS::playout(Board & board) {
    TreeNode * cur = root;
    while (!cur->is_leaf()) {
        pair<unsigned int, TreeNode *> best = cur->select(c_pust);
        cur = best.second;
        board.step(best.first);
    }
    if (!board.over()) {
        vector<double> probs(board.spaces, 1.0 / board.spaces);
        //assert(board.availables.size() == probs.size());
        vector<pair<unsigned int, double> > action_probs = utils::zip<unsigned int, double>(board.availables, probs);
        cur->expend(action_probs);
    }
    /* check
    if (!cur->is_root()) {
        cout << cur->parent->children.size() << " " << depth(cur) << " ";
        for (const auto & child : cur->parent->children) {
            if (child.second == cur) {
                cout << "first " << child.first << " ";
            }
        }
    }
     */
    double leaf_value = evaluate_rollout(board);

    cur->update(-leaf_value);
}

int MCTS::evaluate_rollout(Board & board) {
    unsigned int player = board.current_player();
    int round = 0;
    while (!board.over() && round < max_round) {
        auto i = 0;
        auto action_index = random() % board.spaces;
        for (typename list<unsigned int>::const_iterator itr = board.availables.begin(); itr != board.availables.end(); ++itr, ++i) {
            if (i == action_index) {
                board.step(*itr);
                break;
            }
        }
        ++round;
    }
    int value = 0;
    if (board.winner != -1) {
        value = 2 * (board.winner == player) - 1;
    }
    return value;
}

unsigned int MCTS::get_action(Board & board) { //每个状态(board)下都重建一棵蒙特卡罗树
    for (int i = 0; i < n_playround; ++i) {
        Board board_copy = board.copy();
        playout(board_copy);
    }
    vector<unsigned int> max_visits;
    unsigned int max_n_visits = 0;
    for (const auto & child : root->children) {
        if (child.second->n_visits > max_n_visits) {
            max_n_visits = child.second->n_visits;
            vector<unsigned int>().swap(max_visits);
            max_visits.emplace_back(child.first);
        } else if (child.second->n_visits == max_n_visits) {
            max_visits.emplace_back(child.first);
        }
    }
    return max_visits[random() % max_visits.size()];
}

void MCTS::update_with_action(unsigned int last_action) {
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

unsigned int MCTS::depth(TreeNode * node) {
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

unsigned int MCTS::depth() {
    return depth(root);
}
