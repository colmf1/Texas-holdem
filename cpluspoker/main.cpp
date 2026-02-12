#include <iostream>
#include <array>
#include <random>
#include <algorithm>


// Human io

std::string card_str(int card){
    // I should just return a string?
    std::array<std::string, 4>  suits ={
        "Spades", "Clubs", "Hearts", "Diamonds"
    };
    std::array<std::string, 13> ranks = {
        "2","3","4","5","6","7","8","9", "10", "Jack", "Queen", "King", "Ace"
    };
    return to_string(ranks[card/4] | " of " | to_string(suits[card/12]);
}

void show_bets(Gamestate& game){
    std::cout << "Bets - 0:" << game.bets[game.id] << ", 1:" << game.bets[game.id] << "\n";
}

void show_hand(Gamestate& game){
        if (game.round != 0){
            std::cout << "Table\n";
        for (int i = 0; i < game.round + 2; i++){
            std::cout << card_str(game.table[i]) << "\n";
            }
        }
    std::cout << "Hand\n" << card_str(game.hand[player][0]) << "\n" << card_str(game.hand[player][1]) << "\n\n";
}

void show_info(Gamestate& game, int player){
    show_bets(game);
    show_hands(game);
}

void show_options(Gamestate& game, bool allin=0){
    if (allin){
        std::cout << "0:fold \n1:call"
    }else{
        if (game.bets[0] != game.bets[1]){
            std::cout << "0:fold \n1:call \n2:raisehalf \n3:raisepot \n4:Allin\n"; 
        }else{
            std::cout << "0:fold \n1:check \n2:raisehalf \n3:raisepot \n4:Allin\n"; 
        }
    }    
}

void print_allin(Gamestate& game){
}

int get_input(){
    // not passing limits because that will be done elsewhere
    int x;
    std::cin >> x;
    return x;
}


// Game

struct Gamestate{
    int hand[2][2];
    int table[5];
    int round;
    int chips[2];
    int bets[2];
    bool id; 
    bool player;
    uint32_t history; // deal with this later 
};

// Cards
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


void deal_cards(Gamestate& game, std::array<int,52>& deck){
    shuffle_deck(deck);
    
    game.hand[0][0] = deck[0];
    game.hand[1][0] = deck[1];
    game.hand[0][1] = deck[2];
    game.hand[1][1] = deck[3];
    
    game.table[0] = deck[4];
    game.table[1] = deck[5];
    game.table[2] = deck[6];
    game.table[3] = deck[7];
    game.table[4] = deck[8];
}

// Gamestate
void init_gs(Gamestate& game){
    game.round = 0;
    game.id = 0;
    game.player = 0;
    game.chips[0] = 5000;
    game.chips[1] = 5000;
    
}

void fold(Gamestate& game){
    chips[!game.player] += pot;
    bresets(game);
}

void call(Gamestate& game){
    game.chips[player] -= (game.bets[!player] - game.bets[player]);; 
    game.bets[player]  += (game.bets[!player] - game.bets[player]);; 
    // Pass over action - this needs to go back to the start of the function
    game.player = !game.player;
}

void bethpot(Gamestate& game){
    game.chips[player] -= (game.bet[player] + game.bet[!player])/2;
    game.bet[player]   += (game.bet[player] + game.bet[!player])/2;
    game.player = !game.player;
}

void betfpot(Gamestate& game){
    game.chips[player] -= game.bet[player] + game.bet[!player];
    game.bet[player]   += game.bet[player] + game.bet[!player];    
    game.player = !game.player;
}

void bresets(Gamestate& game){
    // reset bets
    game.bets[0] = 0;
    game.bets[1] = 0;
}

bool all_in(Gamestate& game){
    show_options(game, 1);
    int action = get_input());
    if (action == 0){
        fold();
    }else{
        call();
    }
}

bool play_round(Gamestate& game){
    // This function plays the rounds
    // It exits at Fold and All in and skips to evaluation
    // Any other action will change the player and rerun the function
    // I don't think it's going to work, a check resets the player,
    // a call or raise advances it 
    // all in, fold don't matter
    int action;
    show_info(game);
    action = get_input();
    
    if (action == 0){
        fold(game); 
        return 0;
    }

    if (action == 1){
        if (game.bets[0] == game.bets[1]){
            // check - resets player to small blind and advances the round 
            game.player = !game.bb
            game.round+=1;
            return 1;
        }else{
            // call - Matches bet and passes action
            call(game);
            return 1;
        }
    }
    
    // Raise - halfpot, fullpot
    // places bet and passes player
    // Will mask these at a higher level i.e. if chips aren't good enough
    // dont want to do it now because it will be bot eventually 
    if (action == 2){
        bethpot(game);
        return 1;
    }

    if (action == 3){
        betfpot(game);
        return 1;
    }

    if (action == 4){
        int response;
        allin(game);
        return 0;
        }
    }


int main() {
    
    // init
    Gamestate game;
    std::array<int, 52> deck = create_deck();
    init_gs(game)

    bool bb = 0;
    while (true){
        game.player = bb;
        game.round = 0;
        deal_cards(game, deck);

        bool folded;
        while (!folded){
            folded = play_round(game);
        }
        // folded needs scrapped now I think? I don't understand memory
        // only way of getting here is all in, folded, or showdown.

        // Folded ends round instantly

        bb = !bb;
        return 0;

    }
}


