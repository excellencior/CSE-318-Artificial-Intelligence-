import math


class Board:
    __board = [4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0]
    __player1 = None
    __player2 = None
    __active_player = None
    __move = None
    __node_type = None
    __player_to_move = None

    def __init__(self, node_type):
        if node_type == "MAX":
            self.__value = -math.inf
        else:
            self.__value = math.inf
        self.__node_type = node_type

    def set_value(self, value):
        self.__value = value

    def get_value(self):
        return self.__value

    def set_board(self, board):
        self.__board = board

    def get_board(self):
        return self.__board

    def set_players(self, player1, player2):
        self.__player1 = player1
        self.__player2 = player2

    def get_players(self):
        return [self.__player1, self.__player2]

    def set_active_player(self, player_type):
        if player_type == "MAX":
            self.__active_player = self.__player1
        else:
            self.__active_player = self.__player2

    def get_active_player(self):
        if self.__node_type == "MAX":
            return self.__player1
        else:
            return self.__player2

    def set_move(self, move):
        self.__move = move

    def get_move(self):
        return self.__move

    def get_board_type(self):
        return self.__node_type

    def isGameOver(self):
        return (self.__player1.stones_in_my_side() == 0) or (self.__player2.stones_in_my_side() == 0)

    def __get_ab(self):
        if self.__active_player.get_tag() == "MAX":
            self.__player1 = self.__player1
            self.__player2 = self.__player2
        else:
            self.__player1 = self.__player2
            self.__player2 = self.__player1

        return [self.__player1, self.__player2]

    def heuristic1(self, w1):
        # mana, manb = self.__get_ab()
        return w1 * (self.__player1.stones_in_my_storage() - self.__player2.stones_in_my_storage())

    def heuristic2(self, w1, w2):
        # mana, manb = self.__get_ab()
        val1 = w1 * (self.__player1.stones_in_my_storage() - self.__player2.stones_in_my_storage())
        val2 = w2 * (self.__player1.stones_in_my_side() - self.__player2.stones_in_my_side())
        return val1 + val2

    def heuristic3(self, w1, w2, w3):
        # mana, manb = self.__get_ab()
        val1 = w1 * (self.__player1.stones_in_my_storage() - self.__player2.stones_in_my_storage())
        val2 = w2 * (self.__player1.stones_in_my_side() - self.__player2.stones_in_my_side())
        val3 = w3 * (self.__player1.additional_moves_earned() - self.__player2.additional_moves_earned())

        return val1 + val2 + val3

    def heuristic4(self, w1, w2, w3, w4):
        # mana, manb = self.__get_ab()
        val1 = w1 * (self.__player1.stones_in_my_storage() - self.__player2.stones_in_my_storage())
        val2 = w2 * (self.__player1.stones_in_my_side() - self.__player2.stones_in_my_side())
        val3 = w3 * (self.__player1.additional_moves_earned() - self.__player2.additional_moves_earned())
        val4 = w4 * (self.__player1.get_stones_captured() - self.__player2.get_stones_captured())

        return val1 + val2 + val3 + val4

    def run_heuristic(self, player_to_move, heuristic_id, parameters):  # tuple unpacking *parameters
        self.__player_to_move = player_to_move

        if heuristic_id == 1:
            return self.heuristic1(parameters)
        elif heuristic_id == 2:
            return self.heuristic2(*parameters)
        elif heuristic_id == 3:
            return self.heuristic3(*parameters)
        elif heuristic_id == 4:
            return self.heuristic4(*parameters)

    def print_board(self):
        print("Mancala Board")
        print("---------------------")
        print(
            f"   {self.__board[12]} {self.__board[11]} {self.__board[10]} {self.__board[9]} {self.__board[8]} {self.__board[7]}")
        print(f"{self.__board[13]}               {self.__board[6]}")
        print(
            f"   {self.__board[0]} {self.__board[1]} {self.__board[2]} {self.__board[3]} {self.__board[4]} {self.__board[5]}")
        print("---------------------")
