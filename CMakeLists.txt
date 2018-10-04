cmake_minimum_required(VERSION 3.9)
project(mcts_gobang)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -W -ltensorflow_framework")
link_directories(/Users/clytie/Documents/tensorflow/bazel-bin/tensorflow)
include_directories(/Users/clytie/Documents/tensorflow
        /Users/clytie/Documents/tensorflow/bazel-genfiles
        /usr/local/include/eigen3)

add_executable(mcts_gobang
        src/env/Board.cpp
        src/env/Board.h
        src/MCTS/MCTS.cpp
        src/MCTS/MCTS.h
        src/MCTS/MCTSPlayer.cpp
        src/MCTS/MCTSPlayer.h
        src/network/network.cpp
        src/network/network.h
        src/utils.h
        src/MCTS_alphago_zero/MCTS_alphago_zero.cpp
        src/MCTS_alphago_zero/MCTS_alphago_zero.h
        src/TreeNode/TreeNode.cpp
        src/TreeNode/TreeNode.h
        main.cpp src/MCTS_alphago_zero/MCTSPlayer_alphago_zero.cpp src/MCTS_alphago_zero/MCTSPlayer_alphago_zero.h)
target_link_libraries(mcts_gobang tensorflow_cc)