//
// Created by Clytie on 2018/9/6.
//

#ifndef MCTS_GOBANG_UTILS_H
#define MCTS_GOBANG_UTILS_H

#include <list>
#include <vector>
#include <random>
#include <cassert>
#include <utility>

namespace utils { //两个namespace可能会冲突
    template <typename T1, typename T2>
    static std::vector<std::pair<T1, T2> > zip(std::vector<T1> & vec1, std::vector<T2> & vec2) {
        std::vector<std::pair<T1, T2> > result;
        unsigned long length1 = vec1.size();
        unsigned long length2 = vec2.size();
        unsigned long min_length = (length1 < length2) ? length1 : length2;
        if (min_length <= 0) {
            return result;
        }
        for (unsigned long i = 0; i < min_length; ++i) {
            result.push_back(std::make_pair(vec1[i], vec2[i]));
        }
        return result;
    }

    template <typename T1, typename T2>
    static std::vector<std::pair<T1, T2> > zip(std::list<T1> & list1, std::vector<T2> & vec2) {
        std::vector<std::pair<T1, T2> > result;
        unsigned long length1 = list1.size();
        unsigned long length2 = vec2.size();
        unsigned long min_length = (length1 < length2) ? length1 : length2;
        if (min_length <= 0) {
            return result;
        }
        int i = 0;
        for (typename std::list<T1>::const_iterator itr = list1.begin(); itr != list1.end(); ++itr, ++i) {
            if (i < min_length) {
                result.push_back(std::make_pair(*itr, vec2[i]));
            } else {
                break;
            }
        }
        return result;
    }

    template <typename T1>
    static T1 sampling(std::vector<T1> & vec, std::vector<float> & probs) {
        //assert(vec.size() == probs.size());
        for (int i = 1; i < vec.size(); ++i) {
            probs[i] += probs[i - 1];
        }
        float u = ((float)random() / RAND_MAX) * probs.back();
        int index;
        for (index = 0; index < vec.size(); ++index) {
            if (probs[index] > u) {
                break;
            }
        }
        return vec[index];
    }
};


#endif //MCTS_GOBANG_UTILS_H
