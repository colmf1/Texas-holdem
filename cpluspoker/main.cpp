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
    bool allin=false; // If someone goes all in, and the other calls this is triggered
    std::array<int, 5> table; 
    Player player1;
    Player player2;
}

void place_bet(Player& player,
               Gamestate& game,
               int bet){
    // Check if bet > chips
    // Check if bet > 0
    // player.chips - bet
    // game.pot + bet 
}

void fold(Player& player, 
          Gamestate&game){
    // I think we want to pass in the non folder, and update thier chips, reset pot to 0
}

void call(Player& player,
          Gamestate& game){
    // 
}

void raise(Player& player,
           Gamestate& game){

}

void show_pot(Gamestate& game)

void show_table(Gamestate& game)

void handle_action(Player& player,
                   Gamestate& game){
    // Asks for user input + runs appropriate function
    // Call
    // Fold
    // Raise - need to limit this to N big blinds
    // All in
}

void preflop(Player& big,
             Player& small,
             Gamestate& game, 
             ){
    // small
    // display cards,
    // Ask for action
    // Handle action

    // big - repeat above

    // at any point after this, a check ends the round.
    // I don't think round can end on a call


    // player1 - if action is a
    
}

int main() {
    // deck
    std::array<int, 52> deck = create_deck();
    shuffle_deck(deck.begin, deck.end);
    
    // players
    Player player1 {0, 5000, 0};
    Player player2 {1, 5000, 0}; 
    
    Gamestate game();

    // Post blinds

    // Preflop round

    // Flop

    // Turn

    // River

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
