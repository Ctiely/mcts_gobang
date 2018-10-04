//
// Created by Clytie on 2018/10/3.
//

#include "network.h"

Status network::LoadModel(const string &model_path, std::unique_ptr<tensorflow::Session> * session) {
    const string pathToGraph = model_path + ".meta";
    tensorflow::MetaGraphDef graph_def;
    Status load_graph_status = ReadBinaryProto(tensorflow::Env::Default(), pathToGraph, &graph_def);
    if (!load_graph_status.ok()) {
        return tensorflow::errors::NotFound("Failed to load compute graph at '", pathToGraph, "'");
    }
    session->reset(tensorflow::NewSession(tensorflow::SessionOptions()));
    Status session_create_status = (*session)->Create(graph_def.graph_def());
    if (!session_create_status.ok()) {
        return session_create_status;
    }
    Tensor modelPathTensor(DT_STRING, TensorShape());
    modelPathTensor.scalar<std::string>()() = model_path;
    Status load_model_status = (*session)->Run(
            {{graph_def.saver_def().filename_tensor_name(), modelPathTensor},},
            {},
            {graph_def.saver_def().restore_op_name()},
            nullptr);
    if (!load_model_status.ok()) {
        return load_model_status;
    }
    return Status::OK();
}

Tensor network::preprocess_state(Board & board) {
    Tensor square_state(DT_FLOAT, TensorShape({1, 4, board.size, board.size}));
    auto input_map = square_state.tensor<float, 4>();
    for (int i = 0; i < board.size; ++i) {
        for (int j = 0; j < board.size; ++j) {
            input_map(0, 0, i, j) = board.moves[0][i][j];
            input_map(0, 1, i, j) = board.moves[1][i][j];
            input_map(0, 2, i, j) = 0.0;
            input_map(0, 3, i, j) = 1.0 - board.current_player();
        }
    }
    if (!board.states.empty()) {
        //assert(board.last_action != board.size * board.size);
        input_map(0, 2, board.last_action / board.size, board.last_action % board.size) = 1.0;
    }
    return square_state;
}

network::network(const string & model_path, unsigned int size)
        : size(size), session() {
    Status load_model_status = LoadModel(model_path, &session);
    if (!load_model_status.ok()) {
        LOG(ERROR) << load_model_status;
    }
}

pair<vector<float> ,float> network::policy_value(Tensor & input_states) {
    vector<Tensor> outputs;
    Status run_status = session->Run({{input_states_name, input_states}},
                                     {action_fc_name, evaluation_fc2_name},
                                     {},
                                     &outputs);
    if (!run_status.ok()) {
        LOG(ERROR) << "Running model failed: " << run_status;
    }
    float * pprobs = outputs[0].tensor<float, 2>().data();
    vector<float> action_probs;
    for (int i = 0; i < size * size; ++i) {
        action_probs.emplace_back(exp(*(pprobs + i)));
    }
    return make_pair(action_probs, *outputs[1].scalar<float>().data());
}

pair<vector<pair<unsigned int, double> >, float> network::policy_value_fn(Board & board) {
    assert(board.size == size);
    Tensor input_states = preprocess_state(board);
    auto action_probs_value = policy_value(input_states);
    vector<pair<unsigned int, double> > action_probs;
    for (const auto & available : board.availables) {
        action_probs.emplace_back(make_pair(available, action_probs_value.first[available]));
    }
    return make_pair(action_probs, action_probs_value.second);
}