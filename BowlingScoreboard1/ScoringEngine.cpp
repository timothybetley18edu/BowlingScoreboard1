#include <iostream>
#include <vector>
#include "Frame.cpp"

using namespace std;

class ScoringEngine {
private:
	vector<Frame> frames;

public:
	ScoringEngine() {
		for(int i = 0; i < 9; i++) {
			frames.emplace_back(false);
		}
		frames.emplace_back(true); // Tenth frame
	}
	
	int roll(int pins) {
		for (size_t i = 0; i < frames.size(); ++i) {
			if (!frames[i].isComplete()) {
				frames[i].addRoll(pins);
				return i; // Return the frame index where the roll was added
			}
		}
		return -1; // All frames are complete
	}

	int score() {
		int total = 0;
		for (size_t i = 0; i < frames.size(); ++i) {
			total += frames[i].getScore();
			if (i < 9) {
				if (frames[i].isStrike()) {
					total += strikeBonus(i);
				}
				else if (frames[i].isSpare()) {
					total += spareBonus(i);
				}
			}
		}
		return total;
	}
private:
	int strikeBonus(size_t frameIndex) {
		int bonus = 0;
		if (frameIndex + 1 < frames.size()) {
			const Frame& next = frames[frameIndex + 1];
			bonus += next.getRoll(0);
			if (next.isStrike() && frameIndex + 2 < frames.size()) {
				bonus += frames[frameIndex + 2].getRoll(0);
			}
			else {
				bonus += next.getRoll(1);
			}
		}
		return bonus;

	}
	int spareBonus(size_t frameIndex) {
		if (frameIndex + 1 >= frames.size()) return 0;
		return frames[frameIndex + 1].getRoll(0);
	}
};