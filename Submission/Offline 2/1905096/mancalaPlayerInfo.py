class PlayerInfo:
    __player_type = str
    __tag = str
    __storage_cup = int(6)  # index of my mancala
    __game_board = [4, 4, 4, 4, 4, 4, 0]
    __additional_moves = 0
    __captured = 0  # Last stone fell on an empty cup except the original one

    def __init__(self, tag):
        self.__tag = tag

    def set_attributes(self, prev_player):
        self.__additional_moves = prev_player.additional_moves_earned()
        self.__captured = prev_player.get_stones_captured()

    def reset_attributes(self):
        self.__additional_moves = 0
        self.__captured = 0

    def set_game_board(self, game_board):
        self.__game_board = game_board

    def get_game_board(self):
        return self.__game_board

    def add_stones_in_my_storage(self, stones):
        self.__game_board[self.__storage_cup] += stones

    # Stones in storage cup
    def stones_in_my_storage(self):
        return self.__game_board[self.__storage_cup]

    # Stones sum except the storage cup
    def stones_in_my_side(self):
        return sum(self.__game_board[:self.__storage_cup])

    # Returns the total number of additional moves earned
    def additional_moves_earned(self):
        return self.__additional_moves

    # Last stone falls on favorable storage
    def add_additional_moves(self, moves):
        self.__additional_moves = moves

    # Stones captured if the last stone falls in a favorable cup
    def set_stones_captured(self, captured):
        self.__captured = captured

    def get_stones_captured(self):
        return self.__captured

    def get_tag(self):
        return self.__tag

    def get_player_name(self):
        if self.__tag == "MAX":
            return "player 1"
        return "player 2"

