# Kuhn Poker CFR

## Node Indexing
- Cards: Jack(0), Queen(1), King(2).
- Actions: Pass(0), Bet(1)

### Action Encoding
- We start with a trailing 1, so we can tell the difference between root and pass.
- Each action(1/0) is slotted into the leftmost location, and our action is the integer
- actions << 1 | action.

| History | Binary | Actions | 
|---------|--------|---------|
| (root)  | `1`    | 1       |
| p       | `10`   | 2       |
| b       | `11`   | 3       |
| pp      | `100`  | 4       |
| pb      | `101`  | 5       |
| bp      | `110`  | 6       |
| bb      | `111`  | 7       |
| pbp     | `1010` | 10      |
| pbb     | `1011` | 11      |

### Unique Node Index
- Since the max number of bits is 4, we can just shift card over and stick it onto the back 
- card(3) << 4 | actions(11)
- 110000 + 000011 = 43 max index

## Counterfactual regret minimisation
- We loop through cards for both players, and calculate cfr for the root node
- CFR calculates EV = pbet*evbet + ppass*evpass
- If ev is unknown, cfr is called recursivly on both
- If the node is terminal we can return ev up the ladder
- At each decision node we track cumulative strategy and regret for both actions
- At any given point 
    - curr_strategy = regret[bet]/cum_regret[bet+pass]
    - curr_regret[action] = ev[action] - ev[curr_strategy]
- We use this to calculate regret of this action
- Before updating cumulative strategy+regret, we weight by reach probability
- Strategy: weight by our own actions 
- Regret: weight by opponents actions 

## Output 

Table below shows Bet probability on root directory
- Root Queen never bets, because bet has higher downside and same upside 
- Root Jack bets 20%, despite being lower rated, due to chance of bluffing a queen

| ITER | JACK | QUEEN | KING |
|------|------|-------|------|
| 20 | 0.10 | 0.06 | 0.83 |
| 40 | 0.24 | 0.03 | 0.67 |
| 60 | 0.16 | 0.02 | 0.75 |
| 80 | 0.26 | 0.01 | 0.59 |
| 100 | 0.21 | 0.01 | 0.67 |
| 120 | 0.18 | 0.01 | 0.72 |
| 140 | 0.25 | 0.01 | 0.63 |
| 160 | 0.24 | 0.01 | 0.64 |
| 180 | 0.21 | 0.01 | 0.68 |
| 200 | 0.19 | 0.01 | 0.71 |
| 220 | 0.20 | 0.01 | 0.68 |
| 240 | 0.24 | 0.00 | 0.62 |
| 260 | 0.23 | 0.00 | 0.64 |
| 280 | 0.22 | 0.00 | 0.67 |
| 300 | 0.20 | 0.00 | 0.69 |
| 320 | 0.20 | 0.00 | 0.69 |
| 340 | 0.23 | 0.00 | 0.65 |
| 360 | 0.24 | 0.00 | 0.63 |
| 380 | 0.22 | 0.00 | 0.65 |
| 400 | 0.21 | 0.00 | 0.67 |
| 420 | 0.20 | 0.00 | 0.68 |
| 440 | 0.20 | 0.00 | 0.69 |
| 460 | 0.21 | 0.00 | 0.67 |
| 480 | 0.23 | 0.00 | 0.64 |
| 500 | 0.24 | 0.00 | 0.64 |
