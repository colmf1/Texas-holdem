#include <algorithm>
#include <array>
#include <iostream>
#include <random>

// Whoops idk 3 card poker lol
// My betting round is wrong
// action are

// Pass-Pass: Showdown, high card wins
// Pass-Bet-Call: Showdown, high card wins
// Pass-Bet-Fold: Bettor wins pot
// Bet-Call: Showdown, high card wins
// Bet-Fold: Bettor wins pot

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
  int player = 0;
  int action = 0;
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

// Example
// Infostate: 5(pK),
// Strategy:0.5, Terminal:[1,0], EV = [0,X]
// X=Average EV of all subtrees after bet

// opponent moves,
// Infostate: (player)9 + (action=bet)6 + (0/1) = 15/16
// 15. Strategy = .5, Terminal = 1, EV = [1,1]
// 16. Strategy = .5, Terminal = 1, EV = [1,1]
// X = Strategy*EV
//   = ((0.5*1 + 0.5*1) + (0.5*1 + 0.5*1) / 2)
//   = 1

// Update infostate
// Bet always wins with this hand
// Current EV = 0.5
// EV always bet = 1
// EV always pass = 0
// Regret_bet  = EV of always betting - EV current strategy
//             = 1 - 0.5= 0.5
// Regret_pass = 0
// Cum_Regret = [max(0,0), max(0,0.5)] - Need to only take positive regrets,
// this added to sum every visit
//            = [0, 0.5] - Keeps compounding every round, not for these cards
//            but could be both pos
// Normalise
// total regret = 0.5+0 = 0.5 - sum cumulative regrets from both sides
// Strategy = Strategy / total regret
// Update strategy all at once I think

// So the worst part is I don't think I need poker
// I just need a way of traversing the tree :/

// I just need to start at root,
// Traverse tree
// Calculate EV at bottom
// Bubble back up

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

int play_round(Gamestate &game) {
  int infostate =
      (9 * game.player) + 3 * (game.action + 1) + game.card[game.player];

  // Players both ante 1 chip at start
  // 0 bet/pass - nothing return 1
  // pass pass - showdown for 2 chips
  // bet bet - showdown for 4 chips
  // bet pass- p1 wins 2 chips
  //

  print("Infostate", infostate);
  print("Pass(0) or Bet(1)");
  int action = get_action();

  //

  // Bet bet showdown for 4

  // unless its r0 - passing will end the round with no winner
  if (!(game.action == 0) and (action == 0)) {
    // pass pass -
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
