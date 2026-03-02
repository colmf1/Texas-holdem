# -*- coding: utf-8 -*-
"""
Texas Holdem game
"""

from evaluator import HandEval
import random

class Card:
    SUITS = ['♠', '♥', '♦', '♣']
    RANKS = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']
    RANK_VALUES = {rank: i for i, rank in enumerate(RANKS)}
    
    def __init__(self, suit, rank):
        self.suit = suit
        self.rank = rank
    
    def show(self):
        return f"{self.rank}{self.suit}"    
    
class Deck:
    def __init__(self):
        self.cards = [Card(suit, rank) for suit in Card.SUITS for rank in Card.RANKS]
        self.shuffle()

    def shuffle(self):
        random.shuffle(self.cards)

    def deal(self):
        return self.cards.pop() if self.cards else None
    
class Player:
    def __init__(self, name, chips):
        self.name = name
        self.chips = chips
        self.hand = []
        self.bet = 0
        self.folded = False

    def add_card(self, card):
        self.hand.append(card)

    def clear_hand(self):
        self.hand = []

    def place_bet(self, amount):
        amount = min(amount, self.chips)
        self.chips -= amount
        self.bet += amount
        return amount

class TexasHoldemGame:
    def __init__(self):
        self.deck = Deck()
        self.players = []
        self.community_cards = []
        self.pot = 0
        self.dealer_index = 0
        self.small_blind = 5
        self.big_blind = 10
    
    def add_player(self, name, chips):
        self.players.append(Player(name, chips))
        
    def deal_hole_cards(self):
        for _ in range(2):
            for player in self.players:
                card = self.deck.deal()
                if card:
                    player.add_card(card)
    
    def deal_community_cards(self, num):
        for _ in range(num):
            card = self.deck.deal()
            if card:
                self.community_cards.append(card)

    def collect_blinds(self):
        small_blind_index = (self.dealer_index + 1) % len(self.players)
        big_blind_index = (self.dealer_index + 2) % len(self.players)

        self.pot += self.players[small_blind_index].place_bet(self.small_blind)
        self.pot += self.players[big_blind_index].place_bet(self.big_blind)

        print(f"{self.players[small_blind_index].name} posts small blind: ${self.small_blind}")
        print(f"{self.players[big_blind_index].name} posts big blind: ${self.big_blind}")

    def betting_round(self, initial_bet, button = 99, preFlop = False):                
        if preFlop:
            starting_index = (self.dealer_index + 3) % len(self.players)
        else:
            starting_index = button
                                   
        current_bet = initial_bet
        current_index = starting_index
        
        while True:
            active_players = [p for p in self.players if not p.folded and p.chips > 0]
            if len(active_players) == 1:
                return
            
            player = self.players[current_index]
            if not player.folded and player.chips > 0:
                call_amount = current_bet - player.bet
                if (call_amount > 0) and (call_amount < player.chips):
                    behaving_badly = True
                    while behaving_badly:
                        action = input(f"{player.name}, your hand: {player.hand[0].show()} {player.hand[1].show()}\n"
                                       f"Community cards: {[self.community_cards[i].show() for i in range(len(self.community_cards))]}\n"
                                       f"Current bet: ${current_bet}, Your bet: ${player.bet}\n"
                                       f"Pot: ${self.pot}\n"
                                       f"(C)all ${call_amount}, (R)aise, or (F)old? ").lower()
                        
                        if action == 'c':                    
                            bet = player.place_bet(call_amount)
                            self.pot += bet
                            print(f"{player.name} calls ${bet}")
                            behaving_badly = False
                            
                        elif action == 'r':
                            raise_amount = int(input(f"Raise to how much? (Minimum: ${current_bet * 2}) "))                            
                            if raise_amount >= current_bet * 2:
                                bet = player.place_bet(raise_amount - player.bet)
                                self.pot += bet
                                current_bet = raise_amount
                                starting_index = current_index
                                print(f"{player.name} raises to ${raise_amount}")
                                behaving_badly = False
                                
                            else:
                                print("Invalid raise amount. Treating as a call.")
                                bet = player.place_bet(call_amount)
                                self.pot += bet
                                print(f"{player.name} calls ${bet}")
                                behaving_badly = False
                                
                        elif action == 'f':
                            player.folded = True
                            print(f"{player.name} folds")
                            behaving_badly = False
                            
                        else:
                            print("Invalid input, get it right omg...")
                
                else: 
                    behaving_badly = True
                    while behaving_badly:
                        action = input(f"{player.name}, your hand: {player.hand[0].show()} {player.hand[1].show()}\n"
                                       f"Community cards: {[self.community_cards[i].show() for i in range(len(self.community_cards))]}\n"
                                       f"Current bet: ${current_bet}, Your bet: ${player.bet}\n"
                                       f"Pot: ${self.pot}\n"
                                       f"(C)heck, (R)aise, or (F)old? ").lower()
                        
                        if action == 'r':
                            raise_amount = int(input(f"Raise to how much? (Minimum: ${current_bet * 2}) "))
                            if raise_amount >= current_bet * 2:
                                bet = player.place_bet(raise_amount - player.bet)
                                self.pot += bet
                                current_bet = raise_amount
                                print(f"{player.name} raises to ${raise_amount}")
                                starting_index = current_index
                                behaving_badly = False
                            else:
                                print("Invalid raise amount, get your act together")
                                
                        elif action == 'f':
                            player.folded = True
                            print(f"{player.name} folds")
                            behaving_badly = False
                            
                        elif action == 'c':
                            print(f"{player.name} checks")
                            behaving_badly = False
                            
                        else:
                            print("Invalid action. Will you please get it right")

                current_index = (current_index + 1) % len(self.players) 
                if current_index == starting_index:
                    all_called = all(p.bet == current_bet or p.folded or p.chips == 0 for p in self.players)
                    if all_called:
                        print("All Called")
                        return current_index
        
    def evaluate_winner(self):
        active_players = [p for p in self.players if not p.folded]
        names = {}
        for player in active_players: 
            names[player.name] = player
        ev = HandEval()
        if len(active_players) == 1:
            return active_players[0]
        winner, hands = ev.get_winner(active_players, self.community_cards)
        return [names[key] for key in winner], hands
        
    def play_round(self):
        self.deck = Deck()
        self.community_cards = []
        self.pot = 0
        for player in self.players:
            player.clear_hand()
            player.bet = 0
            player.folded = False
        print(f"\n{self.players[self.dealer_index].name} is the dealer")

        self.collect_blinds()
        self.deal_hole_cards()

        print("\nPre-flop betting round:")
        button = self.betting_round(self.big_blind, preFlop=True)
        
        if sum(not p.folded for p in self.players) > 1:
            print("\nDealing the flop:")
            self.deal_community_cards(3)
            button = self.betting_round(self.players[0].bet, button)
        
        if sum(not p.folded for p in self.players) > 1:
            print("\nDealing the turn:")
            self.deal_community_cards(1)
            button=self.betting_round(self.players[0].bet,button)

        if sum(not p.folded for p in self.players) > 1:
            print("\nDealing the river:")
            self.deal_community_cards(1)
            self.betting_round(self.players[0].bet, button)
            
        print("\nShowdown:")
        hands = self.evaluate_winner()[1]
        print(f"Community Cards: {[card.show() for card in self.community_cards]}")
        for player in self.players:
            if not player.folded:
                print(f"{player.name}'s hand: {player.hand[0].show()} {player.hand[1].show()}\t"
                      f"{hands[player.name]}")
        
        active_players = [p for p in self.players if not p.folded]
        if len(active_players) > 1:
            winner, hands = self.evaluate_winner()
            final_winner = [w.name for w in winner]
            if len(final_winner) > 1:
                print("Draw! Pot will be split")
                split_amount = self.pot/len(final_winner)
                for w in winner:
                    w.chips += split_amount
            else:
                print(f"\n{winner[0].name} wins ${self.pot} with {hands[winner[0].name]}")
                winner[0].chips += self.pot
        else:
            winner = active_players[0]
            print(f"\n{winner.name} wins ${self.pot}")

            
        self.dealer_index = (self.dealer_index + 1) % len(self.players)
        
        self.dealer_index = (self.dealer_index + 1) % len(self.players)
