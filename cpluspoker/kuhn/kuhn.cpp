
#include <algorithm>

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

int get_history(int index) {
  // 1111 - masks 4 bits
  return index & 0xF;
}

int get_card(int index) { return index >> 4; }

int update_history(int history, int action) { return (history << 1 | action); }

int showdown(int &c0, int &c1, int &history) {
  int mult = 1 - (2 * (c0 < c1));
  if (history == 4) {
    return mult * 1;
  }
  return mult * 2;
}

float sum_regret(Node &node) {
  return (std::max(node.regret[0], 0.f) + std::max(node.regret[1], 0.f));
}

float get_strategy(Node &node) {
  float total_regret = sum_regret(node);
  if (total_regret <= 0) {
    return 0.5f;
  }
  return std::max(node.regret[0], 0.f) / total_regret;
}

float ev_node(Node &node, float ev_pass, float ev_bet) {
  float strategy = get_strategy(node);
  return (1 - strategy) * ev_pass + strategy * ev_bet;
}

void update_strategy_sum(Node &node, float reach_prob) {
  float strategy = get_strategy(node);
  node.strategy_sum[0] += reach_prob * (1 - strategy);
  node.strategy_sum[1] += reach_prob * strategy;
}

// include reach prob
void update_regret(Node &node, float reach_prob, float ev_pass, float ev_bet) {
  // opponents reach prob
  float ev = ev_node(node, ev_pass, ev_bet);
  node.regret[0] += reach_prob * (ev_pass - ev);
  node.regret[1] += reach_prob * (ev_bet - ev);
}

float cfr(int &c0, int &c1, int history, float r0, float r1) {
  // Terminal Nodes --
  // Showdown
  if (history == 4 || history == 7 || history == 11) {
    return showdown(c0, c1, history);
  }
  // Fold
  if (history == 6 || history == 10) {
    return (history == 6) ? 1 : -1;
  }

  Node &node = get_node(c0, c1, history);

  // float ev_pass =
  //     cfr(c0, c1, update_history(history, 0), reach * node.strategy[0]);
  // float ev_bet =
  //     cfr(c0, c1, update_history(history, 1), reach * node.strategy[1]);
  //
  // update_regret(node, ev_pass, ev_bet);
  // update strategy
  // Need to track more things

  // return ev_node(node, ev_pass, ev_bet);
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

      ev_bet = cfr(c0, c1, update_history(history, 0), node.strategy[0]);
      ev_pass = cfr(c0, c1, update_history(history, 1), node.strategy[1]);
      update_regret(node, ev_pass, ev_bet);
      // Use ev node to calculate regret
      // This is done within the function for everyone else
      //
    }
  }

  return 0;
}
