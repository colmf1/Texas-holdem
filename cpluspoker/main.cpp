#include <iostream>
#include <string>
#include <array>
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

void print_card(const std::array<int, 52>& deck, int index){
    std::cout << card_str(deck[index]) << "\n";
}

void print_flop(const std::array<int, 52>& deck){
    print_card(deck, 4);
    print_card(deck, 5);
    print_card(deck, 6);
}

void print_turn(const std::array<int, 52>& deck){
    print_flop(deck);
    print_card(deck, 7);
}

void print_river(const std::array<int, 52>& deck){
    print_turn(deck);
    print_card(deck, 8);
}

void print_p1(const std::array<int, 52>& deck){
    print_card(deck, 0);
    print_card(deck, 2); 
}

void print_p2(const std::array<int, 52>& deck){
    print_card(deck, 1);
    print_card(deck, 3);
}

std::array<int, 52> create_deck(){
    std::array<int, 52> deck;
    for (int i=0; i<52; i++){
        int rank = i/4;
        int suit = i%4;
        deck[i] = (rank << 2) | suit;
    }
    return deck;
}

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
