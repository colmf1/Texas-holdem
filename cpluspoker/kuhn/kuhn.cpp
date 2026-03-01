
#include <algorithm>
#include <array>

using float2 = std::array<float, 2>;
// Tree design
// Based on these funcs

// Node &get_node(int c0, int c1, int actions) {
//   // p1
//   if (actions == 2 || actions == 3) {
//     return nodeTable[(c1 << 4) | actions];
//   }
//   // p2
//   return nodeTable[(c0 << 4) | actions];
// }
//
// int update_actions(int actions, int action) { return (actions << 1 | action);
// }
//

// Possible action histories
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
  float2 regret = {0.f, 0.f};
  float2 strategy_sum = {0.f, 0.f};
};

// I think 43 would be fine but dont trust my brain
Node nodeTable[48];

Node &get_node(int c0, int c1, int actions) {
  // p1
  if (actions == 2 || actions == 3) {
    return nodeTable[(c1 << 4) | actions];
  }
  // p2
  return nodeTable[(c0 << 4) | actions];
}

int update_actions(int actions, int action) { return (actions << 1 | action); }

int flip_sign(int cond) { return 1 - (2 * cond); }

int showdown(int &c0, int &c1, int &actions) {
  int mult = flip_sign(c0 < c1);
  if (actions == 4) {
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

  int actp, actb;
  float2 rp, rb;

  actp = update_actions(actions, 0);
  actb = update_actions(actions, 1);

  rp = update_reach(actions, (1 - strategy), reach);
  rb = update_reach(actions, strategy, reach);

  float ev_pass = cfr(c0, c1, actp, rp);
  float ev_bet = cfr(c0, c1, actb, rb);
  float ev = ev_node(strategy, ev_pass, ev_bet);

  // Opponents reach prob
  int player = get_player(actions);
  update_regret(node, ev, reach[!get_player(actions)], ev_pass, ev_bet, player);
  // players reach prob
  update_strategy_sum(node, strategy, reach[get_player(actions)]);

  return ev;
}

int main() {
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
  return 0;
}
