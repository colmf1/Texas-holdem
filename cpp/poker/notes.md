

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


I'll get to bucketing later, I feel like I need a handeval before I can bucket sensibly anyway
