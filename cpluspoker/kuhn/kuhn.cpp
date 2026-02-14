
#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <string>

// Kuhn poker has 3 cards
// Jack < Queen < King
// Options are pass/bet

// This is the most annoying thing ever
// I've just discovered it...
// I know im gonna be writing a ton of int arrays
// I can just fastcode them with arr

// This is first line in my helper funcs script
template <std::size_t N> using arr = std::array<int, N>;

// Instead of
// std::array<int, 2> hand;
// Now I can use like below
// arr<2> hand;

// Cards
arr<3> create_deck() {
  arr<3> deck;
  for (int i = 0; i < 3; i++) {
    deck[i] = i;
  }
  return deck;
}

void shuffle_deck(arr<3> &deck) {
  std::mt19937 rng(std::random_device{}());
  std::shuffle(deck.begin(), deck.end(), rng);
}

struct Gamestate {
  arr<3> hand;
  arr<3> chips;
  bool player;
};

// Get action

int play_round(Gamestate &game) {
  // Show card
  // Bet or pass
  // p1 sees card, p0s action
  // bet or pass
  // bet+bet showdown
  // pass+pass draw
  // bet+pass p0 win
  // pass+bet+pass p1 win
  // pass+bet+bet showdown
  return 0;
}

int main() {
  // Deck
  arr<3> deck = create_deck();
  Gamestate game;

  int playing = 1;
  while (true) {
    // deal
    shuffle_deck(deck);
    game.hand = {deck[0], deck[1]};

    while (playing) {
      playing = play_round(game);
    }
  }
  return 0;
}
