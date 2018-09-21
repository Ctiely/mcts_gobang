import numpy as np
from .MCTS import _MCTS, MCTS

class _MCTSPlayer(object):
    def __init__(self, c_puct=5, n_playout=5000, limit=1000):
        self.mcts = _MCTS(c_puct, n_playout, limit)

    def reset_player(self):
        self.mcts.update_with_action(-1)

    def get_action(self, board):
        action = board.size ** 2
        if not board.over:
            action = self.mcts.get_action(board)
            #print(self.mcts.depth())
            self.mcts.update_with_action(-1)
        return action

class MCTSPlayer(object):
    def __init__(self, policy_value_function, c_puct=5, n_playround=5000, is_selfplay=True):
        self.mcts = MCTS(policy_value_function, c_puct, n_playround)
        self._is_selfplay = is_selfplay

    def reset_player(self):
        self.mcts.update_with_action(-1)

    def get_action(self, board, tau=1e-3, return_prob=False):
        action_probs = np.zeros(board.size ** 2)
        if not board.over:
            actions, probs = self.mcts.get_action_probs(board, tau)
            action_probs[list(actions)] = probs
            if self._is_selfplay:
                action = np.random.choice(actions, p=0.75 * probs + 0.25 * np.random.dirichlet(0.3 * np.ones(len(probs))))
                self.mcts.update_with_action(action)
            else:
                action = np.random.choice(actions, p=probs)
                self.mcts.update_with_action(-1)

            if return_prob:
                return action, action_probs
            else:
                return action
