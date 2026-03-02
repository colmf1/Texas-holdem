# -*- coding: utf-8 -*-
"""
Hand Evaluator 

"""
from collections import Counter

class HandEval:
    # These two haven't been used, but should be 
    def convert_to_numeric(all_cards):
        values = [card.show()[:-1] for card in all_cards]
        value_map = {'A': 14, 'K': 13, 'Q': 12, 'J': 11}
        return list(map(int, [value_map.get(v, v) for v in values]))
    def convert_to_str(cards):
        value_map = {'A': 14, 'K': 13, 'Q': 12, 'J': 11}
        reverse_map = {v: k for k, v in value_map.items()}
        def convert(card):
            return reverse_map.get(card, str(card))
        if isinstance(cards, list):
            return [convert(card) for card in cards]
        else:
            return convert(cards)
        
        

    def eval_flush(self,all_cards):
        suits = [card.show()[-1] for card in all_cards]
        values = [card.show()[:-1] for card in all_cards]
        value_map = {'A': 14, 'K': 13, 'Q': 12, 'J': 11}
        numeric_values = list(map(int, [value_map.get(v, v) for v in values]))
        lookup = {numeric_values[i]: suits[i] for i in range(len(suits))}
        suit_counts = Counter(suits)
        if max(suit_counts.values()) >= 5: 
            flush_suit = [key for key, value in suit_counts.items() if value >= 5][0]
            cards = [card for card, suit in lookup.items() if suit == flush_suit]
            high_card = max(cards)
            card_str = next(k for k, v in value_map.items() if v == high_card) if high_card in value_map.values() else str(high_card)
            return cards, card_str
        else:
            return False
        
    def eval_straight(self,all_cards):
        values = [card.show()[:-1] for card in all_cards]
        value_map = {'A': 14, 'K': 13, 'Q': 12, 'J': 11}
        numeric_values = list(map(int, [value_map.get(v, v) for v in values]))
        unique_values = sorted(set(numeric_values), reverse=True)
        for i in range(len(unique_values)-4):
            window = unique_values[i:i + 5]
            if all(window[j] == window[0] + j for j in range(5)):
                high_card = max(window)
                high_card_str = next(k for k, v in value_map.items() if v == high_card) if high_card in value_map.values() else str(high_card)
                return high_card, high_card_str
            else:
                return False
    
    def eval_high(self,all_cards):
        values = [card.show()[:-1] for card in all_cards]
        value_map = {'A': 14, 'K': 13, 'Q': 12, 'J': 11}
        numeric_values = list(map(int, [value_map.get(v, v) for v in values]))
        unique_values = sorted(set(numeric_values), reverse=True)
        high_card = max(unique_values)
        card_str = next(k for k, v in value_map.items() if v == high_card) if high_card in value_map.values() else str(high_card)
        return unique_values[0:5], f"{card_str} High"
            
    def eval_pairs(self,all_cards):
        values = [card.show()[:-1] for card in all_cards]
        value_map = {'A': 14, 'K': 13, 'Q': 12, 'J': 11}
        reverse_map = {v: k for k, v in value_map.items()}
        numeric_values = list(map(int, [value_map.get(v, v) for v in values]))
        numeric_values.sort(reverse=True)
        value_counts = Counter(numeric_values)
        
        if 4 in value_counts.values():
            card = [key for key, value in value_counts.items() if value == 4][0]
            card_str = next(k for k, v in value_map.items() if v == card) if card in value_map.values() else str(card)
            return 4,  card, f"Four of a Kind {card_str}'s"
        
        elif set(value_counts.values()) == {3, 2}:
            card = [key for key, value in value_counts.items() if value == {3,2}]
            card_str = [reverse_map.get(c, str(c)) for c in card]   
            return 32, card, f"Full House {card_str[0]}'s and {card_str[1]}'s"
        
        elif 3 in list(value_counts.values()):
            card = [key for key, value in value_counts.items() if value == 3][0]
            card_str = next(k for k, v in value_map.items() if v == card) if card in value_map.values() else str(card)
            return 3,  card, f"Three of a Kind {card_str}'s"
        
        elif list(value_counts.values()).count(2) == 2:
            card = [key for key, value in value_counts.items() if value == 2]
            card_str = [reverse_map.get(c, str(c)) for c in card]
            return 22, card, f"Two Pair {card_str[0]}'s and {card_str[1]}'s"
        
        elif 2 in value_counts.values():
            card = [key for key, value in value_counts.items() if value == 2][0]
            card_str = next(k for k, v in value_map.items() if v == card) if card in value_map.values() else str(card)
            return 2,  card, f"Pair of {card_str}'s"
        
        else:
            return 0
        
        
    def evaluate_hand(self,community_cards, hand):
        all_cards = hand + community_cards   
        flush = False
        straight = False
        key = 0
        
        if self.eval_pairs(all_cards):
            key, cards, hand_name = self.eval_pairs(all_cards)
        
        if self.eval_flush(all_cards) == [14, 13, 12, 11, 10]:
            return 1, [14, 13, 12, 11, 10], "Royal Flush"
        elif self.eval_flush(all_cards):
            flush = True 
        if self.eval_straight(all_cards):
            straight = True 
        if flush & straight:
            hand_rank = 2
            cards, card_str = self.eval_straight(all_cards)
            hand_name = f"{card_str} High Straight Flush"
        elif key == 4: 
            hand_rank = 3
        elif key == 32:
            hand_rank = 4
        elif flush:
            hand_rank = 5
            cards, card_str = self.eval_flush(all_cards)
            hand_name = f"{card_str} High Flush"
        elif straight:
            hand_rank = 6
            cards, card_str = self.eval_straight(all_cards)
            hand_name = f"{card_str} High Straight"
        elif key == 3: 
            hand_rank = 7
        elif key == 22:
            hand_rank = 8
        elif key == 2:
            hand_rank = 9
        else:
            hand_rank = 10
            cards, hand_name = self.eval_high(all_cards) 
        return hand_rank, cards, hand_name
    
    def get_winner(self, players, community_cards):
        ranks = {}
        cards = {}
        hands = {}
        names = {}
        for player in players: 
            names[player.name] = player
            ranks[player.name], cards[player.name], hands[player.name] = self.evaluate_hand(community_cards, player.hand)

        winning_rank = min(ranks.values())
        winners = [k for k, v in ranks.items() if v == winning_rank]
        if len(winners) > 1:
            winners = self.tiebreak(cards)
            if len(winners) > 1:
                all_cards = {names[w]: (names[w].hand + community_cards).sort() for w in winners}
                winners = self.kicker(all_cards)
                if len(winners) > 1:
                    return list(winners)
        return list(winners), hands
        
    def tiebreak(self,card_dict):
        names = list(card_dict.keys())
        cards = list(card_dict.values())
        if isinstance(cards[0], list):
            for i in range(len(min(cards, key=len))):
                #winning_card = max(c[i] for c in cards) # Looping through cards
                winning_card = max(c[i] for c in cards if i < len(c))
                winners = [name for name,c, in zip(names,cards) if c[i] == winning_card]
                if len(winners) == 1:
                    return(winners)
            return winners
        else:
            winning_card = max(cards)
            winners = [i for i in card_dict if card_dict[i] == winning_card]
            return winners
        
        
    def kicker(self,all_cards):
        names = list(all_cards.keys())
        cards = list(all_cards.values())
        card_list = []
        
        for hand in all_cards.values():
            hand_vals = [card.show()[:-1] for card in all_cards]
            value_map = {'A': 14, 'K': 13, 'Q': 12, 'J': 11}
            numeric_values = list(map(int, [value_map.get(v, v) for v in hand_vals])).sort(reverse=True)
            card_list.append(numeric_values)
            
        for i in range(min(len(c) for c in card_list)):
            winning_card = max(c[i] for c in card_list)
            winners = [name for name,c in zip(names,cards) if c[i] == winning_card]
        return winners
    
    