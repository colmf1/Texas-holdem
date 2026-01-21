#include "game.h"
#include "evaluator.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <limits>

// TRANSLATION NOTE: In Python, class variables are defined in the class body.
// In C++, static members must be defined outside the class (in the .cpp file).
const std::vector<std::string> Card::SUITS = {"♠", "♥", "♦", "♣"};
const std::vector<std::string> Card::RANKS = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

// TRANSLATION NOTE: Python's dict comprehension {rank: i for i, rank in enumerate(RANKS)}
// is replaced with explicit map initialization in C++.
const std::map<std::string, int> Card::RANK_VALUES = {
    {"2", 0}, {"3", 1}, {"4", 2}, {"5", 3}, {"6", 4}, {"7", 5},
    {"8", 6}, {"9", 7}, {"10", 8}, {"J", 9}, {"Q", 10}, {"K", 11}, {"A", 12}
};

// Card implementation
Card::Card(const std::string& suit, const std::string& rank) 
    : suit(suit), rank(rank) {}

std::string Card::show() const {
    return rank + suit;
}

// Deck implementation
Deck::Deck() {
    // TRANSLATION NOTE: Python's list comprehension with nested loops:
    // [Card(suit, rank) for suit in Card.SUITS for rank in Card.RANKS]
    // In C++, we use nested for loops to achieve the same result.
    for (const auto& suit : Card::SUITS) {
        for (const auto& rank : Card::RANKS) {
            cards.push_back(Card(suit, rank));
        }
    }
    shuffle();
}

void Deck::shuffle() {
    // TRANSLATION NOTE: Python uses random.shuffle() which shuffles in-place.
    // C++ uses std::shuffle with a random number generator.
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

Card* Deck::deal() {
    // TRANSLATION NOTE: Python's list.pop() returns None if empty, handled with ternary.
    // C++ doesn't have built-in Option/Maybe types, so we return a pointer (nullptr for empty).
    // Alternative would be std::optional<Card> in C++17+, but raw pointer is simpler here.
    if (cards.empty()) {
        return nullptr;
    }
    // Note: Returning pointer to local Card would be bad. We need to allocate on heap.
    // IMPORTANT: Caller must manage memory! This is a key Python vs C++ difference.
    Card* card = new Card(cards.back());
    cards.pop_back();
    return card;
}

// Player implementation
Player::Player(const std::string& name, int chips) 
    : name(name), chips(chips), bet(0), folded(false) {}

void Player::add_card(const Card& card) {
    // TRANSLATION NOTE: Python's list.append() is equivalent to vector.push_back()
    hand.push_back(card);
}

void Player::clear_hand() {
    // TRANSLATION NOTE: Python's self.hand = [] creates new list.
    // C++ vector.clear() empties existing vector.
    hand.clear();
}

int Player::place_bet(int amount) {
    // TRANSLATION NOTE: Python's min() function works the same in C++ (std::min)
    amount = std::min(amount, chips);
    chips -= amount;
    bet += amount;
    return amount;
}

// TexasHoldemGame implementation
TexasHoldemGame::TexasHoldemGame() 
    : deck(), pot(0), dealer_index(0), small_blind(5), big_blind(10) {}

void TexasHoldemGame::add_player(const std::string& name, int chips) {
    // TRANSLATION NOTE: Python uses list.append(), C++ uses vector.push_back()
    players.push_back(Player(name, chips));
}

void TexasHoldemGame::deal_hole_cards() {
    for (int i = 0; i < 2; ++i) {
        for (auto& player : players) {
            Card* card = deck.deal();
            if (card) {
                player.add_card(*card);
                delete card;  // TRANSLATION NOTE: Manual memory management in C++!
            }
        }
    }
}

void TexasHoldemGame::deal_community_cards(int num) {
    for (int i = 0; i < num; ++i) {
        Card* card = deck.deal();
        if (card) {
            community_cards.push_back(*card);
            delete card;  // TRANSLATION NOTE: Must delete heap-allocated memory
        }
    }
}

void TexasHoldemGame::collect_blinds() {
    size_t small_blind_index = (dealer_index + 1) % players.size();
    size_t big_blind_index = (dealer_index + 2) % players.size();
    
    pot += players[small_blind_index].place_bet(small_blind);
    pot += players[big_blind_index].place_bet(big_blind);
    
    std::cout << players[small_blind_index].name << " posts small blind: $" << small_blind << std::endl;
    std::cout << players[big_blind_index].name << " posts big blind: $" << big_blind << std::endl;
}

int TexasHoldemGame::betting_round(int initial_bet, int button, bool preFlop) {
    size_t starting_index;
    
    if (preFlop) {
        starting_index = (dealer_index + 3) % players.size();
    } else {
        starting_index = button;
    }
    
    int current_bet = initial_bet;
    size_t current_index = starting_index;
    
    while (true) {
        // TRANSLATION NOTE: Python's list comprehension becomes a counting loop in C++
        int active_count = 0;
        for (const auto& p : players) {
            if (!p.folded && p.chips > 0) {
                active_count++;
            }
        }
        if (active_count == 1) {
            return current_index;
        }
        
        Player& player = players[current_index];
        if (!player.folded && player.chips > 0) {
            int call_amount = current_bet - player.bet;
            
            if (call_amount > 0 && call_amount < player.chips) {
                bool behaving_badly = true;
                while (behaving_badly) {
                    std::cout << player.name << ", your hand: " 
                              << player.hand[0].show() << " " << player.hand[1].show() << "\n";
                    
                    std::cout << "Community cards: [";
                    for (size_t i = 0; i < community_cards.size(); ++i) {
                        std::cout << community_cards[i].show();
                        if (i < community_cards.size() - 1) std::cout << ", ";
                    }
                    std::cout << "]\n";
                    
                    std::cout << "Current bet: $" << current_bet << ", Your bet: $" << player.bet << "\n";
                    std::cout << "Pot: $" << pot << "\n";
                    std::cout << "(C)all $" << call_amount << ", (R)aise, or (F)old? ";
                    
                    std::string action;
                    std::cin >> action;
                    // TRANSLATION NOTE: Python's str.lower() becomes manual check or tolower
                    std::transform(action.begin(), action.end(), action.begin(), ::tolower);
                    
                    if (action == "c") {
                        int bet = player.place_bet(call_amount);
                        pot += bet;
                        std::cout << player.name << " calls $" << bet << std::endl;
                        behaving_badly = false;
                    }
                    else if (action == "r") {
                        std::cout << "Raise to how much? (Minimum: $" << current_bet * 2 << ") ";
                        int raise_amount;
                        std::cin >> raise_amount;
                        
                        // TRANSLATION NOTE: C++ input validation needs error checking
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Invalid input. Please enter a number.\n";
                            continue;
                        }
                        
                        if (raise_amount >= current_bet * 2) {
                            int bet = player.place_bet(raise_amount - player.bet);
                            pot += bet;
                            current_bet = raise_amount;
                            starting_index = current_index;
                            std::cout << player.name << " raises to $" << raise_amount << std::endl;
                            behaving_badly = false;
                        } else {
                            std::cout << "Invalid raise amount. Treating as a call.\n";
                            int bet = player.place_bet(call_amount);
                            pot += bet;
                            std::cout << player.name << " calls $" << bet << std::endl;
                            behaving_badly = false;
                        }
                    }
                    else if (action == "f") {
                        player.folded = true;
                        std::cout << player.name << " folds" << std::endl;
                        behaving_badly = false;
                    }
                    else {
                        std::cout << "Invalid input, get it right omg..." << std::endl;
                    }
                }
            }
            else {
                // Can check or raise
                bool behaving_badly = true;
                while (behaving_badly) {
                    std::cout << player.name << ", your hand: " 
                              << player.hand[0].show() << " " << player.hand[1].show() << "\n";
                    
                    std::cout << "Community cards: [";
                    for (size_t i = 0; i < community_cards.size(); ++i) {
                        std::cout << community_cards[i].show();
                        if (i < community_cards.size() - 1) std::cout << ", ";
                    }
                    std::cout << "]\n";
                    
                    std::cout << "Current bet: $" << current_bet << ", Your bet: $" << player.bet << "\n";
                    std::cout << "Pot: $" << pot << "\n";
                    std::cout << "(C)heck, (R)aise, or (F)old? ";
                    
                    std::string action;
                    std::cin >> action;
                    std::transform(action.begin(), action.end(), action.begin(), ::tolower);
                    
                    if (action == "r") {
                        std::cout << "Raise to how much? (Minimum: $" << current_bet * 2 << ") ";
                        int raise_amount;
                        std::cin >> raise_amount;
                        
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Invalid input. Please enter a number.\n";
                            continue;
                        }
                        
                        if (raise_amount >= current_bet * 2) {
                            int bet = player.place_bet(raise_amount - player.bet);
                            pot += bet;
                            current_bet = raise_amount;
                            std::cout << player.name << " raises to $" << raise_amount << std::endl;
                            starting_index = current_index;
                            behaving_badly = false;
                        } else {
                            std::cout << "Invalid raise amount, get your act together" << std::endl;
                        }
                    }
                    else if (action == "f") {
                        player.folded = true;
                        std::cout << player.name << " folds" << std::endl;
                        behaving_badly = false;
                    }
                    else if (action == "c") {
                        std::cout << player.name << " checks" << std::endl;
                        behaving_badly = false;
                    }
                    else {
                        std::cout << "Invalid action. Will you please get it right" << std::endl;
                    }
                }
            }
            
            current_index = (current_index + 1) % players.size();
            if (current_index == starting_index) {
                // TRANSLATION NOTE: Python's all() with generator expression
                // becomes a loop in C++ checking conditions
                bool all_called = true;
                for (const auto& p : players) {
                    if (!(p.bet == current_bet || p.folded || p.chips == 0)) {
                        all_called = false;
                        break;
                    }
                }
                if (all_called) {
                    std::cout << "All Called" << std::endl;
                    return current_index;
                }
            }
        }
        else {
            current_index = (current_index + 1) % players.size();
            if (current_index == starting_index) {
                bool all_called = true;
                for (const auto& p : players) {
                    if (!(p.bet == current_bet || p.folded || p.chips == 0)) {
                        all_called = false;
                        break;
                    }
                }
                if (all_called) {
                    std::cout << "All Called" << std::endl;
                    return current_index;
                }
            }
        }
    }
}

TexasHoldemGame::WinnerResult TexasHoldemGame::evaluate_winner() {
    WinnerResult result;
    result.single_winner_by_fold = false;
    
    // TRANSLATION NOTE: Python list comprehension becomes explicit loop
    std::vector<Player*> active_players;
    for (auto& p : players) {
        if (!p.folded) {
            active_players.push_back(&p);
        }
    }
    
    if (active_players.size() == 1) {
        result.winners.push_back(active_players[0]);
        result.single_winner_by_fold = true;
        return result;
    }
    
    HandEval ev;
    auto winner_names = ev.get_winner(active_players, community_cards, result.hands);
    
    // Convert names back to Player pointers
    for (const auto& name : winner_names) {
        for (auto& p : players) {
            if (p.name == name) {
                result.winners.push_back(&p);
                break;
            }
        }
    }
    
    return result;
}

void TexasHoldemGame::play_round() {
    // TRANSLATION NOTE: Python creates new Deck object, C++ reassigns with new Deck
    deck = Deck();
    community_cards.clear();
    pot = 0;
    
    for (auto& player : players) {
        player.clear_hand();
        player.bet = 0;
        player.folded = false;
    }
    
    std::cout << "\n" << players[dealer_index].name << " is the dealer" << std::endl;
    
    collect_blinds();
    deal_hole_cards();
    
    std::cout << "\nPre-flop betting round:" << std::endl;
    int button = betting_round(big_blind, 99, true);
    
    // TRANSLATION NOTE: Python's sum() with generator becomes counting loop
    int active_count = 0;
    for (const auto& p : players) {
        if (!p.folded) active_count++;
    }
    
    if (active_count > 1) {
        std::cout << "\nDealing the flop:" << std::endl;
        deal_community_cards(3);
        button = betting_round(players[0].bet, button);
    }
    
    active_count = 0;
    for (const auto& p : players) {
        if (!p.folded) active_count++;
    }
    
    if (active_count > 1) {
        std::cout << "\nDealing the turn:" << std::endl;
        deal_community_cards(1);
        button = betting_round(players[0].bet, button);
    }
    
    active_count = 0;
    for (const auto& p : players) {
        if (!p.folded) active_count++;
    }
    
    if (active_count > 1) {
        std::cout << "\nDealing the river:" << std::endl;
        deal_community_cards(1);
        betting_round(players[0].bet, button);
    }
    
    std::cout << "\nShowdown:" << std::endl;
    auto result = evaluate_winner();
    
    std::cout << "Community Cards: [";
    for (size_t i = 0; i < community_cards.size(); ++i) {
        std::cout << community_cards[i].show();
        if (i < community_cards.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
    
    for (const auto& player : players) {
        if (!player.folded) {
            std::cout << player.name << "'s hand: " 
                      << player.hand[0].show() << " " << player.hand[1].show() << "\t"
                      << result.hands[player.name] << std::endl;
        }
    }
    
    // Determine winner(s)
    active_count = 0;
    for (const auto& p : players) {
        if (!p.folded) active_count++;
    }
    
    if (active_count > 1) {
        if (result.winners.size() > 1) {
            std::cout << "Draw! Pot will be split" << std::endl;
            int split_amount = pot / result.winners.size();
            for (auto* w : result.winners) {
                w->chips += split_amount;
            }
        } else {
            std::cout << "\n" << result.winners[0]->name << " wins $" << pot 
                      << " with " << result.hands[result.winners[0]->name] << std::endl;
            result.winners[0]->chips += pot;
        }
    } else {
        Player* winner = result.winners[0];
        std::cout << "\n" << winner->name << " wins $" << pot << std::endl;
        winner->chips += pot;
    }
    
    dealer_index = (dealer_index + 1) % players.size();
}
