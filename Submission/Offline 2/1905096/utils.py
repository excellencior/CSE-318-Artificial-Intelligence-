from board import Board
from mancalaPlayerInfo import PlayerInfo


def get_valid_moves(player):
    moves = []
    if player.get_tag() == "MAX":
        start_idx = 0
    else:
        start_idx = 7

    playerBoard = player.get_game_board()
    for i in range(0, len(playerBoard)-1):
        if playerBoard[i] != 0:
            moves.append(start_idx + i)

    return moves  # If no moves are possible then len(moves) == 0


def gameOver(board, do_print):
    player1, player2 = board.get_players()
    game_board = board.get_board()

    if player1.stones_in_my_side() == 0:
        for i in range(7, 13):
            game_board[13] += game_board[i]
            game_board[i] = 0
        player2.set_game_board(game_board[7:])
    elif player2.stones_in_my_side() == 0:
        for i in range(0, 6):
            game_board[6] += game_board[i]
            game_board[i] = 0
        player1.set_game_board(game_board[:7])

    if do_print == "print":
        print("\n----------- Game Over ------------")
        board.print_board()

        if player1.stones_in_my_storage() > player2.stones_in_my_storage():
            print(f"{player1.get_tag()} wins the game with stones", player1.stones_in_my_storage())
        elif player1.stones_in_my_storage() < player2.stones_in_my_storage():
            print(f"{player2.get_tag()} wins the game with stones", player2.stones_in_my_storage())
        else:
            print("Draw!")

    return [player1, player2]


def get_next_board(board_obj, player, get_additional_move, captured, game_board):
    next_player = PlayerInfo("")
    player1 = PlayerInfo("MAX")
    player1.set_game_board(game_board[:7])

    player2 = PlayerInfo("MIN")
    player2.set_game_board(game_board[7:])

    prev_player1, prev_player2 = board_obj.get_players()
    # Pass down the values earned
    player1.set_attributes(prev_player1)
    player2.set_attributes(prev_player2)

    if player.get_tag() == "MAX":
        player1.set_stones_captured(player1.get_stones_captured() + captured)

        if get_additional_move:
            player1.add_additional_moves(player.additional_moves_earned() + 1)  # Player gets an additional move
            next_player = player1
        else:
            next_player = player2

    if player.get_tag() == "MIN":
        player2.set_stones_captured(player2.get_stones_captured() + captured)

        if get_additional_move:
            player2.add_additional_moves(player2.additional_moves_earned() + 1)  # Player gets an additional move
            next_player = player2
        else:
            next_player = player1

    new_board = Board(next_player.get_tag())
    new_board.set_board(game_board)
    new_board.set_players(player1, player2)
    new_board.set_active_player(next_player.get_tag())

    return new_board
