
// -- TODO ---------------

// Regret matching
// Reach probabilities
// strategy sum

// Tree design

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

// possible cards, 0,1,2
// Largest history is 11
// 11 in binary = 1011
// 2 in binary = 10
// Node in binary of having pbb with a king = 10|1011=101011 = 43

struct Node {
  float regret[2] = {0.f, 0.f};
  float strategy[2] = {0.f, 0.f};
  float strategy_sum[2] = {0.f, 0.f};
};
// I think 43 would be fine but dont trust my brain
Node nodeTable[48];

Node &get_node(int c0, int c1, int history) {
  // p1
  if (history == 2 || history == 3) {
    return nodeTable[(c1 << 4) | history];
  }
  // p2
  return nodeTable[(c0 << 4) | history];
}

int update_history(int history, int action) { return (history << 1 | action); }

int showdown(int &c0, int &c1, int &history) {
  // winner = c0<c1 0|1
  // mult = 1|-1
  int mult = 1 - (2 * (c0 < c1));
  if (history == 4) {
    // pp
    return mult * 1;
  }
  // bb
  return mult * 2;
}

float ev_node(Node node, float ev_pass, float ev_bet) {
  return node.strategy[0] * ev_pass + node.strategy[1] * ev_bet;
}

void update_regret(Node &node, float ev_pass, float ev_bet) {
  float ev = ev_node(node, ev_pass, ev_bet);
  node.regret[0] += ev_pass - ev;
  node.regret[1] += ev_bet - ev;
}

float cfr(int &c0, int &c1, int history) {
  // Terminal Nodes --
  // Showdown
  if (history == 4 || history == 7 || history == 11) {
    return showdown(c0, c1, history);
  }
  // Fold
  if (history == 6 || history == 10) {
    return (history == 6) ? 1 : -1;
  }
  // Not terminal nodes
  // We needa do cfr
  // We need to update our regret with our current strategy
  // Update our strategy
  // Return the current EV of our strategy
  //

  float ev_pass = cfr(c0, c1, update_history(history, 0));
  float ev_bet = cfr(c0, c1, update_history(history, 1));
  Node &node = get_node(c0, c1, history);

  update_regret(node, ev_pass, ev_bet);

  return ev_node(node, ev_pass, ev_bet);
}

// cfr(terminal) → returns +1.0
//     ↑
// cfr(node, P1 to act) → gets +1.0 from call branch, -1.0 from fold branch
//                       → computes weighted average, returns say +0.3
//                       → also updates regrets using those two values
//     ↑
// cfr(node, P0 to act) → gets +0.3 from bet branch, +0.1 from check branch
//                       → computes weighted average, returns say +0.2
//                       → also updates regrets using those two values
//     ↑
// main() → receives the EV of the whole deal
//
int main() {
  Node node;
  for (int c0 = 0; c0 < 3; c0++) {
    for (int c1 = 0; c1 < 3; c1++) {
      if (c0 == c1) {
        continue;
      }
      int history = 1;
      float ev_bet, ev_pass;
      node = get_node(c0, c1, history);

      ev_bet = cfr(c0, c1, update_history(history, 0));
      ev_pass = cfr(c0, c1, update_history(history, 1));
      update_regret(node, ev_pass, ev_bet);
      // Use ev node to calculate regret
      // This is done within the function for everyone else
      //
    }
  }

  return 0;
}
