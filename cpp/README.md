# Texas Hold'em Poker - C++ Version

This is a C++ translation of the Python Texas Hold'em poker game, designed for learning C++ by comparing it to equivalent Python code.

## Files

- **game.h / game.cpp** - Card, Deck, Player, and TexasHoldemGame classes
- **evaluator.h / evaluator.cpp** - HandEval class for poker hand evaluation
- **main.cpp** - Main program entry point
- **test.cpp** - Test suite for hand evaluation and tiebreakers
- **CMakeLists.txt** - Build configuration
- **TRANSLATION_NOTES.md** - Detailed explanation of Python → C++ translations

## Building

### Option 1: Using g++ directly
```bash
cd cpp
# Build the game
g++ -std=c++11 main.cpp game.cpp evaluator.cpp -o texas_holdem
./texas_holdem

# Build and run tests
g++ -std=c++11 test.cpp game.cpp evaluator.cpp -o test_holdem
./test_holdem
```

### Option 2: Using CMake (recommended)
```bash
cd cpp
mkdir build
cd build
cmake ..
make
./texas_holdem    # Run the game
./test_holdem     # Run the tests
```

## Requirements

- C++11 compatible compiler (GCC 4.8+, Clang 3.3+, MSVC 2015+)
- CMake 3.10+ (if using CMake build method)

## How to Play

1. Run the program
2. Enter the number of players (2-8)
3. Enter names for each player
4. Follow the prompts for betting:
   - **C** = Call (match current bet)
   - **R** = Raise (increase the bet)
   - **F** = Fold (give up this hand)
5. After each round, choose whether to play again

## Learning C++

Read **TRANSLATION_NOTES.md** for a comprehensive guide to understanding the differences between the Python and C++ versions. The document covers:

- Memory management (pointers, new/delete)
- Type system (static typing, structs)
- Data structures (vectors, maps, sets)
- Object-oriented programming in C++
- Standard library differences
- Common pitfalls for Python programmers

## Code Comments

Throughout the C++ code, you'll find comments marked with:
```cpp
// TRANSLATION NOTE: explanation of how this differs from Python
```

These comments highlight key differences and explain C++-specific concepts.

## Comparison with Python

The C++ version maintains the exact same game logic as the Python version but demonstrates:
- Static typing vs dynamic typing
- Manual memory management vs garbage collection
- Compilation vs interpretation
- Header/implementation file separation
- C++ standard library vs Python built-ins

Study both versions side-by-side to understand how Python concepts translate to C++!

## Testing

A comprehensive test suite is included in `test.cpp` that covers:

### Hand Rankings
- Royal Flush, Straight Flush, Four of a Kind
- Full House, Flush, Straight
- Three of a Kind, Two Pair, One Pair
- High Card

### Tiebreakers (Kickers)
- **Same pair, different kickers** - Critical for determining winners when players have the same hand type
- **Same two pair** - Tests the 5th card (kicker) determines the winner
- **Same three of a kind** - Kicker cards break the tie
- **Different straights** - Higher straight wins
- **Multiple players** - Tests 3+ player scenarios

### Edge Cases
- Using best 5 cards from 7 available
- Board plays (all community cards)
- Hand ranking comparisons

Run the test suite to verify all poker logic is working correctly:
```bash
./test_holdem
```

All 18 tests should pass! The tests caught and helped fix critical kicker/tiebreaker logic that wasn't working in the original translation.
