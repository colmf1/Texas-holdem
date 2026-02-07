#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <algorithm>

int get_rank(int card){
    return card >> 2;
}

int get_suit(int card){
    return card & 0b11;
}

std::string card_str(int card_int){
    std::array<std::string, 4>  suits ={
        "Spades", "Clubs", "Hearts", "Diamonds"
    };
    std::array<std::string, 13> ranks = {
        "2","3","4","5","6","7","8","9", "10", "Jack", "Queen", "King", "Ace"
    };

    std::string rank, suit;
    rank = ranks[get_rank(card_int)];
    suit = suits[get_suit(card_int)];
    return rank + " of " + suit;
}

void print_cards(std::array<int> cards, int num_cards) {
    for (int i = 0; i < num_cards, i++){
         std::cout << card_str(card) << "\n";
         }
}

void print_board(std::array<int> table_cards, int game_state) {
    for (int i; i<game_state+3; i++)
        std::cout << card_str(table_cards[i]) << "\n";
}

int main() {
    
    // init
    std::array<int, 52> deck;
    for (int i=0; i<52; i++) {
        deck[i] = i;
    }

    print_cards(deck,52);
    
    // shuffle
    std::mt19937 rng(std::random_device{}());
    std::shuffle(deck.begin(), deck.end(), rng);
    
    print_cards(deck,52);
    
    // deal
    // We don't need this
    // Since the cards are shuffled no need to actually simulate the games, 
    // we can just select cards by the index order on which they'd been dealt
    // std::array<int, 2> hand1={deck[0],deck[2]};
    // std::array<int, 2> hand2={deck[1],deck[3]};
    // std::array<int, 5> board={deck[4],deck[5],deck[6], deck[7], deck[8]};
    //


    
   //  std::array<int, 52> deck;
   //  for (int i=0; i<52; i++){
   //      std::cout << std::to_string(i) << ": ";
   //      int rank = i/4;
   //      int suit = i%4;
   //
   //      // When defining our card, We take the bits of the rank, i.e. 3 = 11, and move them over by 2
   //      // Then we concatenate suit, 3 of spades = 1100
   //      deck[i] = (rank << 2) | suit;
   //      std::cout << std::to_string(rank) << std::to_string(suit) << " = " << 
   //          // You get the rank and suit back by these 
   //          // For rank, we just chuck off the suit bits with >> 2 just the reverse
   //          // For suit, we just mask all but the last two bits with this magic 
   //          // & 0b11 = mask all but the last two bits
   //          // & 3 also works, because 0b means binary 3
   //          std::to_string(deck[i] >> 2) << std::to_string(deck[i] & 0b11) << "\n";
   //  }
    return 0;
}
