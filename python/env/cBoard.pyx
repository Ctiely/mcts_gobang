import copy

from libcpp cimport bool
from libcpp.list cimport list
from libcpp.vector cimport vector
from libcpp.map cimport map


cdef extern from "Board.h":
    cdef cppclass Board:
        Board(int boardsize, int win_n);

        void reset();
        void step(int action);
        void step(int row, int col);
        int current_player();
        bool over();
        void render();

        int size;
        int win_n;
        int spaces;
        int round;
        map[int, int] states;
        vector[vector[vector[int]]] moves;
        list[int] availables;
        int last_action;
        int winner;
        bool gameover;

cdef class cBoard:
    cdef Board * _thisptr

    def __cinit__(self, int boardsize, int win_n=5):
        self._thisptr = new Board(boardsize, win_n)

        if self._thisptr == NULL:
            raise MemoryError()

    def __dealloc__(self):
        if self._thisptr != NULL:
            del self._thisptr

    @property
    def _gameover(self):
        return self._thisptr.gameover

    cpdef void _set_gameover(self, bool gameover):
        self._thisptr.gameover = gameover

    @property
    def winner(self):
        return self._thisptr.winner

    cpdef void _set_winner(self, int winner):
        self._thisptr.winner = winner

    @property
    def last_action(self):
        return self._thisptr.last_action

    cpdef void _set_last_action(self, int action):
        self._thisptr.last_action = action

    @property
    def size(self):
        return self._thisptr.size

    @property
    def round(self):
        return self._thisptr.round

    cpdef void _set_round(self, int round):
        self._thisptr.round = round

    @property
    def spaces(self):
        return self._thisptr.spaces

    cpdef void _set_spaces(self, int spaces):
        self._thisptr.spaces = spaces

    @property
    def win_n(self):
        return self._thisptr.win_n

    @property
    def moves(self):
        return self._thisptr.moves

    cpdef void _set_moves(self, moves):
        self._thisptr.moves = moves

    @property
    def states(self):
        return self._thisptr.states

    cpdef void _set_states(self, dict states):
        self._thisptr.states = states

    @property
    def availables(self):
        return self._thisptr.availables

    cpdef void _set_availables(self, availables):
        self._thisptr.availables = availables

    @property
    def current_player(self):
        return self._thisptr.current_player()

    @property
    def over(self):
        return self._thisptr.over()

    cpdef void reset(self):
        self._thisptr.reset()

    cpdef void step(self, int action):
        self._thisptr.step(action)

    cpdef void step_(self, int row, int col):
        self._thisptr.step(row, col)

    cpdef void render(self):
        self._thisptr.render()

    cpdef cBoard copy(self):
        board_copy = cBoard(self.size, self.win_n)
        board_copy._set_spaces(self.spaces)
        board_copy._set_round(self.round)
        board_copy._set_states(copy.deepcopy(self.states))
        board_copy._set_moves(copy.deepcopy(self.moves))
        board_copy._set_availables(copy.deepcopy(self.availables))
        board_copy._set_last_action(self.last_action)
        board_copy._set_winner(self.winner)
        board_copy._set_gameover(self._gameover)
        return board_copy
