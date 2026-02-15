
#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <string>

// Kuhn poker has 3 cards
// Jack < Queen < King
// Options are pass/bet

// I also needa sort tracking history in other class

// This is the most annoying thing ever
// I've just discovered it...
// I know im gonna be writing a ton of int arrays
// I can just fastcode them with arr

// This is first line in my helper funcs script
template <std::size_t N> using arr = std::array<int, N>;
// print - not wise
// got from goog - needa figure out the ...s - seems self explanatory
template <typename... Args> void print(Args &&...args) {
  ((std::cout << args << " "), ...);
  std::cout << '\n';
}

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
  arr<2> card;
  int player;
  arr<2> action;
  int round;
  arr<4> history;
};

// I need to enumerate every single info state

// All possible infostates for both players
// p0
// Initial - 0 + (0, 1, 2)
// After pass 3 + (0,1, 2)
// After bet 6 + (0,1,2)

// p1
// Initial - 9 + (0, 1, 2)
// After pass 12 + (0,1, 2)
// After bet 15 + (0,1,2)

// From this 2 possible actions always.
//

// history +=

// load of other combos? Do I needa mad for loop r smt
// need to make unique ints

int get_action() {
  // I need to know prev actions to do this
  int action;
  std::cin >> action;
  return action;
}

void showdown(Gamestate &game) {
  // very simple just compare cards?
}

int play_round(Gamestate &game) {
  // This round ends and changes the active player
  // Update the player every time we move
  if (game.round == 3) {
    showdown(game);
    return 0;
  }

  print("Player: ", game.player);
  print(game.card[game.player]);
  print("Bet or Pass");
  int action = get_action();

  // should we be using game.round?
  // we lit needa track history
  // history = [0,0,0]
  // Needa distinguish p1 pass vs p2 pass - different information
  //

  if (action == pass) {
    // send back to start of loop
    return 1;
  }
  // Show card
  // Bet or pass
  // p1 sees card, p0s action
  // bet or pass
  // bet+bet showdown
  // pass+pass draw
  // bet+pass p0 win
  // pass+bet+pass p1 win
  // pass+bet+bet showdown

  // actor is game.player - this gets changed after an action
  // This depends on player
  show_info(game)

      return 0;
}

int main() {
  // Deck
  arr<3> deck = create_deck();
  Gamestate game;

  int playing = 1;
  int bb = 0;
  while (true) {
    game.player = bb;
    // deal
    shuffle_deck(deck);
    game.card = {deck[game.player], deck[!game.player]};
    // I needa change order here
    // before going into round loop we switch bb

    while (playing) {
      playing = play_round(game);
    }
  }
  bb += 1;
  return 0;
}
