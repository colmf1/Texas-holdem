#include <iostream>
#include <array>
#include <random>
#include <algorithm>
#include "cards.h"

struct player{
    std::string name;
    int chips;
};

int main() {
    // create deck
    std::array<int, 52> deck = create_deck();
    // shuffle
    std::mt19937 rng(std::random_device{}());
    std::shuffle(deck.begin(), deck.end(), rng);
    
    std::cout << "Player 1's hand:\n";
    print_p1(deck);
    std::cout << "\nPlayer 2's hand:\n";
    print_p2(deck);
    std::cout << "\nFlop:\n";
    print_flop(deck);
    std::cout << "\nTurn:\n";
    print_turn(deck);
    std::cout << "\nRiver:\n";
    print_river(deck);
    return 0;
}
