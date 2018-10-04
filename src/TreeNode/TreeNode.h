//
// Created by Clytie on 2018/10/3.
//

#ifndef MCTS_GOBANG_TREENODE_H
#define MCTS_GOBANG_TREENODE_H

#include <cmath>
#include <ctime>
#include <random>
#include <cfloat>
#include <vector>
#include <cassert>
#include <iostream>
#include <unordered_map>
using namespace std;

class TreeNode {
public:
    explicit TreeNode(TreeNode * parent=nullptr, double prior_p=1.0)
            : parent(parent), children(), n_visits(0), NoExplore(), Q(0), u(0), prior_p(prior_p) {};
    ~TreeNode() = default;

    void expend(vector<pair<unsigned int, double > > & action_probs);
    pair<unsigned int, TreeNode *> select(double c_puct);
    void update(double leaf_value);
    double get_value(double c_puct);
    bool is_leaf();
    bool is_root();

    TreeNode * parent;
    unordered_map<unsigned int, TreeNode *> children;
    unsigned int n_visits;
private:
    //list<unsigned int> NoExplore;
    unordered_map<unsigned int, TreeNode *> NoExplore;
    double Q;
    double u;
    double prior_p;
};


#endif //MCTS_GOBANG_TREENODE_H
