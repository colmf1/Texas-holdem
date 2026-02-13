#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <string>

// This is all the info we need
// I track big blind outside of the gamestate
// everything else is here

// This could all live elsewhere really
struct Gamestate {
  int hand[2][2];
  int table[5];
  int round;
  int chips[2];
  int bet[2];
  bool id;
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
  std::cout << "Bets - 0:" << game.bet[game.id] << ", 1:" << game.bet[game.id]
            << "\n";
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
  game.id = 0;
  game.player = 0;
  game.chips[0] = 5000;
  game.chips[1] = 5000;
}

void bresets(Gamestate &game) {
  // reset bet
  game.bet[0] = 0;
  game.bet[1] = 0;
}

void fold(Gamestate &game) {
  game.chips[!game.player] += game.bet[0] + game.bet[1];
  bresets(game);
}

void call(Gamestate &game) {
  game.chips[game.player] -= (game.bet[!game.player] - game.bet[game.player]);
  ;
  game.bet[game.player] += (game.bet[!game.player] - game.bet[game.player]);
  ;
  // Pass over action - this needs to go back to the start of the function
  game.player = !game.player;
}

void bethpot(Gamestate &game) {
  game.chips[game.player] -=
      (game.bet[game.player] + game.bet[!game.player]) / 2;
  game.bet[game.player] += (game.bet[game.player] + game.bet[!game.player]) / 2;
  game.player = !game.player;
}

void betfpot(Gamestate &game) {
  game.chips[game.player] -= game.bet[game.player] + game.bet[!game.player];
  game.bet[game.player] += game.bet[game.player] + game.bet[!game.player];
  game.player = !game.player;
}

void allin(Gamestate &game) {
  show_options(game, 1);
  int action = get_input();
  if (action == 0) {
    fold(game);
  } else {
    call(game);
  }
}

int showdown(Gamestate &game) {
  // not really sure if i cba with this one
  // cba implementing hand eval from nums but ahwell
  // anyway
  // if we've got here we need to pick a winner
  // make a hand eval func
  // return index of loser
  // force them to fold
  return 0;
}

int hand_eval(Gamestate &game) {
  int winner;
  // ending for the night here
  game.hand[0];
  game.hand[1];

  return winner;
}

void call_or_check(Gamestate &game) {
  if (game.bet[0] == game.bet[1]) {
    game.player = !game.bb;
    game.round += 1;
  } else {
    call(game);
  }
}

bool play_round(Gamestate &game) {
  if (game.round == 5) {
    showdown(game);
  }

  show_info(game);
  int action = get_input();

  switch (action) {
  case 0:
    fold(game);
    return false;

  case 1:
    call_or_check(game);
    return true;

  case 2:
    bethpot(game);
    return true;

  case 3:
    betfpot(game);
    return true;

  case 4:
    allin(game);
    return false;

  default:
    std::cout << "Invalid action\n";
    return false;
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
    // if we only wipe the losers hand in fold we know the winner
  }
  return 0;
}
