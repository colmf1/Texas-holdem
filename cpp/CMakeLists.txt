cmake_minimum_required(VERSION 3.10)
project(TexasHoldem)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main game executable
add_executable(texas_holdem
    main.cpp
    game.cpp
    evaluator.cpp
)

# Test executable
add_executable(test_holdem
    test.cpp
    game.cpp
    evaluator.cpp
)

# TRANSLATION NOTE: In Python, no build system is needed - scripts run directly.
# C++ requires compilation. This CMakeLists.txt makes building easy:
#   mkdir build && cd build
#   cmake ..
#   make
#   ./texas_holdem    # Run the game
#   ./test_holdem     # Run the tests
