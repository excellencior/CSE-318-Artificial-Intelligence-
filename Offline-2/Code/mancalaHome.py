import math

from utils import *
from board import Board
import random

# Game Simulation
player_storage_cup_no = 6
computer_storage_cup_no = 13
board_size = 14
do_print = "noprint"
game_type = "auto"


# Distributing stones
def distribute(cup_no, board_obj):  # The player for whom IDS is being executed to find the best move
    player = board_obj.get_active_player()
    game_board = list.copy(board_obj.get_board())

    if player.get_tag() == "MAX":  # Stones should not fall is opponent's cup
        banned_cup = computer_storage_cup_no
        storage_cup = player_storage_cup_no
    else:
        banned_cup = player_storage_cup_no
        storage_cup = computer_storage_cup_no

    stones = game_board[cup_no]
    game_board[cup_no] = 0  # Transfer stones from this cup to other cups
    start_cup_no = cup_no
    cup_no += 1  # Start distributing from the next cup

    while stones > 1:
        if cup_no != banned_cup:
            game_board[cup_no] += 1  # Add stones to the next cups one at a time
            stones -= 1
        cup_no += 1
        cup_no = cup_no % board_size

    if 0 <= cup_no <= 5:
        region = "MAX"
    elif 7 <= cup_no <= 12:
        region = "MIN"
    else:
        region = "NONE"  # Storage cups are not considered for capturing

    captured = 0
    # Last stone            # The last stone is in the current player's region
    if (start_cup_no != cup_no) and (player.get_tag() == region):
        mod_cup_no = cup_no % 7
        other_side_cup_no = 12 - cup_no

        if (player.get_game_board()[mod_cup_no] == 0) and (game_board[other_side_cup_no] != 0):
            captured = 1 + game_board[other_side_cup_no]
            game_board[other_side_cup_no] = 0

            game_board[storage_cup] += captured
        else:
            game_board[cup_no] += 1
    else:
        game_board[cup_no] += 1

    get_additional_move = False
    if player.get_tag() == "MAX" and cup_no == 6:
        get_additional_move = True
    elif player.get_tag() == "MIN" and cup_no == 13:
        get_additional_move = True

    # Creating new board
    new_board_obj = get_next_board(board_obj, player, get_additional_move, captured, game_board)

    return new_board_obj


# Every node in the solution tree is a board_object
def minimax_alpha_beta(board, depth, alpha, beta, hp, parameters, player_to_move):
    # if ( depth == 3 ) print(alpha,beta,board.get_move)

    if (depth == 0) or (board.get_active_player().stones_in_my_side() == 0):
        if board.get_active_player().get_tag() == "MAX":
            board.set_value(board.run_heuristic(player_to_move, hp[0], parameters[0]))
        else:
            board.set_value(board.run_heuristic(player_to_move, hp[1], parameters[1]))
        return board

    valid_moves = get_valid_moves(board.get_active_player())
    random.shuffle(valid_moves)

    if board.get_board_type() == "MAX":
        # best = -math.inf  # board's preset value
        for cup_no in valid_moves:
            newBoard = minimax_alpha_beta(distribute(cup_no, board), depth - 1, alpha, beta, hp, parameters, player_to_move)

            if newBoard.get_value() > board.get_value():
                board.set_value(newBoard.get_value())
                board.set_move(cup_no)

            alpha = max(alpha, newBoard.get_value())

            if beta <= alpha:
                break

        return board

    elif board.get_board_type() == "MIN":
        for cup_no in valid_moves:
            newBoard = minimax_alpha_beta(distribute(cup_no, board), depth - 1, alpha, beta, hp, parameters, player_to_move)

            if newBoard.get_value() < board.get_value():
                board.set_value(newBoard.get_value())
                board.set_move(cup_no)

            beta = min(beta, newBoard.get_value())

            if beta <= alpha:
                break

        return board


def playMove(board, depth, hp, parameters):
    new_board = minimax_alpha_beta(board, depth, -math.inf, math.inf, hp, parameters, board.get_board_type())  # Get new board

    if do_print == "print":
        new_board.print_board()
        print("Turn for, ", new_board.get_active_player().get_player_name(), " | Move:", new_board.get_move(), "\n\n")

        if new_board.get_active_player().get_tag() == "MAX" and game_type == "manual":
            move = int(input("Your suggested move is mentioned above. You can also choose otherwise => "))
            new_board.set_move(move)

    new_board = distribute(new_board.get_move(), new_board)

    player1, player2 = new_board.get_players()
    player1.reset_attributes()
    player2.reset_attributes()

    if new_board.isGameOver():
        return gameOver(new_board, do_print)

    return playMove(new_board, depth, hp, parameters)


def InitGame(depth, h_pair, parameters, do_print0, game_type0):
    player1 = PlayerInfo("MAX")  # Player is set as MAX
    player2 = PlayerInfo("MIN")

    boardObj = Board("MAX")
    boardObj.set_players(player1, player2)
    boardObj.set_active_player("MAX")

    global do_print
    global game_type
    do_print = do_print0
    game_type = game_type0

    return playMove(boardObj, depth, h_pair, parameters)
