#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <random>

constexpr std::array<int8_t, 16> make_wheel_lookup() {
  std::array<int8_t, 16> lookup{};
  for (int i = 0; i < 16; i++) {
    int count = 0;
    while (count < 4 && (i >> count & 1))
      count++;
    lookup[i] = count;
  }
  return lookup;
}

constexpr uint64_t MASK13 = 0x1FFF;
constexpr uint16_t MASK4 = 0xF;
constexpr uint64_t SWITCH = 1;
static constexpr std::array<int8_t, 16> WHEEL_LOOKUP = make_wheel_lookup();

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

uint64_t board(int card_idx) { return SWITCH << card_idx; }

uint64_t bitboard(int *cards, int n) {
  uint64_t res = 0;
  for (int i = 0; i < n; i++) {
    res |= board(cards[i]);
  }
  return res;
}

const uint64_t suit_masks[4] = {MASK13, MASK13 << 13, MASK13 << 26,
                                MASK13 << 39};

uint64_t rank_mask(int rank) {
  return (SWITCH << rank) | (SWITCH << (rank + 13)) | (SWITCH << (rank + 26)) |
         (SWITCH << (rank + 39));
}

void count_matches(uint64_t hand, std::array<int, 5> &evals) {
  for (int rank = 0; rank < 13; rank++) {
    uint64_t mask = rank_mask(rank);
    int num_matches = __builtin_popcountll(hand & mask);
    if (num_matches > 1) {
      evals[num_matches - 2] += 1;
    }
  }
}

void count_straight(uint64_t hand, std::array<int, 5> &evals) {
  uint16_t ranks = ((hand)&MASK13) | ((hand >> 13) & MASK13) |
                   ((hand >> 26) & MASK13) | ((hand >> 39) & MASK13);

  uint16_t r2 = ranks & (ranks >> 1);
  uint16_t r3 = r2 & (ranks >> 2);
  uint16_t r4 = r3 & (ranks >> 3);
  uint16_t r5 = r4 & (ranks >> 4);

  int low = WHEEL_LOOKUP[ranks & 0xF];
  int wheel = (ranks >> 12 & 1) && low ? low + 1 : 0;

  evals[3] = std::max(1 + !!r2 + !!r3 + !!r4 + !!r5, wheel);
}

void count_flush(uint64_t hand, std::array<int, 5> &evals) {
  int max_suit = 0;
  for (int suit = 0; suit < 3; suit++) {
    int count = __builtin_popcountll(hand & suit_masks[suit]);
    if (count > max_suit) {
      max_suit = count;
    }
  }
  evals[4] = max_suit;
}

std::array<int, 5> eval_hand(uint64_t hand) {
  std::array<int, 5> evals = {0, 0, 0, 0, 0};
  count_matches(hand, evals);
  count_straight(hand, evals);
  count_flush(hand, evals);
  return evals;
}

static const char *ranks[] = {"2", "3",  "4", "5", "6", "7", "8",
                              "9", "10", "J", "Q", "K", "A"};
static const std::array<std::string, 4> suits = {"Spades", "Clubs", "Hearts",
                                                 "Diamonds"};

void print_hand(uint64_t hand) {
  while (hand) {
    int card_idx = __builtin_ctzll(hand); // grabs first non 0
    hand &= hand - 1;                     // removes the bit we just checked
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
  std::cout << "Table:\n";
  print_hand(table);
  std::array<int, 5> res = eval_hand(cards);

  std::cout << "No Pairs:" << res[0] << "\n";
  std::cout << "No toak:" << res[1] << "\n";
  std::cout << "No foak:" << res[2] << "\n";
  std::cout << "No straight:" << res[3] << "\n";
  std::cout << "No flush:" << res[4] << "\n";
  return 0;
}
