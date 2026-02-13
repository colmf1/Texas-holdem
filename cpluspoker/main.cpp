#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <string>

// This could all live elsewhere really
struct Gamestate {
  int hand[2][2];
  int table[5];
  int round;
  int chips[2];
  int bet[2];
  // id didnt even get used lol theres no point tracking it
  // just store it as an index, and move player around
  // player references the index of the lists, as does bb
  bool player;
  bool bb;
  uint32_t history; // deal with this later
};
// Human io
std::string card_str(int card) {
  // I should just return a string?
  std::array<std::string, 4> suits = {"Spades", "Clubs", "Hearts", "Diamonds"};
  std::array<std::string, 13> ranks = {"2",     "3",    "4",  "5",  "6",
                                       "7",     "8",    "9",  "10", "Jack",
                                       "Queen", "King", "Ace"};
  return ranks[card / 4] + " of " + suits[card / 12];
}

void show_bets(Gamestate &game) {
  std::cout << "Bets - 0:" << game.bet[game.player]
            << ", 1:" << game.bet[!game.player] << "\n";
}

void show_hand(Gamestate &game) {
  if (game.round != 0) {
    std::cout << "Table\n";
    for (int i = 0; i < game.round + 2; i++) {
      std::cout << card_str(game.table[i]) << "\n";
    }
  }
  std::cout << "Hand\n"
            << card_str(game.hand[game.player][0]) << "\n"
            << card_str(game.hand[game.player][1]) << "\n\n";
}

void show_info(Gamestate &game) {
  show_bets(game);
  show_hand(game);
}

void show_options(Gamestate &game, bool allin = 0) {
  if (allin) {
    std::cout << "0:fold \n1:call";
  } else {
    if (game.bet[0] != game.bet[1]) {
      std::cout << "0:fold \n1:call \n2:raisehalf \n3:raisepot \n4:Allin\n";
    } else {
      std::cout << "0:fold \n1:check \n2:raisehalf \n3:raisepot \n4:Allin\n";
    }
  }
}

int get_input() {
  // not passing limits because that will be done elsewhere
  int x;
  std::cin >> x;
  return x;
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

// Gamestate
void init_gs(Gamestate &game) {
  game.round = 0;
  game.player = 0;
  game.chips[0] = 5000;
  game.chips[1] = 5000;
}

void bresets(Gamestate &game) {
  // reset bet
  game.bet[0] = 0;
  game.bet[1] = 0;
}

bool fold(Gamestate &game) {
  game.chips[!game.player] += game.bet[0] + game.bet[1];
  bresets(game);
  return false;
}

bool call(Gamestate &game) {
  game.chips[game.player] -= (game.bet[!game.player] - game.bet[game.player]);
  game.bet[game.player] += (game.bet[!game.player] - game.bet[game.player]);
  game.player = !game.player;
  return true;
}

bool bethpot(Gamestate &game) {
  game.chips[game.player] -=
      (game.bet[game.player] + game.bet[!game.player]) / 2;
  game.bet[game.player] += (game.bet[game.player] + game.bet[!game.player]) / 2;
  game.player = !game.player;
  return true;
}

bool call_or_check(Gamestate &game) {
  if (game.bet[0] == game.bet[1]) {
    game.player = !game.bb;
    game.round += 1;
    return true;
  }
  return call(game);
}

bool betfpot(Gamestate &game) {
  game.chips[game.player] -= game.bet[game.player] + game.bet[!game.player];
  game.bet[game.player] += game.bet[game.player] + game.bet[!game.player];
  game.player = !game.player;
  return true;
}

bool allin(Gamestate &game) {
  show_options(game, 1);
  int action = get_input();
  if (action == 0) {
    return fold(game);
  } else {
    return call(game);
  }
}

// Need hand detector funcs
// Need ranking of hands
// Need draw handling
// loser forced to fold

//
// def eval_flush(self,all_cards):
//     suits = [card.show()[-1] for card in all_cards]
//     values = [card.show()[:-1] for card in all_cards]
//     value_map = {'A': 14, 'K': 13, 'Q': 12, 'J': 11}
//     numeric_values = list(map(int, [value_map.get(v, v) for v in values]))
//     lookup = {numeric_values[i]: suits[i] for i in range(len(suits))}
//     suit_counts = Counter(suits)
//     if max(suit_counts.values()) >= 5:
//         flush_suit = [key for key, value in suit_counts.items() if value >=
//         5][0] cards = [card for card, suit in lookup.items() if suit ==
//         flush_suit] high_card = max(cards) card_str = next(k for k, v in
//         value_map.items() if v == high_card) if high_card in
//         value_map.values() else str(high_card) return cards, card_str
//     else:
//         return False
//
//
//

std::array<int, 2> hand_eval(Gamestate &game) {
  std::array<int, 2> scores;
  // Return two scores
  // If they're equal do further comparison.
  // First it goes highest rank of card within the winning combo
  // Then highest other card
  // I think this is the same for all hands so handle separately
  return scores;
}

int high_card(Gamestate &game) { return 0; }

bool play_round(Gamestate &game) {
  // Full game control in here

  // This function is ran inside a loop
  // We exit the function with true or false
  // False ends the loop, True restarts the loop

  if (game.round == 5) {
    // Check for r5 at start, showdown - all in skips to here
    std::array<int, 2> scores = hand_eval(game);

    if (scores[0] == scores[1]) {
      return high_card(game);
    }
    return scores[0] < scores[1];
    // winner = score0(20)<score1(15)=0 score0(10)<score1(15)=1
  }

  // Show info round dependant
  show_info(game);
  int action = get_input();

  switch (action) {
  case 0:
    // Fold returns false, pays the winner exits loop to endgame
    return fold(game);

  case 1:
    // Call and check
    // These are equivalent to bot
    // Call changes player, check goes onto next round
    // Only check advances the round
    return call_or_check(game);

  // Raises place bets and change player
  case 2:
    return bethpot(game);
  case 3:
    return betfpot(game);

  case 4:
    // All in gives the other player a chance to call
    // If they call returns true and sets round = 5 for a showdown
    // if they fold it returns false
    return allin(game);
  default:
    std::cout << "Invalid action\n";
    return true;
  }
}

int main() {
  // init
  Gamestate game;
  std::array<int, 52> deck = create_deck();
  init_gs(game);
  while (true) {

    // initialise round
    game.player = game.bb;
    game.round = 0;
    deal_cards(game, deck);

    // This loop holds the entire round
    // Only returns 0 in fold or win
    bool playing;
    while (!playing) {
      playing = play_round(game);
    }

    // someone wins the hand
    // winner is paid out in fold?
  }
  return 0;
}
