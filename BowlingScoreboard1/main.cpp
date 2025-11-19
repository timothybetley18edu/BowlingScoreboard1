#include <iostream>
#include "ScoringEngine.cpp"

using namespace std;

int main() {
    ScoringEngine game;

    // Perfect game: 12 strikes
    for (int i = 0; i < 12; ++i) {
        game.roll(10);
    }
    cout << "Final Score: " << game.score() << std::endl; // Output: 300
}
