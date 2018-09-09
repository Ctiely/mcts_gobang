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
#include "Board.h"
#include "utils.h"
using namespace std;

class MCTS {
public:
    explicit MCTS(double c_puct=sqrt(2), unsigned int n_playround=5000, unsigned int limit=1000)
            : c_pust(c_puct), n_playround(n_playround), max_round(limit) {
        root = new TreeNode(nullptr, 1.0);
    }
    ~MCTS() = default;

    unsigned int get_action(Board & board);
    void update_with_action(unsigned int last_action);
    unsigned int depth();

    unsigned int max_round;
private:
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

    TreeNode * root;
    double c_pust;
    unsigned int n_playround;

    unsigned int depth(TreeNode * node);
    void playout(Board & board);
    int evaluate_rollout(Board & board);
};


#endif //MCTS_GOBANG_MCTS_H
