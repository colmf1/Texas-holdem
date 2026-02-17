#include <array>

// CFR for kuhn poker.
// We need a smart way of making unique ints from all combos
// These are all possible decision states
// I Loop through 0,1,2
// int Strategy[12, 12] - can probably be compressed because it must add to 1
// int cumRegret[12,12] - Regret = EV of always doing [pass, bet] - EV of
// current strategy int EV[12,12] Expected payout. Assuming all strategies 50,50
// Eg when in BK-Betting EV=2, Passing EV=-1

// regret = [
//            bet_payout - strategy*bet_payout,
//            pass_payout -(1-strategy)+pass_payout
// ]
// The ratios of the above will be our new strategy - they increase over time
// i.e. when pass doesnt pay off first go, regret= [2,0], new_strategy =
// regret[0] /(2+0) = 2/2=1 strategy = payout_bet/all_payouts If a magic world
// existed where a pass paid off second hand regret = [4,1] new_strat = Now it
// bets always, since only positive regret goes up, the ratios always 1

// 2 - (.5*2 = 1)

// store 2 arrays
// Cum regret and strategy

// Current strategy EV= (.5 * 2) + (.5 * -1) = 1+-.5 = .5
// Betting 100% EV=

// BQ-Betting EV = chance_of_higher/lower_card(.5)*4= +- 2 = 0

// - Current strategy at BQ is to bet 50% of time
// - Current EV = (.5*2)

// When we first enter the loop, in order to make a decision
// we have to calculate the expected payout of making that decision
// both options however are unknown, but calculateable
// We first

// This is just for us - its not used.
// >5 is terminal - can calculate all returns there
// In both decisions we can calculate regret.

// We need a funciton that calculates EV
// If the node > 5 we return ints
// Otherwise we recursivly return our function

// At each stage, we calculate regret
// Regret is EV of doing an action 100% of time - EV of current strategy
// At

std::array<int, 12> node = {
    // p0
    0, // J
    1, // Q
    2, // K
    // p1
    3, // PJ
    4, // PQ
    5, // PK
    6, // BJ-T - B=-2,P=-1
    7, // BQ-T -
    8, // BK-T
    // p0
    9,  // PBJ-T
    10, // PBQ-T
    11  // PBK-T
};

// I need a function that takes a node and an action and finds other nodes
// needed i.e. 0 requires 4,5+6,7 - no jacks node+3 always pass weird way of
// calculating around though node+6 is a bet if node%3 == prev_node%3 pass
//

// 3 arrays in a struct

// float regret[12][2], strategy[12];

// Regret will be cumulative
// strategy[node] = regret[node][0] / regret[node][0] + regret[node][1]
// Strategy updated at end of loop
// We need cards in here
//

float regret(int node, int prev_node{
  // If the node is terminal i.e. > 6, we calculate regret
  if (node > 5) {
    // Showdown occurs here if we bet
    // For <9 - card=prev node - 6, >9 card=prevnode-3
    //
    // We need to get the cards and determine a winner
    //
    int bet_payout[2] = []
    //
  }

  return 0;
}

int main(){
  return 0;
}
