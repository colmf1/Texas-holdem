
struct Node {
  float regret[2] = {0.f, 0.f};
  float strategySum[2] = {0.f, 0.f};
  // I should track payout
  // If the node isn't terminal, there's a payout
  //
};

Node nodeTable[24];

Node &get_node(int card, int history) {
  return nodeTable[(card << 3) | history];
}

void update_history(int &history, int action) {
  history = (history << 1) | action;
}

int showdown(c0, c1, history) {
  // if history is odd it's a fold
  //
}

float get_EV(int c0, int c1, int history) {
  // detect each case from history bits
  // passes are always even, and end the game
  // if history comes in that isn't 2 we can return an endgame
  if (history == 4) {
    // Pass pass
    // Showdown for 1
  }

  if (history == 6) {

    // 6, 10 are folds for p1,p0
    // We can
  }
  if (history == 0 | history == 11) {
    // 7,11 are showdowns
  }

  // Possible histories
  // p0
  // nothing=1
  //
  // p1
  // p=2, b=3
  //
  // p0
  // pp=4 terminal node
  // pb=5
  //
  // p1 (not really)
  // bp=6, bb=7 - both terminal
  //
  // pbp=10 - terminal(1)
  // pbb=11 - terminal(2)
}

int main() {
  Node node;
  for (int c0 = 0; c0 < 3; c0++) {
    for (int c1 = 0; c1 < 3; c1++) {
      if (c0 == c1) {
        break;
      }
      int history = 1;
      cfr(c0, c1, history);
    }
  }

  return 0;
}
