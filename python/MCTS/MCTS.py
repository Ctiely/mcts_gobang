import random
import numpy as np
from .TreeNode import TreeNode


class MCTS(object):
    def __init__(self, policy_value_fn, c_puct=5, n_playround=5000):
        self._policy_value = policy_value_fn
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
            action_probs, leaf_value = self._policy_value(board)
            cur.expend(action_probs)
        else:
            leaf_value = self._evaluate(board.winner, board.current_player)
        cur.update(-leaf_value)

    def get_action_probs(self, board, tau=1e-3):
        for _ in range(self._n_playround):
            board_copy = board.copy()
            self._playout(board_copy)
            #assert board_copy.moves != board.moves
            #assert board_copy.availables != board.availables
        act_visits = [(act, node.n_visits)
                      for act, node in self._root.children.items()]
        acts, visits = zip(*act_visits)
        act_probs = self._softmax(np.log(np.array(visits) + 1e-10) / tau)
        return acts, act_probs

    def update_with_action(self, action):
        if action in self._root.children:
            self._root = self._root.children[action]
            self._root.parent = None
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

class _MCTS(object):
    def __init__(self, c_puct=5, n_playround=5000, limit=1000):
        self.max_round = limit
        self._c_puct = c_puct
        self._n_playround = n_playround
        self._root = TreeNode()

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

    def get_action(self, board):
        for _ in range(self._n_playround):
            board_copy = board.copy()
            self._playout(board_copy)
        return max(self._root.children.items(),
                   key=lambda act_node: act_node[1].n_visits)[0]

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