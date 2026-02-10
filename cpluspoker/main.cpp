#include <iostream>
#include <array>
#include <random>
#include <algorithm>
#include "cards.h"

// changed mind again,
// cache locallity means store all in gamestate
// I need to carry history through 
// optimal way is as a hash table 
// I need to store history bitwise
// All actions gonna be bitwise
// limit raise options to a halfpot,pot and all in
// 

struct Player{
    int id, chips, current_bet;
    std::array<int,2> hand;
};

struct InfoSetKey {
    uint32_t history;
    uint8_t hole0;
    uint8_t hole1;
    uint8_t board0; // maybe bucketed
    uint8_t board1;
    uint8_t board2;
    uint8_t board3;
    uint8_t board4;
};

struct Gamestate{
    // Think i've finally arrived on this.
    std::array<int, 52> deck;
    int hand[2][2];
    int table[5];
    
    int pot;
    int round;
    int chips[2];
    
    int act; // Whos go just varies 0-1 
    uint32_t history; // This needs hardcoded to do bit stuff
}

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

    int action;
    std::cout << "Enter your action \n";
    // Actions 1. Call/Check (bet=0 << action)
    //         2. Raise (bet=bet, << action)

    if (player.current_bet == (game.pot / 2)){
        switch(action) {
            case 1:
                // check - do nothing 
                break;
            case 2:
                // Raise
                // I think I store my actions better
                // Should I store them bitwise?
                // XX YY - YY is action, 1,2,3, 
                // XX is bet, 
                int bet;
                place_bet(player, game, bet)
                break;


        }
        if (action == 1){
            // check is do nothing?
        }
        

        // check
        // raise
        // fold?? Why would anyone fold
        
    }
    // Asks for user input + runs appropriate function
    // User input depends on whether the other opponent has called
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
