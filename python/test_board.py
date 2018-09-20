import cBoard


def test_board():
    game = cBoard.cBoard(10, 5)
    game.step(0)
    game.step(1)
    game.step(10)
    game.step(2)
    game.step(20)
    game.step(3)
    game.step(30)
    game.step(4)
    game.step(11)
    game.step(12)
    game.step(22)
    board_copy = game.copy()
    game.step(21)
    game.step(31)
    game.step(33)
    game.step(44)
    game.step(5)
    print(game.round, game.over, game.winner)
    game.render()
    assert game.states != board_copy.states
    assert game.moves != board_copy.moves
    assert game.availables != board_copy.availables
    board_copy.render()
    print(board_copy.spaces, board_copy.over)