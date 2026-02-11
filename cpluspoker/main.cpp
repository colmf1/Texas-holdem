#include <iostream>
#include <array>
#include <random>
#include <algorithm>

// Don't pass in pointers - pass in the actual gamestate 
// Pointers are low memory, but cause bouncing about in memory
// If we minimise the size of our gamestate we can just copy
// Asked claude about cache locallity - keeping the deck will
// just pollute the cache anyway - keep it separate, we only use it to deal cards
//
// struct Gamestate{    
//     // Deck is just 52 number array
//
//     // hands stored together as a matrix
//     // p0=hand[0][0:1]
//     int hand[2][2];
//     int table[5];
//
//     int pot, round;
//     int chips[2];
//     int bets[2];
//
//     bool p0;
//     uint32_t history; // This needs to be 32 bit encoded for bit stuff
//     // history gonna be the focus of this
//     // we need to track the actions for as far back as possible
//     // minimise memory usage
//     //
// };
//

struct Gamestate{
    int hand[2][2];
    int table[5];
    int pot, round;
    int chips[2];
    int bets[2];
    bool p0; // p0 true/false - to track current player
    uint32_t history; 
};

void change_player(Gamestate& game){
    game.p0 = !game.p0;
}

void init_gs(Gamestate& game){
    game.pot = 0;
    game.round = 0;
    game.p0 = false;
}

std::array<int, 52> create_deck(){
    std::array<int, 52> deck;
    for (int i = 0; i<52; i++){
        deck[i] = i;
    }
    return deck;
}

void shuffle_deck(std::array<int,52>& deck);
    std::mt19937 rng(std::random_device{}());
    std::shuffle(deck.begin(), deck.end(), rng);
}

void deal_cards(Gamestate& game, std::array<int,52>& deck){
    shuffle_deck(deck);
    // hands
    game.hand[0][0] = deck[0];
    game.hand[1][0] = deck[1];
    game.hand[0][1] = deck[2];
    game.hand[1][1] = deck[3];
    // flop-turn-river
    game.table[0] = deck[4];
    game.table[1] = deck[5];
    game.table[2] = deck[6];
    game.table[3] = deck[7];
    game.table[4] = deck[8];
}

void card_str(int card){
    std::array<std::string, 4>  suits ={
        "Spades", "Clubs", "Hearts", "Diamonds"
    };
    std::array<std::string, 13> ranks = {
        "2","3","4","5","6","7","8","9", "10", "Jack", "Queen", "King", "Ace"
    };
    std::cout << ranks[card/4] << " of " << suits[card/12] << "\n";
}

int main() {
    // init
    Gamestate game;
    std::array<int, 52> deck = create_deck();
    game.chips[0] = 5000;
    game.chips[1] = 5000;

    while (true){
        // only break after run out of chips.
        // give a quit option with the other actions for now 
        init_gs(game);
        deal_cards(game, deck);
        //blinds 
        game.bets[game.p0]  = 1;
        game.bets[!game.p0] = 2;

        // preflop

        std::cout << "Bets - 0:" << game.bets[game.p0] << ",1:" << game.bets[game.p0] << "\n";
        std::cout << "Hand\n";
        card_str(game.hand[0][0]); 
        card_str(game.hand[0][1]);
        std::cout << "\n";


        
        // Ask for input 
        // Check if it's a fold or call round 
        // Make a map of inputs to actions - do it later
        int action;
        std::cout << "0:fold \n1:call \n2:raisehalf \n3:raisepot \n4:Allin\n"; 
        std::cin >> action;
        std::cout << "Action was " << action << "\n";
        return 0;
        } 
}
