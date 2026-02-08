#include <iostream>
#include <array>
#include <random>
#include <algorithm>
#include "cards.h"


// This is my new structure, Player and gamestate structs handle everything
// These are easily passed into bot
// Write functions to take Player and gamestate, nothing else is needed

struct Player{
    int id, chips, current_bet;
    std::array<int,2> hand;
};

struct Gamestate{
    int pot=0; 
    int round=0;
    bool allin=0; // If someone goes all in, and the other calls this is triggered
    std::array<int, 5> table; 
}


void place_bet(Player& player,
               Gamestate& game,
               int bet){
    // Check if bet > chips
    // Check if bet > 0
    // player.chips - bet
    // game.pot + bet 
}

void fold(Player& player){
    // I think we want to pass in the non folder, and update thier chips, reset pot to 0
}

// When resetting gamestate I should figure out a smart way to do it

void handle_action(Player& player,
                   Gamestate& game){
    // Asks for user input + runs appropriate function
    // Call
    // Fold
    // Raise - need to limit this to N big blinds
    // All in
}

int main() {
    // create deck
    std::array<int, 52> deck = create_deck();
    Player player1 {0, 5000, 0};
    Player player2 {1, 5000, 0}; 
    Gamestate game {}

    shuffle_deck(deck.begin, deck.end);

    // Dealing cards is irrelevent, we know the indexes and can hardcode them
    // One players BB, other SB, 
    // BB forced to bet 2 chips
    // SB forced to bet 1 chip

    // SB sees gamestate gets option to raise call or fold
    // Gamestate is 2 hole cards no table cards
    // Also holds bets for both players

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
