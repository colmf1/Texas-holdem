#include "hand_eval.h"
#include <algorithm>
#include <array>
#include <cstdint>

namespace {

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

uint64_t board(int card_idx) { return SWITCH << card_idx; }

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

} // namespace

uint64_t bitboard(int *cards, int n) {
  uint64_t res = 0;
  for (int i = 0; i < n; i++) {
    res |= board(cards[i]);
  }
  return res;
}

std::array<int, 5> eval_hand(uint64_t hand) {
  std::array<int, 5> evals = {0, 0, 0, 0, 0};
  count_matches(hand, evals);
  count_straight(hand, evals);
  count_flush(hand, evals);
  return evals;
}
