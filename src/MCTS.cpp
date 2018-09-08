//
// Created by Clytie on 2018/9/7.
//

#include "MCTS.h"

double MCTS::TreeNode::get_value(double c_puct) {
    if (!is_root()) {
        u = c_puct * prior_p * sqrt(parent->n_visits) / (1 + n_visits);
        return Q + u;
    }
    return -1;
}

bool MCTS::TreeNode::is_leaf() {
    return children.empty();
}

bool MCTS::TreeNode::is_root() {
    return parent == nullptr;
}

void MCTS::TreeNode::expend(vector<pair<unsigned int, double> > & action_probs) {
    for (const auto & action_prob : action_probs) {
        children.insert(make_pair(action_prob.first, new TreeNode(this, action_prob.second)));
        NoExplore.insert(make_pair(action_prob.first, new TreeNode(this, action_prob.second)));
    }
}

pair<unsigned int, MCTS::TreeNode *> MCTS::TreeNode::select(double c_puct) {
    auto action = (unsigned int)-1;
    MCTS::TreeNode * node = nullptr;
    if (!is_leaf()) {
        double max_value = -DBL_MAX;
        if (NoExplore.empty()) {
            vector<pair<unsigned int, MCTS::TreeNode *> > best_actions;
            for (const auto & child : children) {
                double cur_value = child.second->get_value(c_puct);
                //cout << "cur_value " << cur_value << " " << child.second->n_visits << endl;
                if (cur_value > max_value) {
                    vector<pair<unsigned int, MCTS::TreeNode *> >().swap(best_actions);
                    best_actions.emplace_back(child);
                    max_value = cur_value;
                } else if (abs(cur_value - max_value) < 1e-5) {
                    best_actions.emplace_back(child);
                }
            }
            /*for (const auto & move : best_actions) {
                cout << move.first << " ";
            }*/
            //cout << "max " << max_value << endl;
            auto action_index = random() % best_actions.size();
            //cout << "action_index " << action_index << endl;
            //cout << "action_index " << action_index << " " << best_actions.size() << endl;
            return best_actions[action_index];
        } else {
            auto i = 0;
            auto action_index = random() % NoExplore.size();
            for (typename unordered_map<unsigned int, MCTS::TreeNode *>::const_iterator itr = NoExplore.begin(); itr != NoExplore.end(); ++itr, ++i) {
                if (i == action_index) {
                    pair<unsigned int, MCTS::TreeNode *> next_action = *itr;
                    NoExplore.erase(itr);
                    //cout << "next_action " << next_action.first << endl;
                    return next_action;
                }
            }
        }
    }
    return make_pair(action, node);
}

void MCTS::TreeNode::update(double leaf_value) {
    MCTS::TreeNode * cur = this;
    while (cur != nullptr) { //对当前节点cur中的n_visits和Q进行更新,蒙特卡罗树根节点也需要更新,因为在(TreeNode *)->get_value(c_pust)操作时会使用parent->n_visits作为分子
        ++(cur->n_visits);
        cur->Q += (leaf_value - cur->Q) / cur->n_visits;
        leaf_value *= -1;
        cur = cur->parent;
        //cout << "n_visits " << cur->n_visits << endl;
    }
}

void MCTS::playout(Board & board) {
    MCTS::TreeNode * cur = root;
    while (!cur->is_leaf()) {
        pair<unsigned int, MCTS::TreeNode *> best = cur->select(c_pust);
        cur = best.second;
        board.step(best.first);
        //cout << "best " << best.first << endl;
    }
    vector<double> probs(board.spaces, 1 / board.spaces);
    assert(board.availables.size() == probs.size());
    vector<pair<unsigned int, double> > action_probs = utils::zip<unsigned int, double>(board.availables, probs);

    if (!board.over()) {
        cur->expend(action_probs);
    }
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
                //cout << "action " << *itr << endl;
                break;
            }
        }
        ++round;
    }
    int value = 0;
    if (board.winner != -1) {
        if (board.winner == player) {
            value = 1;
        } else {
            value = -1;
        }
    }
    return value;
}

unsigned int MCTS::get_action(Board & board) { //每个状态(board)下都重建一棵蒙特卡罗树
    for (int i = 0; i < n_playround; ++i) {
        Board board_copy = board.copy();
        playout(board_copy);
        assert(board_copy.moves != board.moves);
        assert(board_copy.availables != board.availables);
    }
    auto max_n_visits = 0;
    unsigned int action = board.spaces;
    for (const auto & child : root->children) {
        if (child.second->n_visits > max_n_visits) {
            max_n_visits = child.second->n_visits;
            action = child.first;
        }
    }
    return action;
}

void MCTS::update_with_action(unsigned int last_action) {
    if (last_action == (unsigned int)-1) {
        root = new TreeNode(nullptr, 1.0);
        return;
    }
    unordered_map<unsigned int, MCTS::TreeNode *>::const_iterator itr = root->children.find(last_action);
    if (itr != root->children.end()) {
        root = itr->second;
        root->parent = nullptr;
    } else {
        root = new TreeNode(nullptr, 1.0);
    }
}