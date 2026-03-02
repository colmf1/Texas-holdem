# Kuhn Poker CFR

## Node Indexing
- Cards: Jack(0), Queen(1), King(2).
- Actions: Pass(0), Bet(1)

### Action Encoding
- We start with a trailing 1, so we can tell the difference between root and pass.
- Each action causes a bit shift, so we can easily move to the next branch in the tree
- It ends up spelling out 1-7, then 10,11. I just ignore 8,9 in any function.
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
- card << 4 | actions 

