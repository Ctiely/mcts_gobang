import random
import numpy as np


class TreeNode(object):
    def __init__(self, parent=None, prior_p=1):
        self.parent = parent
        self.n_visits = 0
        self.children = {}
        self._NoExplore = []
        self._Q = 0
        self._u = 0
        self._prior_p = prior_p

    def is_root(self):
        return self.parent is None

    def is_leaf(self):
        return self.children == {}

    def get_value(self, c_puct):
        if not self.is_root():
            self._u = c_puct * self._prior_p \
                      * np.sqrt(self.parent.n_visits) / (1 + self.n_visits)
            return self._Q + self._u
        return -1

    def expend(self, action_probs):
        for action, prob in action_probs:
            self.children[action] = TreeNode(self, prob)
            self._NoExplore.append(action)

    def select(self, c_puct):
        action = -1
        node = None
        if not self.is_leaf():
            if len(self._NoExplore):
                action = random.choice(self._NoExplore)
                node = self.children[action]
                self._NoExplore.remove(action)
                return action, node
            else:
                best_actions = []
                max_value = -np.inf
                for action, node in self.children.items():
                    cur_value = node.get_value(c_puct)
                    if cur_value > max_value:
                        best_actions = [(action, node)]
                        max_value = cur_value
                    elif abs(cur_value - max_value) < 1e-5:
                        best_actions.append((action, node))
                return random.choice(best_actions)
        else:
            return action, node

    def update(self, leaf_value):
        cur = self
        while cur is not None:
            cur.n_visits += 1
            cur._Q += (leaf_value - cur._Q) / cur.n_visits
            leaf_value *= -1
            cur = cur.parent

if __name__ == "__main__":
    root = TreeNode()
    root.expend([(0, 0.1), (1, 0.2), (2, 0.3), (3, 0.1), (4, 0.2), (5, 0.1)])
    action, node = root.select(5)
    assert node is root.children[action]