#include <array>
#include <string>

// Our node values
using nodeval = std::array<float, 2>;
using tree = std::array<std::array<float, 2>, 12>;

// Just give up on the stupid shit

struct Node {
  float regret[2] = {0.f, 0.f};
  float strategySum[2] = {0.f, 0.f};
};

// Stores all the nodes
// When I loop through all states, I update regret and strategy
Node nodeTable[24];

Node &getNode(int card, int history) {
  return nodeTable[(card << 3) | history];
}

void update_history(int &history, int action) {
  history = (history << 1) | action;
}
void cfr(int c0, int c1, Node &node) {}

int main() {
  Node node;
  for (int c0 = 0; c0 < 3; c0++) {
    for (int c1 = 0; c1 < 3; c0++) {
      int history = 1;
      node = getNode(c0, history);
      cfr(c0, c1, nodeTable[node]);
    }
  }

  return 0;
}
