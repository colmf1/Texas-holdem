#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>

using float2 = std::array<float, 2>;

struct Node {
  float2 regret = {0.f, 0.f};
  float2 strategy_sum = {0.f, 0.f};
};

// 43 is max index
Node nodeTable[43];

Node &get_node(int c0, int c1, int actions) {
  // p1
  if (actions == 2 || actions == 3) {
    return nodeTable[(c1 << 4) | actions];
  }
  // p2
  return nodeTable[(c0 << 4) | actions];
}

int update_actions(int actions, int action) { return (actions << 1 | action); }

// This gets used for ev multiplier in multiple locations
// [0,1] -> [1,-1]
int flip_sign(int cond) { return 1 - (2 * cond); }

int showdown(int &c0, int &c1, int &actions) {
  int mult = flip_sign(c0 < c1);
  if (actions == 4) { // PP
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
  // Strategy is Pbet
  return std::max(node.regret[1], 0.f) / total_regret;
}

float ev_node(float strategy, float ev_pass, float ev_bet) {
  return (1 - strategy) * ev_pass + strategy * ev_bet;
}

void update_strategy_sum(Node &node, float strategy, float reach_prob) {
  node.strategy_sum[0] += reach_prob * (1 - strategy);
  node.strategy_sum[1] += reach_prob * strategy;
}

void update_regret(Node &node, float ev, float reach_prob, float ev_pass,
                   float ev_bet, int player) {

  int sign = flip_sign(player);
  node.regret[0] += reach_prob * sign * (ev_pass - ev);
  node.regret[1] += reach_prob * sign * (ev_bet - ev);
}

int get_player(int actions) { return (actions == 2 || actions == 3); }

float2 update_reach(int actions, float strategy, float2 reach) {
  reach[get_player(actions)] *= strategy;
  return reach;
}

float cfr(int c0, int c1, int actions, float2 reach) {
  // Terminal Nodes -------------
  // Showdown
  if (actions == 4 || actions == 7 || actions == 11) {
    return showdown(c0, c1, actions);
  }
  // Fold
  if (actions == 6 || actions == 10) {
    return (actions == 6) ? 1 : -1;
  }

  // All other Nodes --------------
  Node &node = get_node(c0, c1, actions);
  float strategy = get_strategy(node);

  int actp = update_actions(actions, 0);
  int actb = update_actions(actions, 1);

  float2 rp = update_reach(actions, (1 - strategy), reach);
  float2 rb = update_reach(actions, strategy, reach);

  float ev_pass = cfr(c0, c1, actp, rp);
  float ev_bet = cfr(c0, c1, actb, rb);
  float ev = ev_node(strategy, ev_pass, ev_bet);

  int player = get_player(actions);
  update_regret(node, ev, reach[!player], ev_pass, ev_bet, player);
  update_strategy_sum(node, strategy, reach[player]);
  return ev;
}

std::string state_string(int actions, int card) {
  std::string res;
  while (actions > 1) {
    // Mask the last bit, if it's 1, bet, otherwise pass
    // then strip actions of it's last bit.
    res += (actions & 1) ? "b" : "p";
    actions >>= 1;
  }
  res += ":" + std::string(1, "JQK"[card]);
  return res;
}

void print_strategies() {
  for (int card = 0; card < 3; card++) {
    Node node = get_node(card, card, 1);
    float strategy =
        node.strategy_sum[1] / (node.strategy_sum[1] + node.strategy_sum[0]);
    std::cout << std::fixed << std::setprecision(2) << strategy << " | ";
  }
  std::cout << '\n';
}

int main() {
  std::cout << "| ITER | JACK | QU3N | KING |" << "\n";
  for (int i = 0; i <= 500; i++) {
    if (i != 0 and i % 20 == 0) {
      std::cout << "|" << std::setw(6) << i << "| ";
      print_strategies();
    }

    for (int c0 = 0; c0 < 3; c0++) {
      for (int c1 = 0; c1 < 3; c1++) {
        if (c0 == c1) {
          continue;
        }
        int actions = 1;
        float2 reach = {1.0, 1.0};
        (void)cfr(c0, c1, actions, reach);
      }
    }
  }
  return 0;
}
