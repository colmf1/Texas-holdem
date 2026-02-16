#include <algorithm>
#include <array>
#include <iostream>
#include <random>

// Helper funcs
template <std::size_t N> using arr = std::array<int, N>;
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
  int action;
};

int get_action() {
  // returns Pass = 3, Bet = 6
  // doesn't really matter
  int action;
  std::cin >> action;
  return (action + 1) * 3;
}

int showdown(Gamestate &game) {
  // returns index of winner
  return game.card[0] < game.card[1];
}

// I need to make a bunch of maps eg
// I think I calculate EV once, and multiply by -1 for p1
// Idk why I'm storing these as arrays, one int is good since only 1 decision
// Look at everything as probability to bet
// apart from move 1 pass ends game with ev=0 anyway

// Infostate: 5(pK),
// Strategy:0.5, Terminal: 0, Payout=X
// Need to work out expected payout from betting
// Non terminal actions result in an opponent move

// opponent moves,
// Infostate: 9 + 6 + (0/1) = 15/16
// Strategy = .5, Terminal = 1, EV = 1
// X = Strategy*EV[1]

// Update infostate
// Current EV = 0.5*0.5=0.25
// Next step is to calculate regret
//

int play_round(Gamestate &game) {

  // Infostate is what the Bot will see
  // Number between 0-18 - 3cards*3actions*2players (1st or second)

  // I'm imagining that these will be nodes on our cfr
  // Each infostate will have an associated float array strategy
  // eg. [0.2, 0.8] = [P(pass), P(bet)]
  // We initialise everything at [.5, .5]
  // We make our decision
  // We then calculate regret??
  // Idk how that works
  // Will need to calculate it every round

  int infostate = (9 * game.player) + game.action + game.card[game.player];

  print("Infostate", infostate);
  print("Pass(0) or Bet(1)");
  // Get action will be replaced with bot action
  int action = get_action();

  // Terminal nodes are ones which return 0 - this is where we calculate regret.
  // They're then recursivly summed.

  // unless its r0 - passing will end the round with no winner
  if (!(game.action == 0) and (action == 3)) {
    print("Passed - ended round");
    // payoff = 0
    return 0;
  }

  // double bets have a showdown
  if (game.action == 6 and action == 6) {
    // Win  payoff = 1
    // Lose payoff = -1
    int winner = showdown(game);
    print("Winner", winner);
    // I'll needa to some regret maxing here I assume
    return 0;
  }

  // Any other scenario it gets updated and we repeat
  game.action = action;
  game.player = !game.player;
  return 1;
}

int main() {
  // Deck
  arr<3> deck = create_deck();
  Gamestate game;

  print("New Game");
  int playing;
  int bb = 0;
  while (true) {
    // round initialisation
    print("New Round");
    playing = 1;
    game.player = bb;
    shuffle_deck(deck);
    game.card = {deck[game.player], deck[!game.player]};
    game.action = 0;

    // Play a round
    while (playing) {
      playing = play_round(game);
    }

    bb = !bb;
  }
  return 0;
}
