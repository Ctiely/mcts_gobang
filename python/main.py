from env.cBoard import cBoard
from MCTS.MCTSPlayer import _MCTSPlayer


def play():
    size = 10
    win_n = 5
    board = cBoard(size, win_n)
    player = _MCTSPlayer(5, 10000, 100)
    board.render()
    while not board.over:
        if board.round % 2 == 1:
            mcts_action = player.get_action(board)
            board.step(mcts_action)
        else:
            row = input("input your action row: ")
            col = input("input your action col: ")
            board.step_(int(row), int(col))
        board.render()

if __name__ == "__main__":
    play()