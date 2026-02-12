#include <iostream>
#include <array>
#include <random>
#include <algorithm>

struct Gamestate{
    int hand[2][2];
    int table[5];
    int pot, round;
    int chips[2];
    int bets[2];
    bool id; 
    bool bb;
    uint32_t history; 
};

void init_gs(Gamestate& game){
    game.pot = 0;
    game.round = 0;
    game.id = 0;
    game.bb = 0;
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

std::string card_str(int card){
    // I should just return a string?
    std::array<std::string, 4>  suits ={
        "Spades", "Clubs", "Hearts", "Diamonds"
    };
    std::array<std::string, 13> ranks = {
        "2","3","4","5","6","7","8","9", "10", "Jack", "Queen", "King", "Ace"
    };
    return to_string(ranks[card/4] | " of " | to_string(suits[card/12])
}

void show_bets(Gamestate& game)
    std::cout << "Bets - 0:" << game.bets[game.id] << ", 1:" << game.bets[game.id] << "\n";
}

void show_hand(Gamestate& game, int player){
   if (round != 0){
        std::cout << "Table\n";
        for (int i = 0; i < round + 2; i++){
            std::cout << card_str(game.table[i]) << "\n";
            }
        } 
    
    std::cout << "Hand\n" << card_str(game.hand[player][0]) << "\n" << card_str(game.hand[player][1]) << "\n\n"
}


void show_info(Gamestate& game, int player){
    show_bets(game);
    show_hands(game);
        // I should make show bets, show hand and show table which uses round 
        std::cout << "Hand\n";
        card_str(game.hand[player][0]); 
        card_str(game.hand[player][1]);
        std::cout << "\n";

        // table cards too 
 
    }
}

void handle_action(Gamsetate& game){
    // placeholder for bot action
    
    // instead of checking for call, check if bets are equal.
    // call and check will be identical to model
    // Show info function
    int action;
    if (game.bets[0]==game.bets[1]){
        std::cout << "0:fold \n1:call \n2:raisehalf \n3:raisepot \n4:Allin\n"; 
    }else{
        std::cout << "0:fold \n1:check \n2:raisehalf \n3:raisepot \n4:Allin\n"; 
    }
    std::cin >> action;
    // if action is 0, run function end round
    // add pot to the winning player !id or whatever
    // ends the round while loop
    // 
    // 
    // there should be encoding here to separate call and check
}

void handle_action(int action){

}

void preflop(Gamestate& game){
    while (true) {
        // Ill fix it later
        handle_action(game);

    }
}

int main() {
    // init
    Gamestate game;
    std::array<int, 52> deck = create_deck();
    game.chips[0] = 5000;
    game.chips[1] = 5000;

    while (true){
        bool bb = 0;
        
        // only break after run out of chips.
        // give a quit option with the other actions for now 
        init_gs(game);
        deal_cards(game, deck);
        //blinds 
        game.bets[!game.bb] = 2;
        game.bets[game.bb]  = 1;

        

        // preflop
        // function that takes gamestate as input and shits these out 
               if (action == 0){
            // ends the round - we need another while 
        }
        if (action == 1){
            // if it's a call, end the round since we have only two players
        }
        // doesn't exist yet 
        check_endgame();
        bb = !bb;
        return 0;
    }
}


