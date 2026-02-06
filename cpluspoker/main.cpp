#include <iostream>
#include <string>
#include <array>

int main() {
    // I think this is pretty cool
    std::array<int, 52> deck;
    for (int i=0; i<52; i++){
        std::cout << std::to_string(i) << ": ";
        int rank = i/4;
        int suit = i%4;
        
        // When defining our card, We take the bits of the rank, i.e. 3 = 11, and move them over by 2
        // Then we concatenate suit, 3 of spades = 1100
        deck[i] = (rank << 2) | suit;
        std::cout << std::to_string(rank) << std::to_string(suit) << " = " << 
            // You get the rank and suit back by these 
            // For rank, we just chuck off the suit bits with >> 2 just the reverse
            // For suit, we just mask all but the last two bits with this magic 
            // & 0b11 = mask all but the last two bits
            // & 3 also works, because 0b means binary 3
            std::to_string(deck[i] >> 2) << std::to_string(deck[i] & 0b11) << "\n";
    }
   return 0;
}
