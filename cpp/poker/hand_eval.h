#pragma once
#include <array>
#include <cstdint>

uint64_t bitboard(int *cards, int n);
std::array<int, 5> eval_hand(uint64_t hand);
