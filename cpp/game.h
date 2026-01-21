#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <map>

// Forward declaration
class Card;
class Deck;
class Player;
class HandEval;

class Card {
public:
    // TRANSLATION NOTE: In Python, SUITS and RANKS were class variables defined at class level.
    // In C++, we use static const to achieve similar behavior - shared across all instances.
    static const std::vector<std::string> SUITS;
    static const std::vector<std::string> RANKS;
    static const std::map<std::string, int> RANK_VALUES;
    
    std::string suit;
    std::string rank;
    
    Card(const std::string& suit, const std::string& rank);
    std::string show() const;
};

class Deck {
private:
    std::vector<Card> cards;
    
public:
    Deck();
    void shuffle();
    // TRANSLATION NOTE: Python's pop() returns None if empty. In C++, we use a pointer
    // to return nullptr for empty deck instead of dealing with exceptions.
    Card* deal();
};

class Player {
public:
    std::string name;
    int chips;
    std::vector<Card> hand;
    int bet;
    bool folded;
    
    Player(const std::string& name, int chips);
    void add_card(const Card& card);
    void clear_hand();
    int place_bet(int amount);
};

class TexasHoldemGame {
private:
    Deck deck;
    std::vector<Player> players;
    std::vector<Card> community_cards;
    int pot;
    size_t dealer_index;
    int small_blind;
    int big_blind;
    
public:
    TexasHoldemGame();
    
    void add_player(const std::string& name, int chips);
    void deal_hole_cards();
    void deal_community_cards(int num);
    void collect_blinds();
    
    // TRANSLATION NOTE: Python allows default arguments with mutable defaults (like button=99).
    // C++ handles defaults similarly but we declare them in header.
    int betting_round(int initial_bet, int button = 99, bool preFlop = false);
    
    // TRANSLATION NOTE: Python's dynamic return types (single player vs list of players and hands)
    // are not possible in C++. We return a struct that contains both possibilities.
    struct WinnerResult {
        std::vector<Player*> winners;
        std::map<std::string, std::string> hands;
        bool single_winner_by_fold;
    };
    WinnerResult evaluate_winner();
    
    void play_round();
    
    // Getters for accessing private members (needed for evaluator)
    std::vector<Player>& get_players() { return players; }
    const std::vector<Card>& get_community_cards() const { return community_cards; }
};

#endif // GAME_H
