# -*- coding: utf-8 -*-
"""
Created on Tue Aug 13 23:17:30 2024

@author: FearonC
"""


from game import TexasHoldemGame

if __name__ == "__main__":
    game = TexasHoldemGame()
    
    num_players = int(input("Enter the number of players (2-8): "))
    for i in range(1, num_players + 1):
        name = input(f"Enter name for Player {i}: ")
        print("Starting chips: 500")
        chips = 500
        game.add_player(name, chips)
        
    while True:
        game.play_round()
        game.players = [p for p in game.players if p.chips > 0]
        
        if len(game.players) < 2:
            print("Game over! Not enough players to continue.")
            print(game.players, "Has Won!")
            break
        
        play_again = input("\nDo you want to play another round? (y/n): ")
        if play_again.lower() != 'y':
            break
    print("\nFinal chip counts:")
    for player in sorted(game.players, key=lambda p: p.chips, reverse=True):
        print(f"{player.name}: ${player.chips}")

    print("Thanks for playing!")
    
    