//
// Created by Clytie on 2018/10/3.
//

#ifndef CTENSORFLOW_NETWORK_H
#define CTENSORFLOW_NETWORK_H

#include <cmath>
#include <vector>
#include <string>
#include <utility>
#include <cassert>
#include <tensorflow/core/platform/env.h>
#include <tensorflow/core/public/session.h>
#include "tensorflow/core/protobuf/meta_graph.pb.h"
#include "tensorflow/cc/client/client_session.h"
#include "tensorflow/cc/ops/standard_ops.h"

#include "../env/Board.h"
using namespace std;
using namespace tensorflow;

class network {
public:
    explicit network(const string & model_path, unsigned int size);
    ~network() = default;

    pair<vector<pair<unsigned int, double> > , float> policy_value_fn(Board & board);

private:
    Tensor preprocess_state(Board & board);
    Status LoadModel(const string & model_path, std::unique_ptr<tensorflow::Session> * session);
    pair<vector<float> ,float> policy_value(Tensor & input_states);

    unsigned int size;
    unique_ptr<Session> session;
    const string input_states_name = "Placeholder:0";
    const string action_fc_name = "dense/LogSoftmax:0";
    const string evaluation_fc2_name = "dense_2/Tanh:0";
};


#endif //CTENSORFLOW_NETWORK_H
