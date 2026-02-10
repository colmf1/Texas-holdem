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
    // Maybe this should be a class
    int SBlind=1;
    int BBlind=2;
    bool allin=false; // If someone goes all in, and the other calls this is triggered
    std::array<int, 5> table; 
    Player player1;
    Player player2;
}
// Lets pretend player isn't in this class

void place_bet(Player& player,
               Gamestate& game,
               int bet;){
    player.chips -= bet;
    player.current_bet += bet;
    game.pot += bet;
    // This must eventually be limited by the other persons chips - can't put them 
}

void end_round(Player& folder,
               Player& winner, 
               Gamestate& game){
    // set_round = n
    // clear both bets
    winner.chips += game.pot;
    
    game.pot = 0;
    winner.current_bet=0;
    folder.current_bet=0;
}

void call(Player& player,
          Gamestate& game){
    place_bet(player, gamestate, (player.current_bet-(game.pot / 2)));
}

void raise(Player& player,
           Gamestate& game){
    // call and raise are redundant with place bet, do it in handle action.
}

void show_pot(Gamestate& game)

void show_table(Gamestate& game)

void handle_action(Player& player,
                   Gamestate& game){
    ""
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
