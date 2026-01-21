# Texas Hold'em Poker - C++ Version

This is a C++ translation of the Python Texas Hold'em poker game, designed for learning C++ by comparing it to equivalent Python code.

## Files

- **game.h / game.cpp** - Card, Deck, Player, and TexasHoldemGame classes
- **evaluator.h / evaluator.cpp** - HandEval class for poker hand evaluation
- **main.cpp** - Main program entry point
- **CMakeLists.txt** - Build configuration
- **TRANSLATION_NOTES.md** - Detailed explanation of Python → C++ translations

## Building

### Option 1: Using g++ directly
```bash
cd cpp
g++ -std=c++11 main.cpp game.cpp evaluator.cpp -o texas_holdem
./texas_holdem
```

### Option 2: Using CMake (recommended)
```bash
cd cpp
mkdir build
cd build
cmake ..
make
./texas_holdem
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
