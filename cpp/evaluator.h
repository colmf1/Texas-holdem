#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "game.h"
#include <string>
#include <vector>
#include <map>

class HandEval {
public:
    // Helper struct to return evaluation results
    struct HandResult {
        int hand_rank;
        std::vector<int> cards;
        std::string hand_name;
    };
    
    // TRANSLATION NOTE: Python methods that return tuples or complex types
    // are converted to structs in C++ for type safety
    struct FlushResult {
        bool is_flush;
        std::vector<int> cards;
        std::string card_str;
    };
    
    struct StraightResult {
        bool is_straight;
        int high_card;
        std::string high_card_str;
    };
    
    struct PairResult {
        int type;  // 0=none, 2=pair, 22=two pair, 3=three of a kind, 32=full house, 4=four of a kind
        std::vector<int> cards;
        std::string hand_name;
    };
    
    // Main evaluation functions
    FlushResult eval_flush(const std::vector<Card>& all_cards);
    StraightResult eval_straight(const std::vector<Card>& all_cards);
    std::pair<std::vector<int>, std::string> eval_high(const std::vector<Card>& all_cards);
    PairResult eval_pairs(const std::vector<Card>& all_cards);
    
    HandResult evaluate_hand(const std::vector<Card>& community_cards, const std::vector<Card>& hand);
    
    // TRANSLATION NOTE: Python returns dynamic types (single player or list + dict).
    // C++ uses explicit return type with vector of names and map of hands
    std::vector<std::string> get_winner(const std::vector<Player*>& players, 
                                        const std::vector<Card>& community_cards,
                                        std::map<std::string, std::string>& hands);
    
    std::vector<std::string> tiebreak(const std::map<std::string, std::vector<int>>& card_dict);
    std::vector<std::string> kicker(const std::map<Player*, std::vector<Card>>& all_cards);
    
private:
    // Helper functions for card value conversion
    static std::vector<int> convert_to_numeric(const std::vector<Card>& all_cards);
    static std::string convert_to_str(int card);
    static std::map<std::string, int> get_value_map();
    static std::map<int, std::string> get_reverse_value_map();
};

#endif // EVALUATOR_H
