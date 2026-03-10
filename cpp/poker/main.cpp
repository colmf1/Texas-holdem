#include "hand_eval.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <random>

struct Gamestate {
  int hand[2][2];
  int table[5];
};

std::array<int, 52> create_deck() {
  std::array<int, 52> deck;
  for (int i = 0; i < 52; i++) {
    deck[i] = i;
  }
  return deck;
}

void shuffle_deck(std::array<int, 52> &deck) {
  std::mt19937 rng(std::random_device{}());
  std::shuffle(deck.begin(), deck.end(), rng);
}

void deal_cards(Gamestate &game, std::array<int, 52> deck) {
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

static const char *ranks[] = {"2", "3",  "4", "5", "6", "7", "8",
                              "9", "10", "J", "Q", "K", "A"};

static const std::array<std::string, 4> suits = {"Spades", "Clubs", "Hearts",
                                                 "Diamonds"};

void print_hand(uint64_t hand) {
  while (hand) {
    int card_idx =
        __builtin_ctzll(hand); // counts trailing 0s,(ctz) ll for int64
    hand &= hand - 1;          // removes the bit we just checked
    std::cout << ranks[card_idx % 13] << " of " << suits[card_idx / 13] << "\n";
  }
}

int main() {
  std::array<int, 52> deck = create_deck();
  Gamestate game;
  deal_cards(game, deck);
  uint64_t hand0 = bitboard(game.hand[0], 2);
  uint64_t table = bitboard(game.table, 5);

  uint64_t cards = hand0 | table;
  std::cout << "Hand:\n";
  print_hand(hand0);
  std::cout << "\n";

  std::cout << "Table:\n";
  print_hand(table);
  std::array<int, 4> res = eval_hand(cards);

  std::cout << "No Pairs:" << res[0] << "\n";
  std::cout << "TOAK1/FOAK2:" << res[1] << "\n";
  std::cout << "No straight:" << res[2] << "\n";
  std::cout << "No flush:" << res[3] << "\n";
  return 0;
}
