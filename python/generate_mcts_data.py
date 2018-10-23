#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Oct 23 21:26:35 2018

@author: clytie
"""

import json
import random
import logging
import numpy as np
from collections import deque
from env.cBoard import cBoard
from MCTS.TreeNode import TreeNode
from network.policy_value_network import preprocess_state

logging.basicConfig(level=logging.INFO, format='%(asctime)s|%(levelname)s|%(message)s')

class MCTS(object):
    def __init__(self, c_puct=5, n_playround=5000, limit=1000):
        self.max_round = limit
        self._c_puct = c_puct
        self._n_playround = n_playround
        self._root = TreeNode()

    def _softmax(self, x):
        probs = np.exp(x - np.max(x))
        probs /= np.sum(probs)
        return probs
    
    def _evaluate(self, winner, current_player):
        if winner == -1:
            return 0
        else:
            return 2 * (winner == current_player) - 1

    def _playout(self, board):
        cur = self._root
        while not cur.is_leaf():
            action, cur = cur.select(self._c_puct)
            board.step(action)
        if not board.over:
            action_prob = np.ones(board.spaces) / board.spaces
            #assert len(board.availables) == board.spaces
            action_probs = zip(board.availables, action_prob)
            cur.expend(action_probs)
        leaf_value = self._evaluate_rollout(board)
        cur.update(-leaf_value)

    def _evaluate_rollout(self, board):
        player = board.current_player
        round = 0
        while not board.over and round < self.max_round:
            action = random.choice(board.availables)
            board.step(action)
            round += 1
        return self._evaluate(board.winner, player)
    
    def get_action_probs(self, board, tau=1e-3):
        for _ in range(self._n_playround):
            board_copy = board.copy()
            self._playout(board_copy)
        act_visits = [(act, node.n_visits)
                      for act, node in self._root.children.items()]
        acts, visits = zip(*act_visits)
        act_probs = self._softmax(np.log(np.array(visits) + 1e-10) / tau)
        return acts, act_probs

    def update_with_action(self, action):
        if action in self._root.children:
            self._root = self._root.children[action]
            self._root._parent = None
        else:
            self._root = TreeNode()

    def depth(self):
        return self._depth(self._root)

    def _depth(self, node):
        if node.is_leaf():
            return 1
        max_depth = 0
        for action, node in node.children.items():
            child_depth = self._depth(node)
            if child_depth > max_depth:
                max_depth = child_depth
        return max_depth + 1

class MCTSPlayer(object):
    def __init__(self, c_puct=5, n_playround=5000, limit=1000):
        self.mcts = MCTS(c_puct, n_playround, limit)

    def reset_player(self):
        self.mcts.update_with_action(-1)

    def get_action(self, board, tau=1e-3, return_prob=False):
        action_probs = np.zeros(board.size ** 2)
        if not board.over:
            actions, probs = self.mcts.get_action_probs(board, tau)
            action_probs[list(actions)] = probs
            
            action = np.random.choice(actions, p=probs)
            self.mcts.update_with_action(-1)

            if return_prob:
                return action, action_probs
            else:
                return action
    
class DataGenerator(object):
    def __init__(self):
        # params of the board and the game
        self.board_size = 9
        self.win_n = 5
        self.board = cBoard(self.board_size, self.win_n)
        self.buffer_size = 100
        self.data_buffer = deque(maxlen=self.buffer_size)
        self.play_batch_size = 1
        self.mcts_player = MCTSPlayer(5, 10000, 100)

    def get_equi_data(self, play_data):
        """
            augment the data set by rotation and flipping
            play_data: [(state, mcts_prob, winner_z), ..., ...]
        """
        extend_data = []
        for state, mcts_porb, winner in play_data:
            for i in [1, 2, 3, 4]:
                # rotate counterclockwise
                equi_state = np.array([np.rot90(s, i) for s in state])
                equi_mcts_prob = np.rot90(np.flipud(
                    mcts_porb.reshape(self.board_size, self.board_size)), i)
                extend_data.append((equi_state,
                                    np.flipud(equi_mcts_prob).flatten(),
                                    winner))
                # flip horizontally
                equi_state = np.array([np.fliplr(s) for s in equi_state])
                equi_mcts_prob = np.fliplr(equi_mcts_prob)
                extend_data.append((equi_state,
                                    np.flipud(equi_mcts_prob).flatten(),
                                    winner))
        return extend_data

    def self_play(self):
        self.board.reset()
        states, mcts_probs, current_players = [], [], []
        while not self.board.over:
            #print("depth: ", self.mcts_player.mcts.depth())
            action, action_probs = self.mcts_player.get_action(self.board, 
                                                               return_prob=True)
            # store the data
            states.append(preprocess_state(self.board))
            mcts_probs.append(action_probs)
            current_players.append(self.board.current_player)
            # perform a action
            #print(action)
            self.board.step(action)
        # winner from the perspective of the current player of each state
        winners_z = np.zeros(len(current_players))
        if self.board.winner != -1:
            winners_z[np.array(current_players) == self.board.winner] = 1.0
            winners_z[np.array(current_players) != self.board.winner] = -1.0
        # reset MCTS root node
        self.mcts_player.reset_player()
        return self.board.winner, zip(states, mcts_probs, winners_z)

    def collect_selfplay_data(self, n_games=1):
        """collect self-play data for training"""
        for i in range(n_games):
            winner, play_data = self.self_play()
            play_data = list(play_data)[:]
            # augment the data
            play_data = self.get_equi_data(play_data)
            self.data_buffer.extend(play_data)

    def save(self, data_file="mcts_data.dat"):
        mcts_datas = []
        while len(self.data_buffer) < self.buffer_size:
            self.collect_selfplay_data(self.play_batch_size)
            logging.info("finish!")
        else:
            for data in self.data_buffer:
                mcts_data = {}
                mcts_data["states"] = data[0].tolist()
                mcts_data["probs"] = data[1].tolist()
                mcts_data["winners"] = data[2]
                mcts_datas.append(mcts_data)
            with open(data_file, "w") as f:
                json.dump(mcts_datas, f)
            logging.info("save mcts data into %s" % data_file)
            
            
if __name__ == '__main__':
    data_generator = DataGenerator()
    data_generator.save("./mcts_data.dat")