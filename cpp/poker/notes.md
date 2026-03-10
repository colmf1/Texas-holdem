

Focus is on hand eval first its gonna make or break project
Did some research, tools i need are 
- Hexidecimal 
    - fastest way to identify cards is with bit actions 
    - best way to create bitmasks are with hexidecimal 
    - 0x1FFF - this is our hero, 13 1s to mask suits
- Bitwise ops 
    - &, And, both must be 1. 1&1 = 1, 1010 & 1001 = 1000
    - |, Or, either can be 1. 0&1 = 1, ...
    - ~, Not, flip the bits
    - ^, Xor, one or the other, not both
    - && is actual and used for logic2
    - __builtin_popcount(ll) - count the 1s, (uint64)
- Unsigned ints
    - Because I'll be working with big unsigned ints, 1 isn't enough
    - I need a big ugly 1ULL for uint64
    - int is only 32 bits, 1<<33 is out of range

When we're evaluating hands, we need to check for collisions 
For this we just use bitboards, 64bits, each card in the deck has a bit 
bits 0-12 are clubs etc 
If we stack in rows, aces are down left
This is an quick+easy way to check hands 
Just gotta do it 

Hand eval is looking good 
Currently returning no pairs, toak, foak, nostraight, no flush
These can be used for both bucketing and hand eval 
my code is a mess and getting compiler warnings but it compiles and runs 



Wheel straight
- This is a trick for grabbing the wheel straight 
- We want to count the number in a row from 2,3,4,5 to check for wheel straight 
- If we do ranks & 0xF(1111) We get the binary table below 
- We can then make a lookup table that returns the consecutive bits

```
constexpr std::array<int8_t, 16> make_wheel_lookup(){
  // Lookup for our wheel straight
  // We want to count consecutive ranks from 2-5
  // If it's not concecutive from 2-> it doesn't matter we break
  // We loop through all possible combinations of 4 bits, 0000,0001 etc
  // At each index 
  // grab the last bit,
  // if its 1, increment count, check next bit
  // break at 0
  std::array<int8_t, 16> lookup{};
  for (int i = 0; i < 16; i++) {
      int count = 0;
      while (count < 4 && (i >> count & 1)) count++;
      lookup[i] = count;
  }
  return lookup;
}

void count_straight(uint64_t hand, std::array<int, 5> &evals) {
  // Squash our hand onto 1 line, ignore suits
  uint16_t ranks = ((hand)&MASK13) | ((hand >> 13) & MASK13) |
                   ((hand >> 26) & MASK13) | ((hand >> 39) & MASK13);
  
  // We & it with shifting it over 1
  // r2 = 0111 & 0011 = 0011,
  // r3 = 0011 & 0001 = 0001
  // later we do !! to turn into 1,0 flag
  uint16_t r2 = ranks & (ranks >> 1);
  uint16_t r3 = r2 & (ranks >> 2);
  uint16_t r4 = r3 & (ranks >> 3);
  uint16_t r5 = r4 & (ranks >> 4);
  
  // This returns no between 1-4,
  // from 2-5, how many consecutive cards are there 
  // if there's an ace, and a low streak, we return low+1, otherwise 0
  int low   = WHEEL_LOOKUP[ranks & 0xF];
  int wheel = (ranks >> 12 & 1) && low ? low + 1 : 0;

  evals[3] = std::max(1 + !!r2 + !!r3 + !!r4 + !!r5, wheel);
}


```

index → binary → consecutive from bit 0 → lookup value
0   →  0000  → none                   → 0
1   →  0001  → 2                      → 1
2   →  0010  → no 2, chain broken     → 0
3   →  0011  → 2,3                    → 2
4   →  0100  → no 2, chain broken     → 0
5   →  0101  → 2, gap at 3            → 1
6   →  0110  → no 2, chain broken     → 0
7   →  0111  → 2,3,4                  → 3
8   →  1000  → no 2, chain broken     → 0
9   →  1001  → 2, gap at 3            → 1
10  →  1010  → no 2, chain broken     → 0
11  →  1011  → 2,3, gap at 4          → 2
12  →  1100  → no 2, chain broken     → 0
13  →  1101  → 2, gap at 3            → 1
14  →  1110  → no 2, chain broken     → 0
15  →  1111  → 2,3,4,5                → 4

- Next steps 
    - Hand eval - fix header files, hidden funcs under namespaces
    - I can group toak and foak together definitely
    - I'll need to figure out a way around branching 
    - But instead of 1,0 toakfoak, I count num_matches, 2-4
    - If num_matches>2 don't include it with num_pairs
    - num_matches-2? toak=1,foak=2? 
    - Add high card to the mix 
    - I want high card within flush straight pairs etc 
    - I want the highest card not on the table specifically
    - Tiebreakers are never decided by table cards
    - Of the players hands, I need to identify the value card
    - It's compared in a tiebreak
    - Actually ace high f<



