cmake_minimum_required(VERSION 3.9)
project(mcts_gobang)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -W")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "/Users/clytie/Documents/Qt/5.11.2/clang_64/lib/cmake")
find_package(Qt5Widgets CONFIG REQUIRED)
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
        src/MCTS_alphago_zero/MCTSPlayer_alphago_zero.cpp
        src/MCTS_alphago_zero/MCTSPlayer_alphago_zero.h
        src/GUI/GUI/game.cpp
        src/GUI/GUI/game.h
        src/GUI/GUI/GoBang.cpp
        src/GUI/GUI/GoBang.h
        src/GUI/GUI/gobang_mcts.cpp
        src/GUI/GUI/gobang_mcts.h
        src/GUI/GUI/MCTSThread.cpp
        src/GUI/GUI/MCTSThread.h
        main.cpp)
target_link_libraries(mcts_gobang tensorflow_cc tensorflow_framework Qt5::Widgets)