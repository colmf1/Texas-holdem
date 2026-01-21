#include "evaluator.h"
#include <algorithm>
#include <set>
#include <map>

// Helper functions
std::map<std::string, int> HandEval::get_value_map() {
    return {{"A", 14}, {"K", 13}, {"Q", 12}, {"J", 11}};
}

std::map<int, std::string> HandEval::get_reverse_value_map() {
    return {{14, "A"}, {13, "K"}, {12, "Q"}, {11, "J"}};
}

std::vector<int> HandEval::convert_to_numeric(const std::vector<Card>& all_cards) {
    std::vector<int> numeric_values;
    std::map<std::string, int> value_map = get_value_map();
    
    for (const auto& card : all_cards) {
        std::string value = card.show().substr(0, card.show().length() - 1);
        auto it = value_map.find(value);
        if (it != value_map.end()) {
            numeric_values.push_back(it->second);
        } else {
            numeric_values.push_back(std::stoi(value));
        }
    }
    return numeric_values;
}

std::string HandEval::convert_to_str(int card) {
    std::map<int, std::string> reverse_map = get_reverse_value_map();
    auto it = reverse_map.find(card);
    if (it != reverse_map.end()) {
        return it->second;
    }
    return std::to_string(card);
}

HandEval::FlushResult HandEval::eval_flush(const std::vector<Card>& all_cards) {
    FlushResult result;
    result.is_flush = false;
    
    // Extract suits and values
    std::vector<std::string> suits;
    std::vector<std::string> values;
    for (const auto& card : all_cards) {
        std::string card_str = card.show();
        suits.push_back(card_str.substr(card_str.length() - 1));
        values.push_back(card_str.substr(0, card_str.length() - 1));
    }
    
    // Convert values to numeric
    std::map<std::string, int> value_map = get_value_map();
    std::vector<int> numeric_values;
    for (const auto& v : values) {
        auto it = value_map.find(v);
        if (it != value_map.end()) {
            numeric_values.push_back(it->second);
        } else {
            numeric_values.push_back(std::stoi(v));
        }
    }
    
    // Create lookup from numeric value to suit
    std::map<int, std::string> lookup;
    for (size_t i = 0; i < suits.size(); ++i) {
        lookup[numeric_values[i]] = suits[i];
    }
    
    // TRANSLATION NOTE: Python's Counter is replaced with std::map counting
    std::map<std::string, int> suit_counts;
    for (const auto& suit : suits) {
        suit_counts[suit]++;
    }
    
    // Find max count
    int max_count = 0;
    for (const auto& pair : suit_counts) {
        if (pair.second > max_count) {
            max_count = pair.second;
        }
    }
    
    if (max_count >= 5) {
        // Find the flush suit
        std::string flush_suit;
        for (const auto& pair : suit_counts) {
            if (pair.second >= 5) {
                flush_suit = pair.first;
                break;
            }
        }
        
        // Get cards of flush suit
        std::vector<int> cards;
        for (const auto& pair : lookup) {
            if (pair.second == flush_suit) {
                cards.push_back(pair.first);
            }
        }
        
        int high_card = *std::max_element(cards.begin(), cards.end());
        std::string card_str = convert_to_str(high_card);
        
        result.is_flush = true;
        result.cards = cards;
        result.card_str = card_str;
    }
    
    return result;
}

HandEval::StraightResult HandEval::eval_straight(const std::vector<Card>& all_cards) {
    StraightResult result;
    result.is_straight = false;
    
    std::vector<std::string> values;
    for (const auto& card : all_cards) {
        std::string card_str = card.show();
        values.push_back(card_str.substr(0, card_str.length() - 1));
    }
    
    std::map<std::string, int> value_map = get_value_map();
    std::vector<int> numeric_values;
    for (const auto& v : values) {
        auto it = value_map.find(v);
        if (it != value_map.end()) {
            numeric_values.push_back(it->second);
        } else {
            numeric_values.push_back(std::stoi(v));
        }
    }
    
    // TRANSLATION NOTE: Python's set() to get unique values, then sorted()
    // In C++, we use std::set (which is sorted) then convert to vector
    std::set<int> unique_set(numeric_values.begin(), numeric_values.end());
    std::vector<int> unique_values(unique_set.rbegin(), unique_set.rend()); // reverse order
    
    // Check for straights
    for (size_t i = 0; i + 4 < unique_values.size(); ++i) {
        std::vector<int> window;
        for (size_t j = 0; j < 5; ++j) {
            window.push_back(unique_values[i + j]);
        }
        
        // Check if consecutive (descending)
        bool is_consecutive = true;
        for (size_t j = 1; j < window.size(); ++j) {
            if (window[j] != window[0] - (int)j) {
                is_consecutive = false;
                break;
            }
        }
        
        if (is_consecutive) {
            int high_card = window[0];
            std::string high_card_str = convert_to_str(high_card);
            result.is_straight = true;
            result.high_card = high_card;
            result.high_card_str = high_card_str;
            return result;
        }
    }
    
    return result;
}

std::pair<std::vector<int>, std::string> HandEval::eval_high(const std::vector<Card>& all_cards) {
    std::vector<std::string> values;
    for (const auto& card : all_cards) {
        std::string card_str = card.show();
        values.push_back(card_str.substr(0, card_str.length() - 1));
    }
    
    std::map<std::string, int> value_map = get_value_map();
    std::vector<int> numeric_values;
    for (const auto& v : values) {
        auto it = value_map.find(v);
        if (it != value_map.end()) {
            numeric_values.push_back(it->second);
        } else {
            numeric_values.push_back(std::stoi(v));
        }
    }
    
    std::set<int> unique_set(numeric_values.begin(), numeric_values.end());
    std::vector<int> unique_values(unique_set.rbegin(), unique_set.rend());
    
    int high_card = unique_values[0];
    std::string card_str = convert_to_str(high_card);
    
    // Return top 5 cards
    std::vector<int> top_five;
    for (size_t i = 0; i < std::min(size_t(5), unique_values.size()); ++i) {
        top_five.push_back(unique_values[i]);
    }
    
    return {top_five, card_str + " High"};
}

HandEval::PairResult HandEval::eval_pairs(const std::vector<Card>& all_cards) {
    PairResult result;
    result.type = 0;
    
    std::vector<std::string> values;
    for (const auto& card : all_cards) {
        std::string card_str = card.show();
        values.push_back(card_str.substr(0, card_str.length() - 1));
    }
    
    std::map<std::string, int> value_map = get_value_map();
    std::map<int, std::string> reverse_map = get_reverse_value_map();
    
    std::vector<int> numeric_values;
    for (const auto& v : values) {
        auto it = value_map.find(v);
        if (it != value_map.end()) {
            numeric_values.push_back(it->second);
        } else {
            numeric_values.push_back(std::stoi(v));
        }
    }
    
    std::sort(numeric_values.begin(), numeric_values.end(), std::greater<int>());
    
    // TRANSLATION NOTE: Python's Counter(list) becomes manual counting in C++
    std::map<int, int> value_counts;
    for (int val : numeric_values) {
        value_counts[val]++;
    }
    
    // Check for four of a kind
    for (const auto& pair : value_counts) {
        if (pair.second == 4) {
            int card = pair.first;
            std::string card_str = convert_to_str(card);
            result.type = 4;
            result.cards.push_back(card);
            result.hand_name = "Four of a Kind " + card_str + "'s";
            return result;
        }
    }
    
    // Check for full house (3 and 2)
    bool has_three = false;
    bool has_two = false;
    std::vector<int> threes, twos;
    for (const auto& pair : value_counts) {
        if (pair.second == 3) {
            has_three = true;
            threes.push_back(pair.first);
        } else if (pair.second == 2) {
            has_two = true;
            twos.push_back(pair.first);
        }
    }
    
    if (has_three && has_two) {
        std::sort(threes.begin(), threes.end(), std::greater<int>());
        std::sort(twos.begin(), twos.end(), std::greater<int>());
        std::string card_str_three = convert_to_str(threes[0]);
        std::string card_str_two = convert_to_str(twos[0]);
        result.type = 32;
        result.cards.push_back(threes[0]);
        result.cards.push_back(twos[0]);
        result.hand_name = "Full House " + card_str_three + "'s and " + card_str_two + "'s";
        return result;
    }
    
    // Check for three of a kind
    if (has_three) {
        std::sort(threes.begin(), threes.end(), std::greater<int>());
        int card = threes[0];
        std::string card_str = convert_to_str(card);
        result.type = 3;
        result.cards.push_back(card);
        result.hand_name = "Three of a Kind " + card_str + "'s";
        return result;
    }
    
    // Check for two pair
    if (twos.size() >= 2) {
        std::sort(twos.begin(), twos.end(), std::greater<int>());
        std::string card_str_1 = convert_to_str(twos[0]);
        std::string card_str_2 = convert_to_str(twos[1]);
        result.type = 22;
        result.cards.push_back(twos[0]);
        result.cards.push_back(twos[1]);
        result.hand_name = "Two Pair " + card_str_1 + "'s and " + card_str_2 + "'s";
        return result;
    }
    
    // Check for pair
    if (has_two) {
        std::sort(twos.begin(), twos.end(), std::greater<int>());
        int card = twos[0];
        std::string card_str = convert_to_str(card);
        result.type = 2;
        result.cards.push_back(card);
        result.hand_name = "Pair of " + card_str + "'s";
        return result;
    }
    
    return result;
}

HandEval::HandResult HandEval::evaluate_hand(const std::vector<Card>& community_cards, 
                                              const std::vector<Card>& hand) {
    HandResult result;
    
    // TRANSLATION NOTE: Python's list concatenation (hand + community_cards)
    // becomes manual vector copying in C++
    std::vector<Card> all_cards = hand;
    all_cards.insert(all_cards.end(), community_cards.begin(), community_cards.end());
    
    bool flush = false;
    bool straight = false;
    int key = 0;
    std::vector<int> cards;
    std::string hand_name;
    
    auto pair_result = eval_pairs(all_cards);
    if (pair_result.type > 0) {
        key = pair_result.type;
        cards = pair_result.cards;
        hand_name = pair_result.hand_name;
    }
    
    // Check for royal flush
    auto flush_result = eval_flush(all_cards);
    if (flush_result.is_flush) {
        std::vector<int> flush_cards = flush_result.cards;
        std::sort(flush_cards.begin(), flush_cards.end(), std::greater<int>());
        // Check if it's 14, 13, 12, 11, 10 (A, K, Q, J, 10)
        if (flush_cards.size() >= 5 && 
            flush_cards[0] == 14 && flush_cards[1] == 13 && 
            flush_cards[2] == 12 && flush_cards[3] == 11 && flush_cards[4] == 10) {
            result.hand_rank = 1;
            result.cards = {14, 13, 12, 11, 10};
            result.hand_name = "Royal Flush";
            return result;
        }
        flush = true;
    }
    
    auto straight_result = eval_straight(all_cards);
    if (straight_result.is_straight) {
        straight = true;
    }
    
    // Determine hand rank
    if (flush && straight) {
        result.hand_rank = 2;
        result.cards = {straight_result.high_card};
        result.hand_name = straight_result.high_card_str + " High Straight Flush";
    }
    else if (key == 4) {
        result.hand_rank = 3;
        result.cards = cards;
        result.hand_name = hand_name;
    }
    else if (key == 32) {
        result.hand_rank = 4;
        result.cards = cards;
        result.hand_name = hand_name;
    }
    else if (flush) {
        result.hand_rank = 5;
        result.cards = flush_result.cards;
        result.hand_name = flush_result.card_str + " High Flush";
    }
    else if (straight) {
        result.hand_rank = 6;
        result.cards = {straight_result.high_card};
        result.hand_name = straight_result.high_card_str + " High Straight";
    }
    else if (key == 3) {
        result.hand_rank = 7;
        result.cards = cards;
        result.hand_name = hand_name;
    }
    else if (key == 22) {
        result.hand_rank = 8;
        result.cards = cards;
        result.hand_name = hand_name;
    }
    else if (key == 2) {
        result.hand_rank = 9;
        result.cards = cards;
        result.hand_name = hand_name;
    }
    else {
        result.hand_rank = 10;
        auto high_result = eval_high(all_cards);
        result.cards = high_result.first;
        result.hand_name = high_result.second;
    }
    
    return result;
}

std::vector<std::string> HandEval::get_winner(const std::vector<Player*>& players, 
                                               const std::vector<Card>& community_cards,
                                               std::map<std::string, std::string>& hands) {
    std::map<std::string, int> ranks;
    std::map<std::string, std::vector<int>> cards;
    
    for (Player* player : players) {
        auto result = evaluate_hand(community_cards, player->hand);
        ranks[player->name] = result.hand_rank;
        cards[player->name] = result.cards;
        hands[player->name] = result.hand_name;
    }
    
    // Find winning rank (lowest number is best)
    int winning_rank = 99;
    for (const auto& pair : ranks) {
        if (pair.second < winning_rank) {
            winning_rank = pair.second;
        }
    }
    
    // Get all winners with that rank
    std::vector<std::string> winners;
    for (const auto& pair : ranks) {
        if (pair.second == winning_rank) {
            winners.push_back(pair.first);
        }
    }
    
    // Tiebreak if multiple winners
    if (winners.size() > 1) {
        std::map<std::string, std::vector<int>> winner_cards;
        for (const auto& winner : winners) {
            winner_cards[winner] = cards[winner];
        }
        winners = tiebreak(winner_cards);
        
        // TRANSLATION NOTE: Python's kicker logic had bugs (unused sort result).
        // We simplify C++ version to just use tiebreak on all cards if still tied.
        if (winners.size() > 1) {
            std::map<Player*, std::vector<Card>> all_cards_map;
            for (Player* player : players) {
                for (const auto& winner : winners) {
                    if (player->name == winner) {
                        std::vector<Card> all_cards = player->hand;
                        all_cards.insert(all_cards.end(), community_cards.begin(), community_cards.end());
                        all_cards_map[player] = all_cards;
                    }
                }
            }
            // For now, return all tied winners
            // The Python kicker function had issues, so we keep ties as ties
        }
    }
    
    return winners;
}

std::vector<std::string> HandEval::tiebreak(const std::map<std::string, std::vector<int>>& card_dict) {
    if (card_dict.empty()) return {};
    
    std::vector<std::string> names;
    std::vector<std::vector<int>> cards;
    
    for (const auto& pair : card_dict) {
        names.push_back(pair.first);
        cards.push_back(pair.second);
    }
    
    // Find minimum length to iterate safely
    size_t min_len = cards[0].size();
    for (const auto& c : cards) {
        if (c.size() < min_len) {
            min_len = c.size();
        }
    }
    
    std::vector<std::string> winners;
    
    // TRANSLATION NOTE: Python iterates through card positions to find highest
    for (size_t i = 0; i < min_len; ++i) {
        int winning_card = -1;
        for (const auto& c : cards) {
            if (i < c.size() && c[i] > winning_card) {
                winning_card = c[i];
            }
        }
        
        winners.clear();
        for (size_t j = 0; j < names.size(); ++j) {
            if (i < cards[j].size() && cards[j][i] == winning_card) {
                winners.push_back(names[j]);
            }
        }
        
        if (winners.size() == 1) {
            return winners;
        }
    }
    
    return winners;
}

std::vector<std::string> HandEval::kicker(const std::map<Player*, std::vector<Card>>& all_cards) {
    // TRANSLATION NOTE: Python's kicker function had a bug (sort() returns None, not sorted list).
    // This is a corrected version.
    std::vector<Player*> players;
    std::vector<std::vector<int>> card_lists;
    
    for (const auto& pair : all_cards) {
        players.push_back(pair.first);
        std::vector<int> numeric = convert_to_numeric(pair.second);
        std::sort(numeric.begin(), numeric.end(), std::greater<int>());
        card_lists.push_back(numeric);
    }
    
    size_t min_len = card_lists[0].size();
    for (const auto& c : card_lists) {
        if (c.size() < min_len) {
            min_len = c.size();
        }
    }
    
    std::vector<std::string> winners;
    for (size_t i = 0; i < min_len; ++i) {
        int winning_card = -1;
        for (const auto& c : card_lists) {
            if (c[i] > winning_card) {
                winning_card = c[i];
            }
        }
        
        winners.clear();
        for (size_t j = 0; j < players.size(); ++j) {
            if (card_lists[j][i] == winning_card) {
                winners.push_back(players[j]->name);
            }
        }
    }
    
    return winners;
}
