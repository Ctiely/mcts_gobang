//
// Created by Clytie on 2018/10/3.
//

#include "TreeNode.h"

double TreeNode::get_value(double c_puct) {
    if (!is_root()) {
        u = c_puct * prior_p * sqrt(parent->n_visits) / (1 + n_visits);
        return Q + u;
    }
    return -1;
}

bool TreeNode::is_leaf() {
    return children.empty();
}

bool TreeNode::is_root() {
    return parent == nullptr;
}

void TreeNode::expend(vector<pair<unsigned int, double> > & action_probs) {
    for (const auto & action_prob : action_probs) {
        children.insert(make_pair(action_prob.first, new TreeNode(this, action_prob.second)));
        //NoExplore.push_back(action_prob.first);
    }
    NoExplore = children;
}

pair<unsigned int, TreeNode *> TreeNode::select(double c_puct) {
    auto action = (unsigned int)-1;
    TreeNode * node = nullptr;
    if (!is_leaf()) {
        double max_value = -DBL_MAX;
        if (NoExplore.empty()) {
            vector<pair<unsigned int, TreeNode *> > best_actions;
            for (const auto & child : children) {
                double cur_value = child.second->get_value(c_puct);
                if (cur_value > max_value) {
                    vector<pair<unsigned int, TreeNode *> >().swap(best_actions);
                    best_actions.emplace_back(child);
                    max_value = cur_value;
                } else if (abs(cur_value - max_value) < 1e-5) {
                    best_actions.emplace_back(child);
                }
            }
            auto action_index = random() % best_actions.size();
            return best_actions[action_index];
        } else {
            auto i = 0;
            auto action_index = random() % NoExplore.size();
            for (typename unordered_map<unsigned int, TreeNode *>::const_iterator itr = NoExplore.begin(); itr != NoExplore.end(); ++itr, ++i) {
                if (i == action_index) {
                    pair<unsigned int, TreeNode *> next_action = *itr;
                    NoExplore.erase(itr);
                    return next_action;
                    //action = itr->first;             //最初的NoExplore是个hash map,它中存储的节点是新创建的,所以与children中的是不同的节点,但如果使用NoExplore = children
                    //return *children.find(action); //就不会出错了,因为会把children中的指针复制一份给NoExplore,这两份指针指向相同的位置。这里一定要返回children中的节点指针,如果
                }                                  //返回的是NoExplore中的节点指针,它们其实指向不同的TreeNode。
            }
        }
    }
    return make_pair(action, node);
}

void TreeNode::update(double leaf_value) {
    TreeNode * cur = this;
    while (cur != nullptr) { //对当前节点cur中的n_visits和Q进行更新,蒙特卡罗树根节点也需要更新,因为在(TreeNode *)->get_value(c_pust)操作时会使用parent->n_visits作为分子
        ++(cur->n_visits);
        cur->Q += (leaf_value - cur->Q) / cur->n_visits;
        leaf_value *= -1;
        cur = cur->parent;
    }
}