#include "game.h"
#include <iostream>
#include <algorithm>
#include <limits>

int main() {
    TexasHoldemGame game;
    
    std::cout << "Enter the number of players (2-8): ";
    int num_players;
    std::cin >> num_players;
    
    // TRANSLATION NOTE: Python's input() with f-string becomes std::cin in C++
    // Error handling is more explicit in C++
    if (std::cin.fail() || num_players < 2 || num_players > 8) {
        std::cerr << "Invalid number of players!" << std::endl;
        return 1;
    }
    
    // Clear the newline left in the buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    for (int i = 1; i <= num_players; ++i) {
        std::cout << "Enter name for Player " << i << ": ";
        std::string name;
        std::getline(std::cin, name);
        
        std::cout << "Starting chips: 500" << std::endl;
        int chips = 500;
        game.add_player(name, chips);
    }
    
    // TRANSLATION NOTE: Python's while True becomes while(true) in C++
    while (true) {
        game.play_round();
        
        // TRANSLATION NOTE: Python's list comprehension for filtering
        // [p for p in game.players if p.chips > 0]
        // In C++, we modify the vector in place using erase-remove idiom
        auto& players = game.get_players();
        players.erase(
            std::remove_if(players.begin(), players.end(),
                [](const Player& p) { return p.chips <= 0; }),
            players.end()
        );
        
        if (players.size() < 2) {
            std::cout << "Game over! Not enough players to continue." << std::endl;
            if (!players.empty()) {
                std::cout << players[0].name << " Has Won!" << std::endl;
            }
            break;
        }
        
        std::cout << "\nDo you want to play another round? (y/n): ";
        std::string play_again;
        std::cin >> play_again;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        // TRANSLATION NOTE: Python's str.lower() becomes manual tolower transformation
        std::transform(play_again.begin(), play_again.end(), play_again.begin(), ::tolower);
        
        if (play_again != "y") {
            break;
        }
    }
    
    std::cout << "\nFinal chip counts:" << std::endl;
    
    // TRANSLATION NOTE: Python's sorted() with lambda key function
    // In C++, we use std::sort with a custom comparator
    auto& players = game.get_players();
    std::sort(players.begin(), players.end(),
        [](const Player& a, const Player& b) {
            return a.chips > b.chips;  // Descending order
        });
    
    for (const auto& player : players) {
        std::cout << player.name << ": $" << player.chips << std::endl;
    }
    
    std::cout << "Thanks for playing!" << std::endl;
    
    return 0;
}
