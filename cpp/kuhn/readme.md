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
- At the root node, to act wisely we need the EV of our actions 
- CFR function returns expected value of a node
- If the node is terminal, i.e. pp, it will be a number
- Otherwise we recursivly call the function on it's subnodes till we get some answers 

### Algorithm 
- Node contains
    - Cumulative Regret {0,0}
    - Strategy Sum {0,0}
- Reach probability[0,1] - Given [my,opp] decisions, what's the probability we got here 
- I didn't think reach probabilities were needed, but then I took them out and it didn't work
- Regret - How much did I lose out by playing my strategy over this action always 
    - Regret[Action] += (EV[Action] - EV[Strategy]) * reach[opp]
- Current strategy = Regret[Action]/total_positive_regret 
- Strategysum[action] += current strategy 
- Strategy at any given point is nonsense, strategy sum converges to nash equilibrium

## Output 
- I'm an idiot, the bot would have beat me after 20 iterations 
- Table below shows Bet probability on root directory
- Queen-bet converges to 0
- if king, bet, they call, lose2
- if jack, bet, they fold, win1, would've won1 with pass pass 
- jack-bet is non zero because you can bluff a queen into folding

| ITER | JACK | QU3N | KING |
|    20| 0.10 | 0.06 | 0.83 | 
|    40| 0.24 | 0.03 | 0.67 | 
|    60| 0.16 | 0.02 | 0.75 | 
|    80| 0.26 | 0.01 | 0.59 | 
|   100| 0.21 | 0.01 | 0.67 | 
|   120| 0.18 | 0.01 | 0.72 | 
|   140| 0.25 | 0.01 | 0.63 | 
|   160| 0.24 | 0.01 | 0.64 | 
|   180| 0.21 | 0.01 | 0.68 | 
|   200| 0.19 | 0.01 | 0.71 | 
|   220| 0.20 | 0.01 | 0.68 | 
|   240| 0.24 | 0.00 | 0.62 | 
|   260| 0.23 | 0.00 | 0.64 | 
|   280| 0.22 | 0.00 | 0.67 | 
|   300| 0.20 | 0.00 | 0.69 | 
|   320| 0.20 | 0.00 | 0.69 | 
|   340| 0.23 | 0.00 | 0.65 | 
|   360| 0.24 | 0.00 | 0.63 | 
|   380| 0.22 | 0.00 | 0.65 | 
|   400| 0.21 | 0.00 | 0.67 | 
|   420| 0.20 | 0.00 | 0.68 | 
|   440| 0.20 | 0.00 | 0.69 | 
|   460| 0.21 | 0.00 | 0.67 | 
|   480| 0.23 | 0.00 | 0.64 | 
|   500| 0.24 | 0.00 | 0.64 | 

