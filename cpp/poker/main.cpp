
// deckboard
// I make a uint64
// 0-12 are clubs
// 0 is an ace
// 13 is an ace etc
// So my deck is just
// 52 1s with 12 trailing 0s = 64 bits
// 1111111111111111111111111111111111111111111111111111000000000000
// 4 aces
// 1000000000000100000000000010000000000001000000000000000000000000
// a hand is a bitboard, and I flag the indeces for the cards

// define a card - 1ULL << index, puts a 1 at index
// This is our mask
// hand |= mask - deals a card
// Since it's OR, and mask is only 1 at the new cards position
// hand|mask 0|1 = 1, 1|0 = 1 - It adds without changing 1s to 0s
// Just use not for removing
// hand &= ~mask
// our mask will be a 1, so it changes
// removing turning it off
// & for matches

#include <algorithm>
#include <array>
#include <cstdint>
#include <random>

uint64_t board(int card_idx) { return 1ULL << card_idx; }

// Mask 13 bits
// this is just 1s, so we can check for collisions
// ULL needed for 64 bit
const uint64_t CLUB_MASK = 0x1FFFULL;
const uint64_t DIAMOND_MASK = 0x1FFFULL << 13;
const uint64_t HEART_MASK = 0x1FFFULL << 26;
const uint64_t SPADE_MASK = 0x1FFFULL << 39;

uint64_t RANK_MASK(int rank) {
  return (1ULL << rank) | (1ULL << (rank + 13)) | (1ULL << (rank + 26)) |
         (1ULL << (rank + 39));
}

// Cards
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

struct Gamestate {
  int hand[2][2];
  int table[5];
};

void deal_cards(Gamestate &game, std::array<int, 52> &deck) {
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

int main() { return 0; }
