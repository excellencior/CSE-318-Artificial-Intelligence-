import random
from mancalaHome import InitGame

heuristic_list = [1, 2, 3, 4]
heuristic_parameters = [0, 1, (1, 2), (1, 2, 3), (1, 2, 3, 4)]
heuristic_pairings = []
depth = 3


def start_fight(game_cnt, do_print, game_type):
    for h_pair in heuristic_pairings:
        player1_win_count = 0
        player2_win_count = 0
        draw_count = 0

        for game_no in range(game_cnt):
            player1, player2 = InitGame(depth, h_pair, [heuristic_parameters[h_pair[0]], heuristic_parameters[h_pair[1]]], do_print, game_type)

            if player1.stones_in_my_storage() > player2.stones_in_my_storage():
                player1_win_count += 1
            elif player1.stones_in_my_storage() < player2.stones_in_my_storage():
                player2_win_count += 1
            else:
                draw_count += 1

        f = open("log.txt", "a")
        f.write(f"""
        
        -----------Statistic----------
        Heuristic IDs: {h_pair[0]} vs {h_pair[1]}
        Heuristic Parameters: {heuristic_parameters[h_pair[0]]} and {heuristic_parameters[h_pair[1]]}
        depth: {depth}
        # Player1 won: {player1_win_count} times with heuristic {h_pair[0]}
        # Player2 won: {player2_win_count} times with heuristic {h_pair[1]}
        # Match drawn: {draw_count}""")
        f.close()

        print("\n-----------Statistic----------")
        print("Heuristic ID:", {h_pair[0]}, "vs", {h_pair[1]})
        print("# Player1 won:", player1_win_count, "times with heuristic", {h_pair[0]})
        print("# Player2 won:", player2_win_count, "times with heuristic", {h_pair[1]})
        print("# Match drawn:", draw_count)


def Game():
    print("""
        Pick One
        1: Play AI vs AI (100 games)
        2: Play AI vs AI (1 game)
        3: Play Human vs AI

        Note: Player-1 is favored as MAX and Player-2 is favored as MIN # fixed #""")

    sel = int(input("Enter choice: "))

    if sel == 1:
        heuristic_pairings.clear()

        for i in heuristic_list:
            for j in heuristic_list:
                if i != j:
                    heuristic_pairings.append((i, j))

        start_fight(100, "noprint", "auto")

    elif sel == 2:
        heuristic_pairings.clear()

        hid_1 = int(input("Heuristic for player1 [1-4]: "))
        hid_2 = int(input("Heuristic for player2 [1-4]: "))

        heuristic_pairings.append((hid_1, hid_2))
        start_fight(1, "print", "auto")

    elif sel == 3:
        sel = int(input("Level of difficulty (1 Easy) (2 Medium) (3 Hard): "))
        global depth  # Default is 5

        if sel == 1:
            depth = 3
        elif sel == 2:
            depth = 5
        else:
            depth = 10

        hid_1 = int(input("Enter your choice of heuristic [1-4]: "))
        hid_2 = int(input("Enter opponent's choice of heuristic [1-4]: "))

        heuristic_pairings.append((hid_1, hid_2))

        start_fight(1, "print", "manual")

        depth = 5


Game()
